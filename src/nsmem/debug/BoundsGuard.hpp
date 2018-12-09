// This file is subject to the terms and conditions defined in the file 'LICENSE.txt'.

#ifndef NSMEM_BOUNDS_GUARD_HPP
#define NSMEM_BOUNDS_GUARD_HPP

#include "nsmem/mem_defines.hpp"

namespace mem
{
	//--------------------------------------------------------------------------------------------------------------
	template< std::size_t GUARD_SIZE, mbyte GUARD_VALUE = 0xA5 >
	class BoundsGuard
	{
	public:
		static const std::size_t	cNumBytes				= GUARD_SIZE;
	public:
		mem_inline void				stamp( void * address )
										{
											_impl::Browser browser;
											browser.address = address;
											for( std::size_t i = 0; i < cNumBytes; ++i )
												{ *(browser.pByte++) = cGuardValue; }
										}
		mem_inline bool				check( void * address )
										{
											_impl::Browser browser;
											browser.address = address;
											for( std::size_t i = 0; i < cNumBytes; ++i )
											{
												if( *(browser.pByte++) != cGuardValue )
													{ return false; }
											}
											return true;
										}
	protected:
		static const mbyte			cGuardValue = GUARD_VALUE;
	};
	//--------------------------------------------------------------------------------------------------------------
	template< mbyte GUARD_VALUE >
	class BoundsGuard< 4, GUARD_VALUE >
	{
	public:
		static const std::size_t	cNumBytes				= 4;
	public:
		mem_inline void				stamp( void * address )
										{
											std::uint32_t * p32 = static_cast< std::uint32_t * >( address );
											*p32 = cGuardValue32;
										}
		mem_inline bool				check( void * address )
										{
											std::uint32_t * p32 = static_cast< std::uint32_t * >( address );
											return ( *p32 == cGuardValue32 );
										}
	protected:
		static const std::uint32_t	cGuardValue32 = std::uint32_t(GUARD_VALUE) |
														(std::uint32_t(GUARD_VALUE) << 8) |
															(std::uint32_t(GUARD_VALUE) << 16) |
																(std::uint32_t(GUARD_VALUE) << 24);
	};
	//--------------------------------------------------------------------------------------------------------------
	template< mbyte GUARD_VALUE >
	class BoundsGuard< 8, GUARD_VALUE >
	{
	public:
		static const std::size_t	cNumBytes				= 8;
	public:
		mem_inline void				stamp( void * address )
										{
											std::uint64_t * p64 = static_cast< std::uint64_t * >( address );
											*p64 = cGuardValue64;
										}
		mem_inline bool				check( void * address )
										{
											std::uint64_t * p64 = static_cast< std::uint64_t * >( address );
											return ( *p64 == cGuardValue64 );
										}
	protected:
		static const std::uint64_t	cGuardValue64 = std::uint64_t(GUARD_VALUE) |
														(std::uint64_t(GUARD_VALUE) << 8) |
															(std::uint64_t(GUARD_VALUE) << 16) |
																(std::uint64_t(GUARD_VALUE) << 24) |
																	(std::uint64_t(GUARD_VALUE) << 32) |
																		(std::uint64_t(GUARD_VALUE) << 40) |
																			(std::uint64_t(GUARD_VALUE) << 48) |
																				(std::uint64_t(GUARD_VALUE) << 56);
	};
}

#endif // NSMEM_BOUNDS_GUARD_HPP
