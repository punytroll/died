#ifndef BUFFERWRITER_H
#define BUFFERWRITER_H

#include "BasicBuffer.h"
#include "BasicWriter.h"

namespace Network
{	
	class BufferWriter : public Network::BasicWriter
	{
	public:
		BufferWriter(Network::BasicBuffer & Buffer);
		virtual bool bWrite(const bool & bValue);
		virtual bool bWrite(const u_int8_t & u8Value);
		virtual bool bWrite(const int8_t & s8Value);
		virtual bool bWrite(const u_int16_t & u16Value);
		virtual bool bWrite(const int16_t & s16Value);
		virtual bool bWrite(const u_int32_t & u32Value);
		virtual bool bWrite(const int32_t & s32Value);
		virtual bool bWrite(const std::string & sValue);
	private:
		Network::BasicBuffer & m_Buffer;
	};
}

#endif
