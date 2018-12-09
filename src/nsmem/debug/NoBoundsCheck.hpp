// This file is subject to the terms and conditions defined in the file 'LICENSE.txt'.

#ifndef NSMEM_NO_BOUNDS_CHECKING_HPP
#define NSMEM_NO_BOUNDS_CHECKING_HPP

#include "nsmem/mem_defines.hpp"

namespace mem
{
	class NoBoundsCheck
	{
	public:
		static const std::size_t	cNumBytes			= 0;
	public:
		mem_inline void				stamp( void * )		{}
		mem_inline bool				check( void * )		{ return true; }
	};
}

#endif // NSMEM_NO_BOUNDS_CHECKING_HPP
