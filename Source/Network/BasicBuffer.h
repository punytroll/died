#ifndef BASICBUFFER_H
#define BASICBUFFER_H

#include <sys/types.h>

namespace Network
{
	class BasicBuffer
	{
	public:
		virtual void vWrite(const u_int8_t * pu8Memory, size_t stSize) = 0;
	};
}

#endif
