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
