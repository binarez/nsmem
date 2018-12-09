// This file is subject to the terms and conditions defined in the file 'LICENSE.txt'.

#ifndef NSMEM_DL_HEAP_ALLOCATOR_HPP
#define NSMEM_DL_HEAP_ALLOCATOR_HPP

#include "nsmem/mem_defines.hpp"

namespace mem
{
	class DLHeapAllocator
	{
	public:
						DLHeapAllocator( std::size_t maxBytes = 0 );	// 0 = no limit
						~DLHeapAllocator( void );
	public: // Allocator
		bool			reset( void );
		void *			alloc( std::size_t size );
		void			dealloc( void * ptr );
#ifdef NSMEM_DEBUG
		void *			allocDbg( std::size_t size, LogInfo & /*logInfo*/ )
							{ return this->alloc( size ); }
		void			deallocDbg( void * ptr, LogInfo & /*logInfo*/ )
							{ this->dealloc( ptr ); }
#endif
	};
}

#endif // NSMEM_DL_HEAP_ALLOCATOR_HPP
