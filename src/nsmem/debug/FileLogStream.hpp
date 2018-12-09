// Copyright (c) 2012-2013, Stephane Duguay.  All Rights Reserved.

#ifndef NSMEM_STDIO_LOG_STREAM_HPP
#define NSMEM_STDIO_LOG_STREAM_HPP

#include "nsmem/mem_defines.hpp"

#include <cstdio>

namespace mem
{
	class FileLogStream
	{
	public:
		FileLogStream( const char * filename )
			: _pFile( fopen( filename, "w" ) )
			{ mem_assert( _pFile ); }
		~FileLogStream( void )
			{
				if( _pFile )
					{ fclose( _pFile ); }
			}
	public:
		FileLogStream & operator << ( const char * str )
		{
			if( _pFile )
				{ fprintf( _pFile, str ); }
			return *this;
		}
	private:
		FILE *	_pFile;
	};
}

#endif // NSMEM_STDIO_LOG_STREAM_HPP
