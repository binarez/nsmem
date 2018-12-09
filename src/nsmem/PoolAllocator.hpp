// This file is subject to the terms and conditions defined in the file 'LICENSE.txt'.

#ifndef NSMEM_POOL_ALLOCATOR_HPP
#define NSMEM_POOL_ALLOCATOR_HPP

#include "nsmem/BlockAllocator.hpp"

namespace mem
{
	template < typename T >
	class PoolAllocator
	{
	public:
						PoolAllocator( mem::Allocator & allocator, size_t poolSize )
							: _pAllocator( & allocator )
							, _bytes( sizeof( T ) * poolSize )
							, _pBuffer( m_new_array( allocator, kult::byte ) [ _bytes ] )
							, _blockAllocator( _pBuffer, _pBuffer + _bytes, sizeof( T ) )
							{ }
						~PoolAllocator( void )
							{ m_delete_array( *_pAllocator ) _pBuffer; }
	public: // Allocator
		void *			alloc( std::size_t size )	{ return _blockAllocator.alloc( size ); }
		void			dealloc( void * ptr )		{ return _blockAllocator.dealloc( ptr ); }
		bool			reset( void )				{ return _blockAllocator.reset(); }
#ifdef NSMEM_DEBUG
		void *			allocDbg( std::size_t size, LogInfo & logInfo ) { return _blockAllocator.allocDbg( size, logInfo ); }
		void			deallocDbg( void * ptr, LogInfo & logInfo )		{ return _blockAllocator.deallocDbg( ptr, logInfo ); }
#endif
	protected:
		mem::Allocator *	_pAllocator;
		size_t				_bytes;
		mbyte *				_pBuffer;
		BlockAllocator		_blockAllocator;
	};
}

#endif // NSMEM_POOL_ALLOCATOR_HPP
