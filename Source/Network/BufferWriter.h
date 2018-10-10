/* DiED - A distributed Editor.
 * Copyright (C) 2005 Hagen Möbius & Aram Altschudjian
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

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
		virtual bool bWrite(const Glib::ustring & sValue);
		virtual bool bWrite(const void * pvMemory, std::size_t stSize);
	private:
		Network::BasicBuffer & m_Buffer;
	};
}

#endif
