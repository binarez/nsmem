// This file is subject to the terms and conditions defined in the file 'LICENSE.txt'.

#ifndef NSMEM_SEQUENTIAL_BACK_ALLOCATOR_HPP
#define NSMEM_SEQUENTIAL_BACK_ALLOCATOR_HPP

#include "nsmem/mem_defines.hpp"

namespace mem
{
	class SequentialBackAllocator
	{
	public:
					SequentialBackAllocator( void * pEnd, void * pBegin )
						: _pBegin( static_cast< mbyte * >( pBegin ) )
						, _pEnd( static_cast< mbyte * >( pEnd ) )
						, _pCurrent( static_cast< mbyte * >( pBegin ) )
						{
							mem_assert( pBegin != 0 );
							mem_assert( pEnd != 0 );
							mem_assert( pBegin > pEnd );
						}
	public:	// Allocator
		bool		reset( void )
						{
							_pCurrent = _pBegin;
							return true;
						}
		void *		alloc( std::size_t size )
						{
							mem_assert( _pBegin > _pEnd );
							mem_assert( _pBegin >= _pCurrent && _pCurrent >= _pEnd );

							mbyte * pNewCurrent = _pCurrent - size;
							if( pNewCurrent >= _pEnd )
							{
								void * ptr = pNewCurrent;
								_pCurrent = pNewCurrent;
								return ptr;
							}

							// Failed alloc
							return 0;
						}
		void		dealloc( void * )
						{ /* no op */ }
#ifdef NSMEM_DEBUG
		void *		allocDbg( std::size_t size, LogInfo & /*logInfo*/ )
						{ return this->alloc( size ); }
		void		deallocDbg( void * ptr, LogInfo & /*logInfo*/ )
						{ this->dealloc( ptr ); }
#endif
	protected:
		mbyte *		_pBegin;
		mbyte *		_pEnd;
		mbyte *		_pCurrent;
	};
}

#endif // NSMEM_SEQUENTIAL_BACK_ALLOCATOR_HPP
