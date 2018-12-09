// This file is subject to the terms and conditions defined in the file 'LICENSE.txt'.

#ifndef NSMEM_MEM_DEFINES_HPP
#define NSMEM_MEM_DEFINES_HPP

#include <cstddef>							// std::size_t
#include <cstdint>							// std::int32_t

#ifndef mem_inline
# ifdef _MSC_VER
#  define	mem_inline						__forceinline
# else
#  define	mem_inline						inline
# endif
#endif

#ifndef mem_snprintf
# ifdef _MSC_VER
#  define	mem_snprintf					std::snprintf
# else
#  define	mem_snprintf					std::snprintf
# endif
#endif

#ifndef mem_assert
#include <cassert>
# define	mem_assert( TEST )				assert( TEST )
#endif

#ifndef mem_decltype
# define	mem_decltype( EXPR )			decltype( EXPR )
#endif

namespace mem
{
	typedef			unsigned char			mbyte;
	typedef			std::uint32_t			msize;

	namespace _impl
	{
		union Browser
		{
			void *		address;
			mbyte *		pByte;
			msize *		pSize;
		};
	}

	mem_inline std::size_t		GB( double size )		{ return static_cast< std::size_t >( (size * 1024.0 * 1024.0 * 1024.0 ) + 0.5 ); }
	mem_inline std::size_t		MB( double size )		{ return static_cast< std::size_t >( (size * 1024.0 * 1024.0 ) + 0.5 ); }
	mem_inline std::size_t		KB( double size )		{ return static_cast< std::size_t >( (size * 1024.0 ) + 0.5 ); }
	mem_inline std::size_t		 B( std::size_t size )	{ return size; }

	struct LogInfo;
}

#endif // NSMEM_MEM_DEFINES_HPP
