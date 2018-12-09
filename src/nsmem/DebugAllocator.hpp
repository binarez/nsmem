// This file is subject to the terms and conditions defined in the file 'LICENSE.txt'.

#ifndef NSMEM_DEBUG_ALLOCATOR_HPP
#define NSMEM_DEBUG_ALLOCATOR_HPP

#include "nsmem/debug/LogInfo.hpp"
#include "nsmem/debug/DefaultLogFormater.hpp"
#include "nsmem/debug/NoBoundsCheck.hpp"
#include "nsmem/debug/CountingLeakTracker.hpp"

#include <ostream>
#include <ctime>

namespace mem
{

	template <	typename ALLOCATOR_T,
				typename TRACKER_T = CountingLeakTracker,
				typename LOG_FORMATER_T = DefaultLogFormater,
				// By default an std::ostream so you can easily pass in std::cout, std::clog,
				// std::cerr or an instance of std::ofstream.
				typename LOG_STREAM_T = std::ostream,
				typename FRONT_GUARD_T = NoBoundsCheck,
				typename BACK_GUARD_T = FRONT_GUARD_T >
	class DebugAllocator
	{
	public:
						DebugAllocator( ALLOCATOR_T & memAllocator, LOG_STREAM_T & stream )
							: _pStream( & stream )
							, _pMemAllocator( & memAllocator )
							, _startTime( std::clock() )
							{
								std::time_t now = std::time( 0 );
								(*_pStream) << "Begin of nsmem log: ";
								(*_pStream) << std::asctime( std::localtime( & now  ) ) << "\n";
							}
						~DebugAllocator( void )
							{
								std::time_t now = std::time( 0 );
								(*_pStream) << "\n" << "End of nsmem log: ";
								(*_pStream) << std::asctime( std::localtime( & now  ) ) << "\n";

								(*_pStream) << "------------------------------------------\n";
								(*_pStream) << _tracker.report().c_str();
								(*_pStream) << "------------------------------------------\n";
							}
	public:
		bool			reset( void )
							{
								mem_assert( _pMemAllocator );
								return _pMemAllocator->reset();
							}
		void *			alloc( std::size_t size )						{ return _pMemAllocator->alloc( size ); }
		void			dealloc( void * ptr )							{ _pMemAllocator->dealloc( ptr ); }
		void *			allocDbg( std::size_t size, LogInfo & logInfo );
		void			deallocDbg( void * ptr, LogInfo & logInfo );
	protected:
		FRONT_GUARD_T	_frontGuard;
		BACK_GUARD_T	_backGuard;
		TRACKER_T		_tracker;
		LOG_FORMATER_T	_formater;
		LOG_STREAM_T *	_pStream;
		ALLOCATOR_T *	_pMemAllocator;
		std::clock_t	_startTime;
	};

	template < typename ALLOCATOR_T, typename TRACKER_T, typename LOG_FORMATER_T, typename LOG_STREAM_T, typename FRONT_GUARD_T, typename BACK_GUARD_T >
	void * DebugAllocator< ALLOCATOR_T, TRACKER_T, LOG_FORMATER_T, LOG_STREAM_T, FRONT_GUARD_T, BACK_GUARD_T  >::allocDbg( std::size_t size, LogInfo & logInfo )
	{
		logInfo._time = std::clock() - _startTime;

		const std::size_t totalSize = size + sizeof( msize ) + _frontGuard.cNumBytes + _frontGuard.cNumBytes;
		_impl::Browser browser;
		browser.address = _pMemAllocator->alloc( totalSize );
		*(browser.pSize++) = size;

		// Write guards
		_frontGuard.stamp( browser.address );
		browser.pByte += _frontGuard.cNumBytes;
		void * ptr = browser.address;
		browser.pByte += size;
		_backGuard.stamp( browser.address );

		logInfo._address = ptr;
		logInfo._numBytes = size;
		_formater.format( * _pStream, logInfo );

		_tracker.onAlloc( ptr, logInfo._arraySize > 0 );
		return ptr;
	}

	template < typename ALLOCATOR_T, typename TRACKER_T, typename LOG_FORMATER_T, typename LOG_STREAM_T, typename FRONT_GUARD_T, typename BACK_GUARD_T >
	void DebugAllocator< ALLOCATOR_T, TRACKER_T, LOG_FORMATER_T, LOG_STREAM_T, FRONT_GUARD_T, BACK_GUARD_T >::deallocDbg( void * ptr, LogInfo & logInfo )
	{
		_tracker.onDealloc( ptr, logInfo._arraySize > 0 );

		logInfo._time = std::clock() - _startTime;
		logInfo._address = ptr;
		_formater.format( * _pStream, logInfo );

		_impl::Browser browser;
		browser.address = ptr;
		browser.pByte -= sizeof( msize ) + _frontGuard.cNumBytes;
		ptr = browser.address;
		const std::size_t size = *(browser.pSize++);

		const bool goodFront = _frontGuard.check( browser.address );
		mem_assert( goodFront );	// TODO FIX SD, better reporting.

		browser.pByte += _frontGuard.cNumBytes + size;
		const bool goodBack = _backGuard.check( browser.address );
		mem_assert( goodBack );		// TODO FIX SD, better reporting.

		_pMemAllocator->dealloc( ptr );
	}
}

#endif // NSMEM_DEBUG_ALLOCATOR_HPP
