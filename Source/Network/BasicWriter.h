#ifndef BASICWRITER_H
#define BASICWRITER_H

#include <sys/types.h>

#include <glibmm/ustring.h>

namespace Network
{
	class BasicWriter
	{
	public:
		virtual bool bWrite(const bool & bValue) = 0;
		virtual bool bWrite(const u_int8_t & u8Value) = 0;
		virtual bool bWrite(const int8_t & s8Value) = 0;
		virtual bool bWrite(const u_int16_t & u16Value) = 0;
		virtual bool bWrite(const int16_t & s16Value) = 0;
		virtual bool bWrite(const u_int32_t & u32Value) = 0;
		virtual bool bWrite(const int32_t & s32Value) = 0;
		virtual bool bWrite(const Glib::ustring & sValue) = 0;
		virtual bool bWrite(const void * pvMemory, std::size_t stSize) = 0;
	};
}

#endif
