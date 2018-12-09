// This file is subject to the terms and conditions defined in the file 'LICENSE.txt'.

#ifndef NSMEM_NULL_ALLOCATOR_HPP
#define NSMEM_NULL_ALLOCATOR_HPP

#include "nsmem/mem_defines.hpp"

namespace mem
{
	class NullAllocator
	{
	public: // Allocator
		void *	alloc( std::size_t )		{ return 0; }
		void	dealloc( void * )			{ }
		bool	reset( void )				{ return true; }
#ifdef NSMEM_DEBUG
		void *	allocDbg( std::size_t, LogInfo & )	{ return 0; }
		void	deallocDbg( void *, LogInfo & )		{ }
#endif
	};
}

#endif // NSMEM_NULL_ALLOCATOR_HPP
