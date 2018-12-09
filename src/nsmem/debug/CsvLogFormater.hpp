// This file is subject to the terms and conditions defined in the file 'LICENSE.txt'.

#ifndef NSMEM_DEFAULT_LOG_FORMATER_HPP
#define NSMEM_DEFAULT_LOG_FORMATER_HPP

#include "nsmem/debug/LogInfo.hpp"

namespace mem
{
	// SD TODO complete the csv formater
	struct CsvLogFormater
	{
		static const int ciMaxChars = 32;

		template< typename LogStream >
		void format( LogStream & stream, const LogInfo & logInfo )
		{
			switch( logInfo._event )
			{
				case Event::New:					stream << "m_new";			break;
				case Event::Delete:					stream << "m_delete";			break;
				case Event::NewArray:				stream << "m_new_array";		break;
				case Event::DeleteArray:			stream << "m_delete_array";	break;
				default:							stream << "unknown";			break;
			}
			stream << ", " << logInfo._filename << ", ";
			char strBuffer[ ciMaxChars ] = { 0 };
			snprintf( strBuffer, ciMaxChars, "%d", logInfo._line );
			stream << lineStr;
			if( logInfo._typename && logInfo._typename[0] )
			{
				stream << ", " << logInfo._typename;
			}
			stream << "\n";
		}
	};
}

#endif // NSMEM_DEFAULT_LOG_FORMATER_HPP
