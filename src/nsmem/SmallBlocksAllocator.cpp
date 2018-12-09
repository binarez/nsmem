// This file is subject to the terms and conditions defined in the file 'LICENSE.txt'.

#include "nsmem/SmallBlocksAllocator.hpp"
#include "nsmem/BlockAllocator.hpp"
#include "nsmem/mem.hpp"

#include <cstdint>

namespace mem { namespace _impl { namespace _smallblocks
{
	std::size_t ceilLog2( std::uint64_t value )
	{
//		mem_assert( value > 0 );

		std::size_t pos = 0;
		if (value & (value - 1ULL))
		{
		  pos = 1;
		}
		if (value & 0xFFFFFFFF00000000ULL)
		{
		  pos += 32;
		  value = value >> 32;
		}
		if (value & 0x00000000FFFF0000ULL)
		{
		  pos += 16;
		  value = value >> 16;
		}
		if (value & 0x000000000000FF00ULL)
		{
		  pos += 8;
		  value = value >> 8;
		}
		if (value & 0x00000000000000F0ULL)
		{
		  pos += 4;
		  value = value >> 4;
		}
		if (value & 0x000000000000000CULL)
		{
		  pos += 2;
		  value = value >> 2;
		}
		if (value & 0x0000000000000002ULL)
		{
		  pos += 1;
		  value = value >> 1;
		}

		return pos;
	}

	std::size_t ceilLog2( std::uint32_t value )
	{
//		mem_assert( value > 0 );

		std::size_t pos = 0;
		if (value & (value - 1UL))
		{
		  pos = 1;
		}
		if (value & 0xFFFF0000UL)
		{
		  pos += 16;
		  value = value >> 16;
		}
		if (value & 0x0000FF00UL)
		{
		  pos += 8;
		  value = value >> 8;
		}
		if (value & 0x000000F0UL)
		{
		  pos += 4;
		  value = value >> 4;
		}
		if (value & 0x0000000CUL)
		{
		  pos += 2;
		  value = value >> 2;
		}
		if (value & 0x00000002UL)
		{
		  pos += 1;
		  value = value >> 1;
		}

		return pos;
	}
} } }

namespace mem
{
	static const std::size_t cDefaultGrowSize = 128;

	SmallBlocksAllocator::SmallBlocksAllocator( std::size_t low, std::size_t high,
												Allocator & srcAllocator, std::size_t * growSizes )
	:	_pSrcAllocator( & srcAllocator )
	,	_lowLog2( _impl::_smallblocks::ceilLog2( low ) )
	,	_hiLog2( _impl::_smallblocks::ceilLog2( high ) )
	,	_pHeads( 0 )
	{
		mem_assert( low <= high );
		mem_assert( _lowLog2 <= _hiLog2 );

		_pHeads = m_new_array( srcAllocator, ChainHead )[ _hiLog2 - _lowLog2 + 1 ];
		for( std::size_t sLog2 = _lowLog2; sLog2 <= _hiLog2; ++sLog2 )
		{
			_pHeads[ sLog2 - _lowLog2 ]._growSize = growSizes ? growSizes[ sLog2 - _lowLog2 ] : cDefaultGrowSize;
		}
	}

	SmallBlocksAllocator::~SmallBlocksAllocator( void )
	{
		SmallBlocksAllocator::reset();
		mem_assert( _pSrcAllocator != 0 );
		m_delete_array( *_pSrcAllocator ) _pHeads;
	}

	void * SmallBlocksAllocator::alloc( std::size_t size )
	{
		mem_assert( _pSrcAllocator != 0 );
		mem_assert( _lowLog2 <= _hiLog2 );

		const std::size_t sizeLog2 = _impl::_smallblocks::ceilLog2( size );
		const bool withinRange = ( _lowLog2 <= sizeLog2 ) && ( sizeLog2 <= _hiLog2 );
		if( withinRange )
		{
			BlockAllocator * pAllocator = fullestAllocator( sizeLog2 );
			if( !pAllocator )
			{
				pAllocator = addBlockAlloc( 1 << sizeLog2 );
			}
			return ( pAllocator ? pAllocator->alloc( size ) : 0 );
		}

		return _pSrcAllocator->alloc( size );
	}

	void SmallBlocksAllocator::dealloc( void * ptr )
	{
		for( std::size_t sLog2 = _lowLog2; sLog2 <= _hiLog2; ++sLog2 )
		{
			BlocksHeader * pHeader = _pHeads[ sLog2 - _lowLog2 ]._pHeader;
			for( ; pHeader; pHeader = pHeader->_pNext )
			{
				if( pHeader->_allocator.owns( ptr ) )
				{
					pHeader->_allocator.dealloc( ptr );
					// TODO FIX SD : if the allocator is empty, should we free this block?

					return;
				}
			}
		}

		// If we get here, it must be the source allocator's
		mem_assert( _pSrcAllocator != 0 );
		_pSrcAllocator->dealloc( ptr );
	}

	bool SmallBlocksAllocator::reset( void )
	{
		mem_assert( _pSrcAllocator != 0 );

		for( std::size_t sLog2 = _lowLog2; sLog2 <= _hiLog2; ++sLog2 )
		{
			BlocksHeader * pHeader = _pHeads[ sLog2 - _lowLog2 ]._pHeader;
			while( pHeader )
			{
				BlocksHeader * pNext = pHeader->_pNext;
				_pSrcAllocator->dealloc( pHeader );
				pHeader = pNext;
			}
		}

		return true;
	}

	BlockAllocator * SmallBlocksAllocator::fullestAllocator( std::size_t sizeLog2 )
	{
		BlockAllocator * pAllocator = 0;
		BlockAllocator * pFullestAllocator = 0;
		SmallBlocksAllocator::BlocksHeader * pHeader = _pHeads[ sizeLog2 - _lowLog2 ]._pHeader;
		for( ; pHeader; pHeader = pHeader->_pNext )
		{
			pAllocator = &( pHeader->_allocator );
			if( !pAllocator->full() &&
				( !pFullestAllocator || ( pAllocator->allocated() > pFullestAllocator->allocated() ) ) )
			{
				pFullestAllocator = pAllocator;
			}
		}
		return pFullestAllocator;
	}

	BlockAllocator * SmallBlocksAllocator::addBlockAlloc( std::size_t size )
	{
		mem_assert( _pSrcAllocator != 0 );
		const std::size_t sizeLog2 = _impl::_smallblocks::ceilLog2( size );
		mem_assert( ( _lowLog2 <= sizeLog2 ) && ( sizeLog2 <= _hiLog2 ) );
		const std::size_t headIndex = sizeLog2 - _lowLog2;
		const std::size_t bufferSize = sizeof( BlocksHeader ) + ( _pHeads[ headIndex ]._growSize * size );
		mbyte * pBuffer = static_cast< mbyte * >( _pSrcAllocator->alloc( bufferSize ) );
		if( pBuffer )
		{
			mbyte * pBufferEnd = pBuffer + bufferSize;
			BlocksHeader * pNewHeader = new( pBuffer ) BlocksHeader( pBuffer + sizeof( BlocksHeader ), pBufferEnd, size );
			pNewHeader->_pNext = _pHeads[ headIndex ]._pHeader;
			_pHeads[ headIndex ]._pHeader = pNewHeader;

			return &( pNewHeader->_allocator );
		}
		return 0;
	}
}
