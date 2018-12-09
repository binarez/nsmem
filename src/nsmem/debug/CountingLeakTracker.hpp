// This file is subject to the terms and conditions defined in the file 'LICENSE.txt'.

#ifndef NSMEM_COUNTING_LEAK_TRACKER_HPP
#define NSMEM_COUNTING_LEAK_TRACKER_HPP

#include "nsmem/mem_defines.hpp"

#include <sstream>
#include <cassert>

namespace mem
{
	// A simple leak tracker: it counts allocations and deallocations.
	// It does that for single element and array allocations/deallocations.
	class CountingLeakTracker
	{
	public:
							CountingLeakTracker( void )
								: _numSingleAllocs( 0 )
								, _numSingleDeallocs( 0 )
								, _numArrayAllocs( 0 )
								, _numArrayDeallocs( 0 )
								{ }
							~CountingLeakTracker( void )
							{
								// Check leaks
								assert( _numSingleAllocs == _numSingleDeallocs );
								assert( _numArrayAllocs ==  _numArrayDeallocs );
							}
	public:	// DebugAllocator's TRACKER_T interface
		mem_inline void		onAlloc( void *, bool isArray )
								{
									if( isArray )
									{
										++_numArrayAllocs;
									}
									else
									{
										++_numSingleAllocs;
									}
								}
		mem_inline void		onDealloc( void *, bool isArray )
								{
									if( isArray )
									{
										++_numArrayDeallocs;
									}
									else
									{
										++_numSingleDeallocs;
									}
								}
		inline std::string	report( void )
								{
									std::ostringstream leakReport;
									leakReport << "Single element allocations   : " << _numSingleAllocs << std::endl;
									leakReport << "Single element deallocations : " << _numSingleDeallocs << std::endl;
									leakReport << "Single element leaks         : " << ( _numSingleAllocs - _numSingleDeallocs ) << std::endl;
									leakReport << "Array allocations   : " << _numArrayAllocs << std::endl;
									leakReport << "Array deallocations : " << _numArrayDeallocs << std::endl;
									leakReport << "Array leaks         : " << ( _numArrayAllocs - _numArrayDeallocs ) << std::endl;
									return leakReport.str();
								}
	protected:
		int		_numSingleAllocs;
		int		_numSingleDeallocs;
		int		_numArrayAllocs;
		int		_numArrayDeallocs;
	};
}

#endif // NSMEM_COUNTING_LEAK_TRACKER_HPP
