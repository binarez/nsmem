// This file is subject to the terms and conditions defined in the file 'LICENSE.txt'.

#ifndef NSMEM_ALLOCATOR_HPP
#define NSMEM_ALLOCATOR_HPP

#include "nsmem/mem_defines.hpp"

#include <cstddef>		// std::size_t

namespace mem
{
	struct LogInfo;

	class Allocator
	{
	public:
		virtual					~Allocator( void )									{ }
	public:
		virtual void *			alloc( std::size_t size )							= 0;
		virtual void			dealloc( void * ptr )								= 0;
		virtual bool			reset( void )										= 0;
#ifdef NSMEM_DEBUG
		virtual void *			allocDbg( std::size_t size, LogInfo & /*logInfo*/ )	{ return this->alloc( size ); }
		virtual void			deallocDbg( void * ptr, LogInfo & /*logInfo*/ )		{ this->dealloc( ptr ); }
#endif
	};
}

#endif // NSMEM_ALLOCATOR_HPP
