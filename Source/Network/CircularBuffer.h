#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <sys/types.h>

#include <boost/shared_ptr.hpp>

#include "BasicBuffer.h"
#include "BufferReader.h"

namespace Network
{
	class CircularBuffer : public Network::BasicBuffer
	{
	public:
		CircularBuffer(size_t stSize) :
			m_pu8StorageBegin(new u_int8_t[stSize]),
			m_pu8StorageEnd(m_pu8StorageBegin + stSize),
			m_pu8DataBegin(m_pu8StorageBegin),
			m_pu8DataEnd(m_pu8DataEnd)
		{
		}
		
		virtual size_t stGetCount(void)
		{
			if(m_pu8DataBegin <= m_pu8DataEnd)
			{
				return (m_pu8DataEnd - m_pu8DataBegin);
			}
			else
			{
				return (m_pu8StorageEnd - m_pu8DataBegin) + (m_pu8DataEnd - m_pu8StorageBegin);
			}
		}
	public:
		boost::shared_ptr< Network::BufferReader > GetReader(void)
		{
			return boost::shared_ptr< Network::BufferReader >(new BufferReader(*this));
		}
	private:
		u_int8_t * m_pu8StorageBegin;
		u_int8_t * m_pu8StorageEnd;
		u_int8_t * m_pu8DataBegin;
		u_int8_t * m_pu8DataEnd;
	};
}

#endif
