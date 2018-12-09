// This file is subject to the terms and conditions defined in the file 'LICENSE.txt'.

#ifndef NSMEM_CIRCULAR_ALLOCATOR_HPP
#define NSMEM_CIRCULAR_ALLOCATOR_HPP

#include "nsmem/mem_defines.hpp"

namespace mem
{
	class CircularAllocator
	{
	public:
						CircularAllocator( void * pBegin, void * pEnd );
	public:	// Allocator
		bool			reset( void )
							{
								_pHead = static_cast< mbyte * >( _pBegin );
								_pTail = static_cast< mbyte * >( _pBegin );
								return true;
							}
		void *			alloc( std::size_t size );
		void			dealloc( void * );
#ifdef NSMEM_DEBUG
		void *			allocDbg( std::size_t size, LogInfo & /*logInfo*/ )
							{ return this->alloc( size ); }
		void			deallocDbg( void * ptr, LogInfo & /*logInfo*/ )
							{ this->dealloc( ptr ); }
#endif
	protected:
		mbyte *			_pBegin;
		mbyte *			_pEnd;
		mbyte *			_pHead; 
		mbyte *			_pTail;
	};
}

#endif // NSMEM_CIRCULAR_ALLOCATOR_HPP
