// Copyright (c) 2012-2013, Stephane Duguay.  All Rights Reserved.

#ifndef NSMEM_PRINTF_LOG_STREAM_HPP
#define NSMEM_PRINTF_LOG_STREAM_HPP

#include <cstdio>

namespace mem
{
	class PrintfLogStream
	{
	public:
		PrintfLogStream & operator << ( const char * str )
		{
			printf( str );
			return *this;
		}
	};
}

#endif // NSMEM_PRINTF_LOG_STREAM_HPP
