// This file is subject to the terms and conditions defined in the file 'LICENSE.txt'.

#include <cstddef>
#include <limits>
#include <new>

namespace mem
{
	template < typename T, typename STATIC_ALLOC_TYPE >
	class StlAllocator
	{
	public:
		typedef T						value_type;
		typedef T *						pointer;
		typedef const T *				const_pointer;
		typedef T &						reference;
		typedef const T &				const_reference;
		typedef std::size_t				size_type;
		typedef std::ptrdiff_t			difference_type;
	public:
		template < typename U >
		struct rebind
		{
			typedef StlAllocator< U, STATIC_ALLOC_TYPE >	other;
		};
	public:
		StlAllocator( void ) throw()											{}
		StlAllocator( const StlAllocator & ) throw()							{}
		~StlAllocator( void ) throw()											{}
	public:
		template < typename U >
		StlAllocator ( const StlAllocator< U, STATIC_ALLOC_TYPE > & ) throw()	{}
	public:
		pointer			address( reference value ) const		{ return & value; }
		const_pointer	address( const_reference value ) const	{ return & value; }
		size_type		max_size( void ) const throw()
							{ return std::numeric_limits<std::size_t>::max() / sizeof(T); }
		pointer			allocate( size_type num, const void * = 0 )
							{
								mem_assert( STATIC_ALLOC_TYPE::pStaticAllocator );
								return static_cast< pointer >( STATIC_ALLOC_TYPE::pStaticAllocator->alloc( num * sizeof( T ) ) );
							}
		void			deallocate ( pointer p, size_type )
							{
								mem_assert( STATIC_ALLOC_TYPE::pStaticAllocator );
								STATIC_ALLOC_TYPE::pStaticAllocator->dealloc( p );
							}
		void			construct( pointer ptr, const T & from )
							{
								new( static_cast< void * >( ptr ) ) T( from );
							}
		void			destroy ( pointer ptr )
							{ ptr; ptr->~T(); }
	};

	template < typename T1, typename STATIC_ALLOC_TYPE1, typename T2, typename STATIC_ALLOC_TYPE2 >
	bool operator == (	const typename StlAllocator< T1, STATIC_ALLOC_TYPE1 > & lhs,
						const typename StlAllocator< T2, STATIC_ALLOC_TYPE2 > & rhs ) throw()
	{
		return STATIC_ALLOC_TYPE1::pStaticAllocator == STATIC_ALLOC_TYPE2::pStaticAllocator;
	}

	template < typename T1, typename STATIC_ALLOC_TYPE1, typename T2, typename STATIC_ALLOC_TYPE2 >
	bool operator != (	const typename StlAllocator< T1, STATIC_ALLOC_TYPE1 > & lhs,
						const typename StlAllocator< T2, STATIC_ALLOC_TYPE2 > & rhs ) throw()
	{
		return STATIC_ALLOC_TYPE1::pStaticAllocator != STATIC_ALLOC_TYPE2::pStaticAllocator;
	}
}
//-------------------------------------------------------------------------------------------------------------------
