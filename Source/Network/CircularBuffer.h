#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <sys/types.h>

#include <boost/shared_ptr.hpp>

#include "BasicBuffer.h"
#include "BufferReader.h"
#include "BufferWriter.h"

namespace Network
{
	class CircularBuffer : public Network::BasicBuffer
	{
	public:
		CircularBuffer(size_t stSize);
		boost::shared_ptr< Network::BufferReader > GetReader(void);
		boost::shared_ptr< Network::BufferWriter > GetWriter(void);
		virtual size_t stGetCapacity(void);
		virtual size_t stGetSize(void);
		virtual size_t stGetFree(void);
		virtual size_t stRead(u_int8_t * pu8Memory, size_t stSize);
		virtual void vWrite(const u_int8_t * pu8Memory, size_t stSize);
	private:
		u_int8_t * m_pu8StorageBegin;
		u_int8_t * m_pu8StorageEnd;
		u_int8_t * m_pu8DataBegin;
		u_int8_t * m_pu8DataEnd;
	};
}

#endif
