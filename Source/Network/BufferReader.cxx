#include "BufferReader.h"

Network::BufferReader::BufferReader(Network::BasicBuffer & Buffer) :
	m_BasicBuffer(Buffer)
{
}

bool Network::BufferReader::bRead(bool & bValue)
{
}

bool Network::BufferReader::bRead(u_int8_t & u8Value)
{
}

bool Network::BufferReader::bRead(int8_t & s8Value)
{
}

bool Network::BufferReader::bRead(u_int16_t & u16Value)
{
}

bool Network::BufferReader::bRead(int16_t & s16Value)
{
}

bool Network::BufferReader::bRead(u_int32_t & u32Value)
{
}

bool Network::BufferReader::bRead(int32_t & s32Value)
{
}

bool Network::BufferReader::bRead(std::string & sValue)
{
}
