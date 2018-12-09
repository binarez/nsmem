// This file is subject to the terms and conditions defined in the file 'LICENSE.txt'.

#ifndef NSMEM_DOUBLE_LIFO_ALLOCATOR_HPP
#define NSMEM_DOUBLE_LIFO_ALLOCATOR_HPP

#include "nsmem/LifoAllocator.hpp"
#include "nsmem/LifoBackAllocator.hpp"

namespace mem
{
	class DoubleLifoAllocator
	{
	public:
								DoubleLifoAllocator( void * pBegin, void * pEnd, std::size_t hardLimit = 0 )
#ifdef NSMEM_DEBUG
									: _bottom( pBegin, limitBottom( pBegin, pEnd, hardLimit ), _top )
									, _top( limitTop( pEnd, pBegin, hardLimit ), pEnd, _bottom )
#else
									: _bottom( pBegin, limitBottom( pBegin, pEnd, hardLimit ) )
									, _top( limitTop( pEnd, pBegin, hardLimit ), pEnd )
#endif
									{ }
	public:
		mem_inline LifoAllocator &		bottom()		{ return _bottom;	}
		mem_inline LifoBackAllocator &	top()			{ return _top;		}
		mem_inline bool					reset( void )	{ return _bottom.reset() && _top.reset(); }
	protected:
		mem_inline void *		limitBottom( void * pBegin, void * pEnd, std::size_t hardLimit )
									{ return ( hardLimit != 0 ) ? static_cast< mbyte* >( pBegin ) + hardLimit : pEnd; }
		mem_inline void *		limitTop( void * pBegin, void * pEnd, std::size_t hardLimit )
									{ return ( hardLimit != 0 ) ? static_cast< mbyte* >( pBegin ) - hardLimit : pEnd; }
#ifdef NSMEM_DEBUG
	protected:
		template < typename LIFO_TYPE, typename LINKED_LIFO_TYPE >
		class LinkedLifo : public LIFO_TYPE
		{
		public:
							LinkedLifo( void * pBegin, void * pEnd, LinkedLifo< LINKED_LIFO_TYPE, LIFO_TYPE > & other )
								:	LIFO_TYPE( pBegin, pEnd )
								,	_pOther( & other )
								{ }
		public:
			virtual void *	alloc( std::size_t size )
								{
									mem_assert( ( (_pBegin < _pEnd) && ( _pCurrent + size <= _pOther->cursor() ) ) || (_pBegin > _pEnd) );

									// TODO SD Buggy pcq overhead dans le backlifo?
									mem_assert( ( (_pBegin > _pEnd) && ( _pCurrent - size >= _pOther->cursor() ) ) || (_pBegin < _pEnd) );

									return LIFO_TYPE::alloc( size );
								}
		protected:
			LinkedLifo< LINKED_LIFO_TYPE, LIFO_TYPE > *	_pOther;
		};
	protected:
		LinkedLifo< LifoAllocator, LifoBackAllocator >		_bottom;
		LinkedLifo< LifoBackAllocator, LifoAllocator >		_top;
#else
		LifoAllocator					_bottom;
		LifoBackAllocator				_top;
#endif
	};
}

#endif // NSMEM_DOUBLE_LIFO_ALLOCATOR_HPP
