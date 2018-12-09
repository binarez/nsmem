// This file is subject to the terms and conditions defined in the file 'LICENSE.txt'.

#ifndef NSMEM_MEM_SYSFAIL_HPP
#define NSMEM_MEM_SYSFAIL_HPP

#include "mem_defines.hpp"

/*--------------------------------------------------------------------------------------------------
| 8 global operators need to be rewritten for a compliant and portable result:
|  operator new( std::size_t )
|  operator new( std::size_t, const std::nothrow_t & )
|  operator new[]( std::size_t )
|  operator new[]( std::size_t, const std::nothrow_t & )
|  operator delete( void * )
|  operator delete( void *, const std::nothrow_t & )
|  operator delete[]( void * )
|  operator delete[]( void *, const std::nothrow_t & )
--------------------------------------------------------------------------------------------------*/

/*
// From here: new, delete, malloc, realloc, free are going to get overriden to assert
#define new
#define delete
#define malloc
#define free

void *	operator new ( std::size_t size )
			{  }
void *	operator new ( std::size_t size, const std::nothrow_t & )
			{  }

void	operator delete ( void * ptr )
			{  }
void	operator delete ( void * ptr, const std::nothrow_t & )
			{  }

void *	operator new [] ( std::size_t size )
			{  }
void *	operator new [] ( std::size_t size, const std::nothrow_t & )
			{  }

void	operator delete [] ( void * ptr )
			{  }
void	operator delete [] ( void * ptr, const std::nothrow_t & )
			{  }
*/

#endif // NSMEM_MEM_SYSFAIL_HPP
