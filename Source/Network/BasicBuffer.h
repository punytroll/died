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

#ifndef BASICBUFFER_H
#define BASICBUFFER_H

#include <sys/types.h>

namespace Network
{
	class BasicBuffer
	{
	public:
		static const size_t npos = 0xFFFFFFFF;
		virtual size_t stGetSize(void) = 0;
		virtual size_t stRead(u_int8_t * pu8Memory, size_t stSize) = 0;
		virtual void vWrite(const u_int8_t * pu8Memory, size_t stSize) = 0;
	};
}

#endif
