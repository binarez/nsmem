// This file is subject to the terms and conditions defined in the file 'LICENSE.txt'.

#ifndef NSMEM_MALLOC_ALLOCATOR_HPP
#define NSMEM_MALLOC_ALLOCATOR_HPP

#include "nsmem/mem_defines.hpp"

#include <cstdlib>

namespace mem
{
	class MallocAllocator
	{
	public:
					MallocAllocator( void )				{ }
					~MallocAllocator( void )			{ }
	public: // Allocator
		void *		alloc( std::size_t size )			{ return std::malloc( size ); }
		void		dealloc( void * ptr )				{ std::free( ptr ); }
		bool		reset( void )						{ return false; }	// Can I reset malloc?
#ifdef NSMEM_DEBUG
		void *		allocDbg( std::size_t size, LogInfo & /*logInfo*/ )	{ return this->alloc( size ); }
		void		deallocDbg( void * ptr, LogInfo & /*logInfo*/ )		{ this->dealloc( ptr ); }
#endif
	};
}

#endif // NSMEM_MALLOC_ALLOCATOR_HPP
