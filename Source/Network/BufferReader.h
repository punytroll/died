#ifndef BUFFERREADER_H
#define BUFFERREADER_H

#include "BasicBuffer.h"
#include "BasicReader.h"

namespace Network
{	
	class BufferReader : public Network::BasicReader
	{
	public:
		BufferReader(Network::BasicBuffer & Buffer);
		virtual bool bRead(bool & bValue);
		virtual bool bRead(u_int8_t & u8Value);
		virtual bool bRead(int8_t & s8Value);
		virtual bool bRead(u_int16_t & u16Value);
		virtual bool bRead(int16_t & s16Value);
		virtual bool bRead(u_int32_t & u32Value);
		virtual bool bRead(int32_t & s32Value);
		virtual bool bRead(std::string & sValue);
	private:
		Network::BasicBuffer & m_BasicBuffer;
	};
}

#endif
