#ifndef BASICBUFFER_H
#define BASICBUFFER_H

#include <sys/types.h>

namespace Network
{
	class BasicBuffer
	{
	public:
		static const size_t npos = 0xFFFFFFFF;
		virtual size_t stGetSize(void) = 0;
		virtual size_t stRead(u_int8_t * pu8Memory, size_t stSize) = 0;
		virtual void vWrite(const u_int8_t * pu8Memory, size_t stSize) = 0;
	};
}

#endif
