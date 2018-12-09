// This file is subject to the terms and conditions defined in the file 'LICENSE.txt'.

#ifndef NSMEM_BLOCK_ALLOCATOR_HPP
#define NSMEM_BLOCK_ALLOCATOR_HPP

#include "nsmem/mem_defines.hpp"

namespace mem
{
	class BlockAllocator
	{
	public:
						BlockAllocator( void * pBegin, void * pEnd, std::size_t blockSize )
							:	_pBegin( static_cast< mbyte * >( pBegin ) )
							,	_pEnd( static_cast< mbyte * >( pEnd ) )
							,	_blockSize( blockSize > sizeof( PoolEntry ) ? blockSize : sizeof( PoolEntry ) )
							,	_numBlocks( ( _pEnd - _pBegin ) / _blockSize )
							,	_allocated( 0 )
							,	_freeList( 0 )
							{
								mem_assert( _pBegin != 0 );
								mem_assert( _pEnd != 0 );
								mem_assert( _pBegin < _pEnd );
								mem_assert( _blockSize > 0 );
								mem_assert( _numBlocks > 0 );

								BlockAllocator::reset();
							}
						~BlockAllocator( void )
							{
							}
	public:
		bool			full( void ) const
							{ return ( _freeList == 0 ); }
		bool			empty( void ) const
							{ return ( _allocated == 0 ); }
		std::size_t		allocated( void ) const
							{ return _allocated; }
		std::size_t		capacity( void ) const
							{ return _numBlocks; }
		bool			owns( void * ptr ) const
							{
								mem_assert( _pBegin != 0 );
								mem_assert( _pEnd != 0 );
								mem_assert( _pBegin < _pEnd );

								return ( _pBegin <= ptr ) && ( ptr < _pEnd );
							}
	public: // Allocator
		void *			alloc( std::size_t size )
							{
								mem_assert( _pBegin != 0 );
								mem_assert( _pEnd != 0 );
								mem_assert( _pBegin < _pEnd );
								mem_assert( _blockSize > 0 );
								mem_assert( _numBlocks > 0 );
								mem_assert( _allocated <= _numBlocks );

								mem_assert( size <= _blockSize );
								if( size <= _blockSize && _freeList != 0 )
								{
									void * pBlock = _freeList;
									_freeList = _freeList->_pNext;
									++_allocated;
									return pBlock;
								}

								// Failed alloc
								return 0;
							}
		void			dealloc( void * ptr )
							{
								mem_assert( _pBegin != 0 );
								mem_assert( _pEnd != 0 );
								mem_assert( _pBegin < _pEnd );
								mem_assert( _blockSize > 0 );
								mem_assert( _numBlocks > 0 );
								mem_assert( _allocated <= _numBlocks );

								PoolEntry * pEntry = reinterpret_cast< PoolEntry * >( ptr );
								pEntry->_pNext = _freeList;
								_freeList = pEntry;
								--_allocated;
							}
		bool			reset( void )
							{
								mem_assert( _pBegin != 0 );
								mem_assert( _pEnd != 0 );
								mem_assert( _pBegin < _pEnd );
								mem_assert( _blockSize > 0 );
								mem_assert( _numBlocks > 0 );
								mem_assert( _allocated <= _numBlocks );

								_freeList = 0;
								_allocated = 0;
								if( _numBlocks > 0 )
								{
									PoolEntry * freeListPtr = reinterpret_cast< PoolEntry * >( _pBegin );
									for( mbyte * pBlockBegin = _pBegin, * pBlockEnd = _pBegin + _blockSize;
										 pBlockEnd <= (_pEnd - _blockSize);
										 pBlockBegin += _blockSize, pBlockEnd += _blockSize	)
									{
										freeListPtr->_pNext = reinterpret_cast< PoolEntry * >( pBlockEnd );
										freeListPtr = freeListPtr->_pNext;
									}
									freeListPtr->_pNext = 0;
									_freeList = reinterpret_cast< PoolEntry * >( _pBegin );
								}

								return true;
							}
#ifdef NSMEM_DEBUG
		void *			allocDbg( std::size_t size, LogInfo & /*logInfo*/ )
							{ return this->alloc( size ); }
		void			deallocDbg( void * ptr, LogInfo & /*logInfo*/ )
							{ this->dealloc( ptr ); }
#endif
	protected:
		struct PoolEntry
		{
			PoolEntry *	_pNext;
		};
	protected:
		mbyte *			_pBegin;
		mbyte *			_pEnd;
		msize			_blockSize;
		msize			_numBlocks;
		msize			_allocated;
		PoolEntry *		_freeList;
	};
}

#endif // NSMEM_BLOCK_ALLOCATOR_HPP
