// This file is subject to the terms and conditions defined in the file 'LICENSE.txt'.

#ifndef NSMEM_DEFAULT_LOG_FORMATER_HPP
#define NSMEM_DEFAULT_LOG_FORMATER_HPP

#include "nsmem/debug/LogInfo.hpp"

#include <ctime>

namespace mem
{
	struct DefaultLogFormater
	{
		static const int ciMaxChars = 64;

		template< typename LogStream >
		void format( LogStream & stream, const LogInfo & logInfo )
		{
			char strBuffer[ ciMaxChars ] = { 0 };
			long long timestampMS = logInfo._time * 1000 / CLOCKS_PER_SEC;
			mem_snprintf( strBuffer, ciMaxChars, "%lld | ", timestampMS );
			stream << strBuffer;
			switch( logInfo._event )
			{
				case Event::New:					stream << "m_new ---------- ";	break;
				case Event::Delete:					stream << "m_delete ------- ";	break;
				case Event::NewArray:				stream << "m_new_array ---- ";	break;
				case Event::DeleteArray:			stream << "m_delete_array - ";	break;
				default:							stream << "unknown -------- ";	break;
			}
			mem_snprintf( strBuffer, ciMaxChars, "%d", logInfo._line );
			stream << logInfo._filename << "(" << strBuffer << ") " << logInfo._typename;
			if( logInfo._arraySize > 0 )
			{
				mem_snprintf( strBuffer, ciMaxChars, "%d", logInfo._arraySize );
				stream << "[" << strBuffer << "]";
			}
			if( logInfo._event == Event::New || logInfo._event == Event::NewArray )
			{
				stream << " = ";
				mem_snprintf( strBuffer, ciMaxChars, "%d", logInfo._numBytes );
				stream << strBuffer << " bytes";
			}
//			mem_snprintf( strBuffer, ciMaxChars, " @ %#.8x", logInfo._address );
			mem_snprintf(strBuffer, ciMaxChars, " @ %p", logInfo._address);
			stream << strBuffer << "\n";
		}
	};
}

#endif // NSMEM_DEFAULT_LOG_FORMATER_HPP
