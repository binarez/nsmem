// This file is subject to the terms and conditions defined in the file 'LICENSE.txt'.

#ifndef NSMEM_WIN32_LOG_STREAM_HPP
#define NSMEM_WIN32_LOG_STREAM_HPP

#define NOMINMAX
#include <Windows.h>
#undef NOMINMAX

namespace mem
{
	class Win32LogStream
	{
	public:
		Win32LogStream & operator << ( const char * str )
		{
			OutputDebugString( str );
			return *this;
		}
	};
}

#endif // NSMEM_WIN32_LOG_STREAM_HPP
