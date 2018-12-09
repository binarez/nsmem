// This file is subject to the terms and conditions defined in the file 'LICENSE.txt'.

#ifndef NSMEM_MEM_HPP
#define NSMEM_MEM_HPP

#include "nsmem/mem_impl.hpp"

#include <new>

#ifdef NSMEM_DEBUG

#define m_new( ALLOCATOR, T )			new( (ALLOCATOR).allocDbg( sizeof( T ), mem::_impldbg::debugTempRef( mem::LogInfo( mem::Event::New, __FILE__, __LINE__, #T ) ) ) ) T
#define m_delete( ALLOCATOR )			mem::_impldbg::Destroyer< mem_decltype(ALLOCATOR) >( (ALLOCATOR), mem::LogInfo( mem::Event::Delete, __FILE__, __LINE__, "" ) ) -
#define m_new_array( ALLOCATOR, T )		mem::_impldbg::ArrayCreator< mem_decltype(ALLOCATOR), T >( (ALLOCATOR), mem::LogInfo( mem::Event::NewArray, __FILE__, __LINE__, #T ) )
#define m_delete_array( ALLOCATOR )		mem::_impldbg::ArrayDestroyer< mem_decltype(ALLOCATOR) >( (ALLOCATOR), mem::LogInfo( mem::Event::DeleteArray, __FILE__, __LINE__, "" ) ) -

#else

#define m_new( ALLOCATOR, T )			new( (ALLOCATOR).alloc( sizeof( T ) ) ) T
#define m_delete( ALLOCATOR )			mem::_impl::Destroyer< mem_decltype(ALLOCATOR) >( ALLOCATOR ) -
#define m_new_array( ALLOCATOR, T )		mem::_impl::ArrayCreator< mem_decltype(ALLOCATOR), T >( ALLOCATOR )
#define m_delete_array( ALLOCATOR )		mem::_impl::ArrayDestroyer< mem_decltype(ALLOCATOR) >( ALLOCATOR ) -

#endif

#endif // NSMEM_MEM_HPP
