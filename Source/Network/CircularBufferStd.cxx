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

#include "CircularBufferStd.h"

#include <iostream>
#include <algorithm>

#include <Common.h>

Network::CircularBufferStd::CircularBufferStd(size_t stSize) :
	m_Ring(stSize, 0)
{
}

Network::CircularBufferStd::~CircularBufferStd(void)
{
}

size_t Network::CircularBuffer::stGetCapacity(void)
{
	return m_Ring.max_size();
}

size_t Network::CircularBuffer::stGetSize(void)
{
	return m_Ring.size();
}

size_t Network::CircularBuffer::stRead(u_int8_t * pu8Memory, size_t stSize)
{
	size_t stReturn(std::min(stGetSize(), stSize));
	
	std::copy(m_Ring.begin(), m_Ring.begin() + stReturn, pu8Memory);
	m_Ring.erase(m_Ring.begin(), m_Ring.begin() + stReturn);
	
	return stReturn;
}

void Network::CircularBuffer::vWrite(const u_int8_t * pu8Memory, size_t stSize)
{
	m_Ring.insert(m_Ring.end(), pu8Memory, pu8Memory + stSize);
}
