// This file is subject to the terms and conditions defined in the file 'LICENSE.txt'.

#ifndef NSMEM_MEM_IMPL_H
#define NSMEM_MEM_IMPL_H

#include "nsmem/mem_defines.hpp"
#include "nsmem/Allocator.hpp"

#ifdef NSMEM_DEBUG
#include "nsmem/debug/LogInfo.hpp"
#endif

#include <cstdio>
#include <type_traits>

namespace mem
{
#ifdef NSMEM_DEBUG
  namespace _impldbg
#else
  namespace _impl
#endif
  {
	//--------------------------------------------------------------------------------------------------------------
	template < typename T >
	mem_inline bool isPOD( void )
	{
		return std::is_pod< T >::value;
	};
	//--------------------------------------------------------------------------------------------------------------
	template< typename ALLOCTYPE, typename T >
	class ArrayCreator
	{
	public:
#ifdef NSMEM_DEBUG
		ArrayCreator( ALLOCTYPE & allocator, const LogInfo & logInfo )
			: _allocator( allocator )
			, _logInfo( logInfo )
			{ }
#else
		ArrayCreator( ALLOCTYPE & allocator )
			: _allocator( allocator )
			{ }
#endif
	public:
		mem_inline T * operator [] ( std::size_t howMany )
		{
			void * address = 0;

#ifdef NSMEM_DEBUG
			_logInfo._arraySize = howMany;
			address = _allocator.allocDbg( sizeof( std::size_t ) + ( sizeof( T ) * howMany ), _logInfo );
#else
			address = _allocator.alloc( sizeof( std::size_t ) + ( sizeof( T ) * howMany ) );
#endif
			union
			{
				void *		ptr;
				size_t *	pSize;
				T *			pT;
			};

			ptr = address;
			*(pSize++) = howMany;
			T * pArray = pT;
			for( std::size_t i = 0; i < howMany; ++i, ++pT )
			{
				new( pT ) T;
			}

			return pArray;
		}
	private:
		ArrayCreator &		operator = ( const ArrayCreator & rhs );
	private:
		ALLOCTYPE &	_allocator;
#ifdef NSMEM_DEBUG
		LogInfo		_logInfo;
#endif
	};
	//---------------------------------------------------------------------------------------------------------------
	template< typename ALLOCTYPE >
	class Destroyer
	{
	public:
#ifdef NSMEM_DEBUG
		Destroyer( ALLOCTYPE & allocator, const LogInfo & logInfo )
			: _allocator( allocator )
			, _logInfo( logInfo )
			{ }
#else
		Destroyer( ALLOCTYPE & allocator )
			: _allocator( allocator )
			{ }
#endif
	public:
		template< typename T >
		mem_inline void operator - ( T * pObjToDelete )
		{
			if( pObjToDelete )
			{
				if( !isPOD< T >() )
				{
					pObjToDelete->~T();
				}
#ifdef NSMEM_DEBUG
				_allocator.deallocDbg( pObjToDelete, _logInfo );
#else
				_allocator.dealloc( pObjToDelete );
#endif
			}
		}
	private:
		Destroyer &	operator = ( const Destroyer & rhs );
	private:
		ALLOCTYPE &	_allocator;
#ifdef NSMEM_DEBUG
		LogInfo		_logInfo;
#endif
	};
	//---------------------------------------------------------------------------------------------------------------
	template< typename ALLOCTYPE >
	class ArrayDestroyer
	{
	public:
#ifdef NSMEM_DEBUG
		ArrayDestroyer( ALLOCTYPE & allocator, const LogInfo & logInfo )
			: _allocator( allocator )
			, _logInfo( logInfo )
			{ }
#else
		ArrayDestroyer( ALLOCTYPE & allocator )
			: _allocator( allocator )
			{ }
#endif
	public:
		template< typename T >
		mem_inline void operator - ( T * pObjsToDelete )
		{
			if( pObjsToDelete )
			{
				union
				{
					void *		ptr;
					size_t *	pSize;
				};
				ptr = pObjsToDelete;
				const std::size_t instances = *(pSize - 1);
#ifdef NSMEM_DEBUG
				_logInfo._arraySize = instances;
#endif
				if( !isPOD< T >() )
				{
					for( int i = instances - 1; i >= 0; --i )
					{
						(pObjsToDelete + i )->~T();
					}
				}
				--pSize;
#ifdef NSMEM_DEBUG
				_allocator.deallocDbg( ptr, _logInfo );
#else
				_allocator.dealloc( ptr );
#endif
			}
		}
	private:
		ArrayDestroyer &	operator = ( const ArrayDestroyer & rhs );
	private:
		ALLOCTYPE &	_allocator;
#ifdef NSMEM_DEBUG
		LogInfo 	_logInfo;
#endif
	}; 
	//---------------------------------------------------------------------------------------------------------------
#ifdef NSMEM_DEBUG
	template < typename T >
	mem_inline T & debugTempRef( const T & constRef )
	{
		return const_cast< T & >( constRef );	// Yeah, I know... but only in debug!
	};
	//---------------------------------------------------------------------------------------------------------------
#endif
} }

#endif // NSMEM_MEM_IMPL_H
