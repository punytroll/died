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

#include "BasicBuffer.h"

#include "BufferReader.h"
#include "BufferWriter.h"

Network::BasicBuffer::size_type Network::BasicBuffer::stGetFree(void)
{
	return stGetCapacity() - stGetSize();
}

Network::BufferReader Network::BasicBuffer::GetReader(void)
{
	return BufferReader(*this);
}

Network::BufferWriter Network::BasicBuffer::GetWriter(void)
{
	return BufferWriter(*this);
}
