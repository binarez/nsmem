// This file is subject to the terms and conditions defined in the file 'LICENSE.txt'.

#ifndef NSMEM_SMALL_BLOCKS_ALLOCATOR_HPP
#define NSMEM_SMALL_BLOCKS_ALLOCATOR_HPP

#include "nsmem/Allocator.hpp"
#include "nsmem/BlockAllocator.hpp"

namespace mem
{
	class SmallBlocksAllocator
	{
	public:
					SmallBlocksAllocator(	std::size_t low, std::size_t high,
											Allocator & _srcAllocator, std::size_t * growSizes	);
					~SmallBlocksAllocator( void );
	public: // Allocator
		void *		alloc( std::size_t size );
		void		dealloc( void * ptr );
		bool		reset( void );
#ifdef NSMEM_DEBUG
		void *		allocDbg( std::size_t size, LogInfo & /*logInfo*/ )
						{ return this->alloc( size ); }
		void		deallocDbg( void * ptr, LogInfo & /*logInfo*/ )
						{ this->dealloc( ptr ); }
#endif
	protected:
		struct BlocksHeader
		{
			BlocksHeader( void * pBegin, void * pEnd, std::size_t blockSize )
				: _pNext( 0 )
				, _allocator( pBegin, pEnd, blockSize )
				{ }
			BlocksHeader *	_pNext;
			BlockAllocator	_allocator;
		};
	protected:
		struct ChainHead
		{
			ChainHead( void )
				: _pHeader( 0 )
				, _growSize( 0 )
				{ }
			BlocksHeader *	_pHeader;
			std::size_t		_growSize;
		};
	protected:
		BlockAllocator *	fullestAllocator( std::size_t sizeLog2 );
		BlockAllocator *	addBlockAlloc( std::size_t sizeLog2 );
	protected:
		Allocator *			_pSrcAllocator;
		std::size_t			_lowLog2;
		std::size_t			_hiLog2;
		ChainHead *			_pHeads;
	};
}

#endif // NSMEM_SMALL_BLOCKS_ALLOCATOR_HPP
