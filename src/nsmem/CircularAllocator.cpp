// This file is subject to the terms and conditions defined in the file 'LICENSE.txt'.

#include "CircularAllocator.hpp"

namespace mem
{
	CircularAllocator::CircularAllocator( void * pBegin, void * pEnd )
	: _pBegin( static_cast< mbyte * >( pBegin ) )
	, _pEnd( static_cast< mbyte * >( pEnd ) )
	, _pHead( static_cast< mbyte * >( pBegin ) )
	, _pTail( static_cast< mbyte * >( pBegin ) )
	{
		mem_assert( pBegin != 0 );
		mem_assert( pEnd != 0 );
		mem_assert( pBegin < pEnd );
	}

	void * CircularAllocator::alloc( std::size_t size )
	{
		mem_assert( _pBegin < _pEnd );
		mem_assert( _pBegin <= _pHead && _pHead <= _pEnd );
		mem_assert( _pBegin <= _pTail && _pTail <= _pEnd );

		const std::size_t totalSize = size + sizeof( msize );
		mbyte * pBeginAlloc = 0;
		mbyte * pNewHead = _pHead + totalSize;
		if(	pNewHead <= _pEnd )
			{ pBeginAlloc = _pHead; }
		else
		{
			pNewHead = _pBegin + totalSize;
			if(	pNewHead <= _pTail )
				{ pBeginAlloc = _pBegin; }
		}

		if( pBeginAlloc )
		{
			_impl::Browser browser;
			browser.pByte = pBeginAlloc;
			*(browser.pSize++) = size;
			_pHead = pBeginAlloc + totalSize;

			return browser.pByte;
		}

		// Failed alloc
		return 0;
	}

	void CircularAllocator::dealloc( void * ptr )
	{
		mem_assert( _pBegin < _pEnd );
		mem_assert( _pBegin <= _pHead && _pHead <= _pEnd );
		mem_assert( _pBegin <= _pTail && _pTail <= _pEnd );
		mem_assert(	( _pBegin + sizeof( msize ) ) <= ptr && ptr < _pEnd );

		_impl::Browser browser;
		browser.pByte = static_cast< mbyte * >( ptr );
		const msize size = *(browser.pSize - 1);
		mem_assert( _pTail == reinterpret_cast< mbyte * >(browser.pSize - 1) );
		_pTail = ( browser.pByte + size );
		mem_assert( _pBegin <= _pTail && _pTail <= _pEnd );
	}
}
