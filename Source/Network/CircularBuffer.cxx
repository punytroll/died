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
//~ 	std::cout << "[Read]: this = " << this << "; BufferSize = " << stGetSize() << "\n\tpu8Memory = " << reinterpret_cast< int * >(pu8Memory) << "; stSize = " << stSize << "\n\tm_pu8StorageBegin = " << reinterpret_cast< int * >(m_pu8StorageBegin) << "; m_pu8StorageEnd = " << reinterpret_cast< int * >(m_pu8StorageEnd) << "\n\tm_pu8DataBegin = " << reinterpret_cast< int * >(m_pu8DataBegin) << "; m_pu8DataEnd = " << reinterpret_cast< int * >(m_pu8DataEnd) << std::endl;
	
	if(m_pu8DataBegin == m_pu8StorageEnd)
	{
		m_pu8DataBegin = m_pu8StorageBegin;
	}
	
	size_t stReturn = stGetSize() <? stSize;
	
	if(m_pu8DataBegin <= m_pu8DataEnd)
	{
		
//~ 		std::cout << "\t[Read]: DataBegin <= DataEnd [" << stReturn << "]." << std::endl;
		memcpy(pu8Memory, m_pu8DataBegin, stReturn);
		m_pu8DataBegin += stReturn;
	}
	else
	{
		if(m_pu8StorageEnd >= m_pu8DataBegin + stReturn)
		{
//~ 			std::cout << "[Read]: Overlap.\n\tthis = " << this << "; BufferSize = " << stGetSize() << "\n\tpu8Memory = " << reinterpret_cast< int * >(pu8Memory) << "; stSize = " << stReturn << "\n\tm_pu8StorageBegin = " << reinterpret_cast< int * >(m_pu8StorageBegin) << "; m_pu8StorageEnd = " << reinterpret_cast< int * >(m_pu8StorageEnd) << "\n\tm_pu8DataBegin = " << reinterpret_cast< int * >(m_pu8DataBegin) << "; m_pu8DataEnd = " << reinterpret_cast< int * >(m_pu8DataEnd) << std::endl;
			memcpy(pu8Memory, m_pu8DataBegin, stReturn);
			m_pu8DataBegin += stReturn;
			
//~ 			std::cout << "[Read]: If we were here, things would be nice." << std::endl;
		}
		else
		{
//~ 			std::cout << "[Read]: Overlap.\n\tthis = " << this << "; BufferSize = " << stGetSize() << "\n\tpu8Memory = " << reinterpret_cast< int * >(pu8Memory) << "; stSize = " << stReturn << "\n\tm_pu8StorageBegin = " << reinterpret_cast< int * >(m_pu8StorageBegin) << "; m_pu8StorageEnd = " << reinterpret_cast< int * >(m_pu8StorageEnd) << "\n\tm_pu8DataBegin = " << reinterpret_cast< int * >(m_pu8DataBegin) << "; m_pu8DataEnd = " << reinterpret_cast< int * >(m_pu8DataEnd) << std::endl;
			
			size_t stEndSize(m_pu8StorageEnd - m_pu8DataBegin);
			
			memcpy(pu8Memory, m_pu8DataBegin, stEndSize);
			memcpy(pu8Memory + stEndSize, m_pu8StorageBegin, stReturn - stEndSize);
			m_pu8DataBegin = m_pu8StorageBegin + (stReturn - stEndSize);
			
//~ 			std::cout << "[Read]: If we were here, things would be even more nice." << std::endl;
		}
	}
//~ 	std::cout << "[Read]: this = " << this << "; BufferSize = " << stGetSize() << "\n\tpu8Memory = " << reinterpret_cast< int * >(pu8Memory) << "; stSize = " << stReturn << "\n\tm_pu8StorageBegin = " << reinterpret_cast< int * >(m_pu8StorageBegin) << "; m_pu8StorageEnd = " << reinterpret_cast< int * >(m_pu8StorageEnd) << "\n\tm_pu8DataBegin = " << reinterpret_cast< int * >(m_pu8DataBegin) << "; m_pu8DataEnd = " << reinterpret_cast< int * >(m_pu8DataEnd) << "\n\tstReturn = " << stReturn << std::endl;
	
	return stReturn;
}

void Network::CircularBuffer::vWrite(const u_int8_t * pu8Memory, size_t stSize)
{
	// we need a buffer size + 1 because Begin may only be equal End if the buffer is empty.
	if(stSize >= stGetFree())
	{
//~ 		std::cout << "[Write]: TODO: Enlarge buffer." << std::endl;
		// TODO: enlarge buffer
	}
	if(m_pu8DataBegin <= m_pu8DataEnd)
	{
//~ 		std::cout << "[Write]: DataBegin <= DataEnd" << std::endl;
		if(m_pu8StorageEnd >= stSize + m_pu8DataEnd)
		{
//~ 			std::cout << "[Write]: New data fits." << std::endl;
			memcpy(m_pu8DataEnd, pu8Memory, stSize);
			m_pu8DataEnd += stSize;
		}
		else
		{
//~ 			std::cout << "[Write]: New data overlaps the storage end." << std::endl;
//~ 			std::cout << "[Write]: this = " << this << "; BufferSize = " << stGetSize() << "\n\tpu8Memory = " << reinterpret_cast< const int * >(pu8Memory) << "; stSize = " << stSize << "\n\tm_pu8StorageBegin = " << reinterpret_cast< int * >(m_pu8StorageBegin) << "; m_pu8StorageEnd = " << reinterpret_cast< int * >(m_pu8StorageEnd) << "\n\tm_pu8DataBegin = " << reinterpret_cast< int * >(m_pu8DataBegin) << "; m_pu8DataEnd = " << reinterpret_cast< int * >(m_pu8DataEnd) << std::endl;
			memcpy(m_pu8DataEnd, pu8Memory, m_pu8StorageEnd - m_pu8DataEnd);
			stSize -= (m_pu8StorageEnd - m_pu8DataEnd);
			memcpy(m_pu8StorageBegin, pu8Memory + (m_pu8StorageEnd - m_pu8DataEnd), stSize);
			m_pu8DataEnd = m_pu8StorageBegin + stSize;
//~ 			std::cout << "[Write]: this = " << this << "; BufferSize = " << stGetSize() << "\n\tpu8Memory = " << reinterpret_cast< const int * >(pu8Memory) << "; stSize = " << stSize << "\n\tm_pu8StorageBegin = " << reinterpret_cast< int * >(m_pu8StorageBegin) << "; m_pu8StorageEnd = " << reinterpret_cast< int * >(m_pu8StorageEnd) << "\n\tm_pu8DataBegin = " << reinterpret_cast< int * >(m_pu8DataBegin) << "; m_pu8DataEnd = " << reinterpret_cast< int * >(m_pu8DataEnd) << std::endl;
		}
	}
	else
	{
//~ 		std::cout << "[Write]: DataBegin > DataEnd" << std::endl;
		memcpy(m_pu8DataEnd, pu8Memory, stSize);
		m_pu8DataEnd += stSize;
	}
}
