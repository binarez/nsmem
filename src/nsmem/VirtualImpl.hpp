 // This file is subject to the terms and conditions defined in the file 'LICENSE.txt'.

#ifndef NSMEM_VIRTUAL_IMPL_HPP
#define NSMEM_VIRTUAL_IMPL_HPP

#include "nsmem/Allocator.hpp"

namespace mem
{
	template< typename T >
	class VirtualImpl : public Allocator
	{
	public:
			template<typename... Args>
			VirtualImpl( Args&&... args )
				: _allocator( std::forward<Args>(args)... ) { }

			/*
			VirtualImpl( void )
				: _allocator() { }
			 

			 template< typename P1 >
			VirtualImpl( P1 & p1 )
				: _allocator( p1 ) { }

			template< typename P1, typename P2 >
			VirtualImpl( P1 & p1, P2 & p2 )
				: _allocator( p1, p2 ) { }

			template< typename P1, typename P2, typename P3 >
			VirtualImpl( P1 & p1, P2 & p2, P3 & p3 )
				: _allocator( p1, p2, p3 ) { }

			template< typename P1, typename P2, typename P3, typename P4 >
			VirtualImpl( P1 & p1, P2 & p2, P3 & p3, P4 & p4 )
				: _allocator( p1, p2, p3, p4 ) { }

			template< typename P1, typename P2, typename P3, typename P4, typename P5 >
			VirtualImpl( P1 & p1, P2 & p2, P3 & p3, P4 & p4, P5 & p5 )
				: _allocator( p1, p2, p3, p4, p5 ) { }

			template< typename P1, typename P2, typename P3, typename P4, typename P5, typename P6 >
			VirtualImpl( P1 & p1, P2 & p2, P3 & p3, P4 & p4, P5 & p5, P6 & p6 )
				: _allocator( p1, p2, p3, p4, p5, p6 ) { }

			template< typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7 >
			VirtualImpl( P1 & p1, P2 & p2, P3 & p3, P4 & p4, P5 & p5, P6 & p6, P7 & p7 )
				: _allocator( p1, p2, p3, p4, p5, p6, p7 ) { }

			template< typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8 >
			VirtualImpl( P1 & p1, P2 & p2, P3 & p3, P4 & p4, P5 & p5, P6 & p6, P7 & p7, P8 & p8 )
				: _allocator( p1, p2, p3, p4, p5, p6, p7, p8 ) { }

			template< typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9 >
			VirtualImpl( P1 & p1, P2 & p2, P3 & p3, P4 & p4, P5 & p5, P6 & p6, P7 & p7, P8 & p8, P9 & p9 )
				: _allocator( p1, p2, p3, p4, p5, p6, p7, p8, p9 ) { }

			template< typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10 >
			VirtualImpl( P1 & p1, P2 & p2, P3 & p3, P4 & p4, P5 & p5, P6 & p6, P7 & p7, P8 & p8, P9 & p9, P10 & p10 )
				: _allocator( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 ) { }
			*/
	public:
		virtual				~VirtualImpl( void )			{ }
	public:
		T &					impl( void )				{ return _allocator; }
		const T &			impl( void ) const			{ return _allocator; }
	public: // Allocator
		virtual void *		alloc( std::size_t size )	{ return _allocator.alloc( size ); }
		virtual void		dealloc( void * ptr )		{ _allocator.dealloc( ptr ); }
		virtual bool		reset( void )				{ return _allocator.reset(); }
#ifdef NSMEM_DEBUG
		virtual void *		allocDbg( std::size_t size, LogInfo & logInfo )	{ return _allocator.allocDbg( size, logInfo ); }
		virtual void		deallocDbg( void * ptr, LogInfo & logInfo )		{ _allocator.deallocDbg( ptr, logInfo ); }
#endif
	protected:
		T					_allocator;
	};
}

#endif // NSMEM_VIRTUAL_IMPL_HPP
