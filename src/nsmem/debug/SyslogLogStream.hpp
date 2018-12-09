// This file is subject to the terms and conditions defined in the file 'LICENSE.txt'.

#ifndef NSMEM_SYSLOG_LOG_STREAM_HPP
#define NSMEM_SYSLOG_LOG_STREAM_HPP

#include <syslog.h>

namespace mem
{
	template< int SYSLOG_PRIORITY = LOG_DEBUG >
	class SyslogLogStream
	{
	public:
		SyslogLogStream( void )					{ openlog( "nsmem", 0, LOG_USER ); }
		~SyslogLogStream( void )				{ closelog(); }
	public:
		SyslogLogStream & operator << ( const char * str )
		{
			syslog( SYSLOG_PRIORITY, str );
			return *this;
		}
	};
}

#endif // NSMEM_SYSLOG_LOG_STREAM_HPP
