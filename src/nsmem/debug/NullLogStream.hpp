// This file is subject to the terms and conditions defined in the file 'LICENSE.txt'.

#ifndef NSMEM_NULL_LOG_STREAM_HPP
#define NSMEM_NULL_LOG_STREAM_HPP

namespace mem
{
	class NullLogStream
	{
	public:
		NullLogStream & operator << ( const char * )
		{
			return *this;
		}
	};
}

#endif // NSMEM_NULL_LOG_STREAM_HPP
