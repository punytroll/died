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

#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <sys/types.h>

#include "BasicBuffer.h"

namespace Network
{
	class CircularBuffer : public Network::BasicBuffer
	{
	public:
		CircularBuffer(size_t stSize);
		virtual size_t stGetCapacity(void);
		virtual size_t stGetSize(void);
		virtual size_t stRead(u_int8_t * pu8Memory, size_t stSize);
		virtual void vWrite(const u_int8_t * pu8Memory, size_t stSize);
	private:
		u_int8_t * m_pu8StorageBegin;
		u_int8_t * m_pu8StorageEnd;
		u_int8_t * m_pu8DataBegin;
		u_int8_t * m_pu8DataEnd;
	};
}

#endif
