#include "BufferReader.h"

#include <iostream>

Network::BufferReader::BufferReader(Network::BasicBuffer & Buffer) :
	m_Buffer(Buffer)
{
}

bool Network::BufferReader::bRead(bool & bValue)
{
	std::cout << "Reading bool from the circular buffer." << std::endl;
	
	if(m_Buffer.stGetSize() < sizeof(bool))
	{
		return false;
	}
	
	size_t stReturn = m_Buffer.stRead(reinterpret_cast< u_int8_t * >(&bValue), sizeof(bool));
	
	std::cout << "\tValue = " << bValue << std::endl;

	return stReturn == sizeof(bool);
}

bool Network::BufferReader::bRead(u_int8_t & u8Value)
{
	std::cout << "Reading u_int8_t from the circular buffer." << std::endl;
	
	if(m_Buffer.stGetSize() < sizeof(u_int8_t))
	{
		return false;
	}
	
	size_t stReturn = m_Buffer.stRead(reinterpret_cast< u_int8_t * >(&u8Value), sizeof(u_int8_t));
	
	std::cout << "\tValue = " << u8Value << std::endl;

	return stReturn == sizeof(u_int8_t);
}

bool Network::BufferReader::bRead(int8_t & s8Value)
{
	std::cout << "Reading int8_t from the circular buffer." << std::endl;
	
	if(m_Buffer.stGetSize() < sizeof(int8_t))
	{
		return false;
	}
	
	size_t stReturn = m_Buffer.stRead(reinterpret_cast< u_int8_t * >(&s8Value), sizeof(int8_t));
	
	std::cout << "\tValue = " << s8Value << std::endl;

	return stReturn == sizeof(int8_t);
}

bool Network::BufferReader::bRead(u_int16_t & u16Value)
{
	std::cout << "Reading u_int16_t from the circular buffer." << std::endl;
	
	if(m_Buffer.stGetSize() < sizeof(u_int16_t))
	{
		return false;
	}
	
	size_t stReturn = m_Buffer.stRead(reinterpret_cast< u_int8_t * >(&u16Value), sizeof(u_int16_t));
	
	std::cout << "\tValue = " << u16Value << std::endl;

	return stReturn == sizeof(u_int16_t);
}

bool Network::BufferReader::bRead(int16_t & s16Value)
{
	std::cout << "Reading int16_t from the circular buffer." << std::endl;
	
	if(m_Buffer.stGetSize() < sizeof(int16_t))
	{
		return false;
	}
	
	size_t stReturn = m_Buffer.stRead(reinterpret_cast< u_int8_t * >(&s16Value), sizeof(int16_t));
	
	std::cout << "\tValue = " << s16Value << std::endl;

	return stReturn == sizeof(int16_t);
}

bool Network::BufferReader::bRead(u_int32_t & u32Value)
{
	std::cout << "Reading u_int32_t from the circular buffer." << std::endl;
	
	if(m_Buffer.stGetSize() < sizeof(u_int32_t))
	{
		return false;
	}
	
	size_t stReturn = m_Buffer.stRead(reinterpret_cast< u_int8_t * >(&u32Value), sizeof(u_int32_t));
	
	std::cout << "\tValue = " << u32Value << std::endl;

	return stReturn == sizeof(u_int32_t);
}

bool Network::BufferReader::bRead(int32_t & s32Value)
{
	std::cout << "Reading int32_t from the circular buffer." << std::endl;
	
	if(m_Buffer.stGetSize() < sizeof(int32_t))
	{
		return false;
	}
	
	size_t stReturn = m_Buffer.stRead(reinterpret_cast< u_int8_t * >(&s32Value), sizeof(int32_t));
	
	std::cout << "\tValue = " << s32Value << std::endl;

	return stReturn == sizeof(int32_t);
}

bool Network::BufferReader::bRead(std::string & sValue)
{
	throw;
}
