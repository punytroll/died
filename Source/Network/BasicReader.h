#ifndef BASICREADER_H
#define BASICREADER_H

#include <sys/types.h>

#include <glibmm/ustring.h>

namespace Network
{
	class BasicReader
	{
	public:
		virtual bool bRead(bool & bValue) = 0;
		virtual bool bRead(u_int8_t & u8Value) = 0;
		virtual bool bRead(int8_t & s8Value) = 0;
		virtual bool bRead(u_int16_t & u16Value) = 0;
		virtual bool bRead(int16_t & s16Value) = 0;
		virtual bool bRead(u_int32_t & u32Value) = 0;
		virtual bool bRead(int32_t & s32Value) = 0;
		virtual bool bRead(Glib::ustring & sValue) = 0;
	};
}

#endif
