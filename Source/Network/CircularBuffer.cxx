#include "CircularBuffer.h"

#include <iostream>

Network::CircularBuffer::CircularBuffer(size_t stSize) :
	m_pu8StorageBegin(new u_int8_t[stSize]),
	m_pu8StorageEnd(m_pu8StorageBegin + stSize),
	m_pu8DataBegin(m_pu8StorageBegin),
	m_pu8DataEnd(m_pu8StorageBegin)
{
}

boost::shared_ptr< Network::BufferReader > Network::CircularBuffer::GetReader(void)
{
	return boost::shared_ptr< Network::BufferReader >(new BufferReader(*this));
}

boost::shared_ptr< Network::BufferWriter > Network::CircularBuffer::GetWriter(void)
{
	return boost::shared_ptr< Network::BufferWriter >(new BufferWriter(*this));
}

size_t Network::CircularBuffer::stGetCapacity(void)
{
	return m_pu8StorageEnd - m_pu8StorageBegin;
}

size_t Network::CircularBuffer::stGetSize(void)
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

size_t Network::CircularBuffer::stGetFree(void)
{
	return stGetCapacity() - stGetSize();
}

size_t Network::CircularBuffer::stRead(u_int8_t * pu8Memory, size_t stSize)
{
	if(m_pu8DataBegin <= m_pu8DataEnd)
	{
		size_t stReturn = stGetSize() <? stSize;
		
		std::cout << "DataBegin <= DataEnd [" << stReturn << "]." << std::endl;
		memcpy(pu8Memory, m_pu8DataBegin, stReturn);
		m_pu8DataBegin += stReturn;
		
		return stReturn;
	}
	else
	{
		if((m_pu8StorageEnd - m_pu8DataBegin) >= stSize)
		{
			memcpy(pu8Memory, m_pu8DataBegin, stSize);
			m_pu8DataBegin += stSize;
			
			return stSize;
		}
		else
		{
			size_t stReturn = stGetSize();
			
			memcpy(pu8Memory, m_pu8DataBegin, m_pu8StorageEnd - m_pu8DataBegin);
			memcpy(pu8Memory + (m_pu8StorageEnd - m_pu8DataBegin), m_pu8StorageBegin, m_pu8DataEnd - m_pu8StorageBegin);
			m_pu8DataBegin = m_pu8DataEnd;
			
			return stReturn;
		}
	}
}

void Network::CircularBuffer::vWrite(const u_int8_t * pu8Memory, size_t stSize)
{
	// we need a buffer size + 1 because Begin may only be equal End if the buffer is empty.
	if(stSize >= stGetFree())
	{
		std::cout << "Enlarge buffer." << std::endl;
		// TODO: enlarge buffer
	}
	if(m_pu8DataBegin <= m_pu8DataEnd)
	{
		std::cout << "DataBegin <= DataEnd" << std::endl;
		if((m_pu8StorageEnd - m_pu8DataEnd) >= stSize)
		{
			memcpy(m_pu8DataEnd, pu8Memory, stSize);
			m_pu8DataEnd += stSize;
		}
		else
		{
			memcpy(m_pu8DataEnd, pu8Memory, m_pu8StorageEnd - m_pu8DataEnd);
			stSize -= (m_pu8StorageEnd - m_pu8DataEnd);
			memcpy(m_pu8StorageBegin, pu8Memory + (m_pu8StorageEnd - m_pu8DataEnd), stSize);
			m_pu8DataEnd = m_pu8StorageBegin + stSize;
		}
	}
	else
	{
		memcpy(m_pu8DataEnd, pu8Memory, stSize);
		m_pu8DataEnd += stSize;
	}
}
