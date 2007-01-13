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

#include "BufferWriter.h"

#include <iostream>

Network::BufferWriter::BufferWriter(Network::BasicBuffer & Buffer) :
	m_Buffer(Buffer)
{
}

bool Network::BufferWriter::bWrite(const bool & bValue)
{
//~ 	std::cerr << "Writing bool to the circular buffer. [value = '" << bValue << "'; size = " << sizeof(bool) << "]" << std::endl;
	
	m_Buffer.vWrite(reinterpret_cast< const u_int8_t * >(&bValue), sizeof(bool));
	
	return true;
}

bool Network::BufferWriter::bWrite(const u_int8_t & u8Value)
{
//~ 	std::cerr << "Writing u_int8_t to the circular buffer. [" << u8Value << ']' << std::endl;
	
	m_Buffer.vWrite(reinterpret_cast< const u_int8_t * >(&u8Value), sizeof(u_int8_t));
	
	return true;
}

bool Network::BufferWriter::bWrite(const int8_t & s8Value)
{
//~ 	std::cerr << "Writing int8_t to the circular buffer. [" << s8Value << ']' << std::endl;
	
	m_Buffer.vWrite(reinterpret_cast< const u_int8_t * >(&s8Value), sizeof(int8_t));
	
	return true;
}

bool Network::BufferWriter::bWrite(const u_int16_t & u16Value)
{
//~ 	std::cerr << "Writing u_int16_t to the circular buffer. [" << u16Value << ']' << std::endl;
	
	m_Buffer.vWrite(reinterpret_cast< const u_int8_t * >(&u16Value), sizeof(u_int16_t));
	
	return true;
}

bool Network::BufferWriter::bWrite(const int16_t & s16Value)
{
//~ 	std::cerr << "Writing int16_t to the circular buffer. [" << s16Value << ']' << std::endl;
	
	m_Buffer.vWrite(reinterpret_cast< const u_int8_t * >(&s16Value), sizeof(int16_t));
	
	return true;
}

bool Network::BufferWriter::bWrite(const u_int32_t & u32Value)
{
//~ 	std::cerr << "Writing u_int32_t to the circular buffer. [value = '" << u32Value << "' ; size = " << sizeof(u_int32_t) << "]" << std::endl;
	
	m_Buffer.vWrite(reinterpret_cast< const u_int8_t * >(&u32Value), sizeof(u_int32_t));
	
	return true;
}

bool Network::BufferWriter::bWrite(const int32_t & s32Value)
{
//~ 	std::cerr << "Writing int32_t to the circular buffer. [" << s32Value << ']' << std::endl;
	
	m_Buffer.vWrite(reinterpret_cast< const u_int8_t * >(&s32Value), sizeof(int32_t));
	
	return true;
}

bool Network::BufferWriter::bWrite(const Glib::ustring & sValue)
{
//~ 	std::cerr << "Writing Glib::ustring to the circular buffer. [value = \"" << sValue << "\" ; size = " << (sValue.bytes() + 1) << "]" << std::endl;
	
	u_int8_t * pu8Buffer = new u_int8_t[sValue.bytes() + 1];
	
	memcpy(pu8Buffer, sValue.c_str(), sValue.bytes() + 1);
	m_Buffer.vWrite(pu8Buffer, sValue.bytes() + 1);
	
	return true;
}

bool Network::BufferWriter::bWrite(const void * pvMemory, std::size_t stSize)
{
//~ 	std::cerr << "Writing raw to the circular buffer. [size = " << stSize << "]" << std::endl;
	
	m_Buffer.vWrite(reinterpret_cast< const u_int8_t * >(pvMemory), stSize);
	
	return true;
}
