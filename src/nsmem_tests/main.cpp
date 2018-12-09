// This file is subject to the terms and conditions defined in the file 'LICENSE.txt'.

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif // _MSC_VER

#include "nsmem/mem.hpp"

#include "nsmem/DebugAllocator.hpp"
#include "nsmem/MallocHeapAllocator.hpp"
#include "nsmem/debug/FileLogStream.hpp"
#include "nsmem/debug/PrintfLogStream.hpp"
#include "nsmem/debug/Win32LogStream.hpp"
#include "nsmem/StackFrameAllocator.hpp"
#include "nsmem/LifoAllocator.hpp"
#include "nsmem/DoubleLifoAllocator.hpp"
#include "nsmem/DLHeapAllocator.hpp"
#include "nsmem/CircularAllocator.hpp"
#include "nsmem/PoolAllocator.hpp"
#include "nsmem/StlAllocator.hpp"
#include "nsmem/SmallBlocksAllocator.hpp"
#include "nsmem/BlockAllocator.hpp"
#include "nsmem/VirtualImpl.hpp"
#include "nsmem/debug/BoundsGuard.hpp"

#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>

using namespace mem;

struct Point2D
{
	Point2D( void ) : x(0), y(0) {}
	float x;
	float y;
};

VirtualImpl< DLHeapAllocator > theHeap( mem::MB(100.0) );
struct stlAlloc1
{
	static DLHeapAllocator * pStaticAllocator;
};
struct stlAlloc2
{
	static DLHeapAllocator * pStaticAllocator;
};

DLHeapAllocator * stlAlloc1::pStaticAllocator = & theHeap.impl();
DLHeapAllocator * stlAlloc2::pStaticAllocator = & theHeap.impl();

int main( void )
{
{
	assert( KB(1) == 1024 );
	assert( KB(0.5) == 512 );
	assert( KB(0.25) == 256 );
	assert( GB(1) == 1073741824 );

	DebugAllocator< DLHeapAllocator,
					CountingLeakTracker,
					DefaultLogFormater,
					std::ostream,
					BoundsGuard< 8 > >	allocator( theHeap.impl(), std::cout );	// std::cerr std::clog

	int maxNums = 1000;
	int * pNums = m_new_array( allocator, int )[ maxNums ];
	for( int broke = 0; broke </*=*/ maxNums; ++broke )
	{
		pNums[broke] = 666;
	}
	m_delete_array( allocator ) pNums;

	float * pF = m_new( allocator, float )( 3.141592f );
	m_delete( allocator ) pF;

	const int c = 32;
	mem::mbyte* pBytes = m_new_array( theHeap, mem::mbyte )[ c ];
	mem::DoubleLifoAllocator lifo( pBytes, pBytes + c );
	int* pStart = m_new( lifo.top(), int )( 666 );
	for( int i = 0; i < 5; ++i )
	{
		int* pInt = m_new( lifo.bottom(), int )( 666 );
		*pInt;
	}
	m_delete( lifo.top() ) pStart;

	std::vector< int, mem::StlAllocator< int, stlAlloc1 > > nombres1;
	std::vector< int, mem::StlAllocator< int, stlAlloc2 > > nombres2;
	nombres1.reserve( 100 );
	nombres1.push_back( 20 );

	mem::BlockAllocator balloc( pBytes, pBytes + c, 4 );
	for( int i = 0; i < 8; ++i )
	{
		int* pInt = m_new( balloc, int )( 666 );
		*pInt = 0xcc;
		m_delete( balloc ) pInt;

	}

	//mem::StackFrameAllocator< 1024 > stackMem;
	//float * pY0 = m_new_array( stackMem, float )[ 50 ];
	//m_delete_array( stackMem ) pY0;
	/*
	DoubleLifoAllocator dlifo( pBytes, pBytes + c );
	void* pIt = dlifo.top().alloc( 25 );
	void* pItToo = dlifo.bottom().alloc( 25 );
	pIt;
	pItToo;
	*/

	mbyte * pF2 = reinterpret_cast< mbyte * >( m_new_array( theHeap, float )[ 100 ] );
	LifoBackAllocator backLifo( pF2, pF2 + sizeof( float ) * 100 );
	std::vector< Point2D * > points;
	points.resize( 25 );
	for( auto & p : points )
	{
		p = m_new( backLifo, Point2D );
		p->x = FLT_MAX;
		p->y = FLT_MAX;
	}
	for( auto p = points.rbegin(); p != points.rend(); ++p )
	{
		m_delete( backLifo ) *p;
	}
	m_delete_array( theHeap ) pF2;

	std::size_t growSizes[] = { 2000, 1000, 900, 800, 700, 600, 500, 400, 300, 200, 100 };
	SmallBlocksAllocator sbAlloc( 1, 1024, theHeap, growSizes );
	std::vector< char * > chunks;
	for( int z = 0; z < 10000; ++z )
	{
		char * pChar = m_new_array( sbAlloc, char )[ rand() % 1024 + 1 ];
		chunks.push_back( pChar );
	}
	std::random_shuffle( chunks.begin(), chunks.end() );
	for( std::vector< char *>::iterator iterChunks = chunks.begin();
		iterChunks != chunks.end();
		++iterChunks )
	{
		m_delete_array( sbAlloc ) *iterChunks;
	}

	mem::VirtualImpl< SequentialAllocator > vLinear( pBytes, pBytes + c );
	vLinear.alloc( 10 );

	m_delete_array( theHeap ) pBytes;

	std::size_t logInfoBytes = sizeof( LogInfo );
	std::cout << logInfoBytes;
 }
#ifdef _MSC_VER
	_CrtDumpMemoryLeaks();
#endif

	::system( "pause" );
	return 0;
}
