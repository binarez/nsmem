// This file is subject to the terms and conditions defined in the file 'LICENSE.txt'.

#ifndef NSMEM_LOGINFO_HPP
#define NSMEM_LOGINFO_HPP

#include <ctime>
#include <cstddef>		// std::size_t

namespace mem
{
	struct Event
	{
		enum Type
		{
			None			= 0,
			New,
			Delete,
			NewArray,
			DeleteArray
		};
	};

	struct LogInfo
	{
		LogInfo( Event::Type theEvent, const char * file, int line, const char* type )
			: _event( theEvent ), _filename( file ), _line( line ), _typename( type )
			, _address( 0 ), _numBytes( 0 ), _arraySize( 0 ), _time( 0 )
			{ }
		LogInfo( Event::Type theEvent, const char * file, int line, const char* type, void * address, std::size_t numBytes )
			: _event( theEvent ), _filename( file ), _line( line ), _typename( type )
			, _address( address ), _numBytes( numBytes ), _arraySize( 0 ), _time( 0 )
			{ }
		std::clock_t	_time;
		const char *	_filename;
		const char *	_typename;
		void *			_address;
		std::size_t		_numBytes;
		int				_line;
		int				_arraySize;
		Event::Type		_event;
	};
}


#endif // NSMEM_LOGINFO_HPP
