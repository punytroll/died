#include "BufferWriter.h"

#include <iostream>

Network::BufferWriter::BufferWriter(Network::BasicBuffer & Buffer) :
	m_Buffer(Buffer)
{
}

bool Network::BufferWriter::bWrite(const bool & bValue)
{
	std::cerr << "Writing bool to the circular buffer. [" << bValue << ']' << std::endl;
	
	m_Buffer.vWrite(reinterpret_cast< const u_int8_t * >(&bValue), sizeof(bool));
}

bool Network::BufferWriter::bWrite(const u_int8_t & u8Value)
{
	std::cerr << "Writing u_int8_t to the circular buffer. [" << u8Value << ']' << std::endl;
	
	m_Buffer.vWrite(reinterpret_cast< const u_int8_t * >(&u8Value), sizeof(u_int8_t));
}

bool Network::BufferWriter::bWrite(const int8_t & s8Value)
{
	std::cerr << "Writing int8_t to the circular buffer. [" << s8Value << ']' << std::endl;
	
	m_Buffer.vWrite(reinterpret_cast< const u_int8_t * >(&s8Value), sizeof(int8_t));
}

bool Network::BufferWriter::bWrite(const u_int16_t & u16Value)
{
	std::cerr << "Writing u_int16_t to the circular buffer. [" << u16Value << ']' << std::endl;
	
	m_Buffer.vWrite(reinterpret_cast< const u_int8_t * >(&u16Value), sizeof(u_int16_t));
}

bool Network::BufferWriter::bWrite(const int16_t & s16Value)
{
	std::cerr << "Writing int16_t to the circular buffer. [" << s16Value << ']' << std::endl;
	
	m_Buffer.vWrite(reinterpret_cast< const u_int8_t * >(&s16Value), sizeof(int16_t));
}

bool Network::BufferWriter::bWrite(const u_int32_t & u32Value)
{
	std::cerr << "Writing u_int32_t to the circular buffer. [" << u32Value << ']' << std::endl;
	
	m_Buffer.vWrite(reinterpret_cast< const u_int8_t * >(&u32Value), sizeof(u_int32_t));
}

bool Network::BufferWriter::bWrite(const int32_t & s32Value)
{
	std::cerr << "Writing int32_t to the circular buffer. [" << s32Value << ']' << std::endl;
	
	m_Buffer.vWrite(reinterpret_cast< const u_int8_t * >(&s32Value), sizeof(int32_t));
}

bool Network::BufferWriter::bWrite(const std::string & sValue)
{
	throw;
}
