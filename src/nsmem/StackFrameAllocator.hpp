// This file is subject to the terms and conditions defined in the file 'LICENSE.txt'.

#ifndef NSMEM_STACK_ALLOCATOR_HPP
#define NSMEM_STACK_ALLOCATOR_HPP

#include "nsmem/SequentialAllocator.hpp"

namespace mem
{
	template< std::size_t NUM_BYTES, typename FIXED_ALLOCATOR >
	class StackFrameAllocator : public FIXED_ALLOCATOR
	{
	public:
								StackFrameAllocator( void )
									: FIXED_ALLOCATOR( _buffer, _buffer + NUM_BYTES )
									{ }
	protected:
		char					_buffer[ NUM_BYTES ];
	};
}

#endif // NSMEM_STACK_ALLOCATOR_HPP
