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

#include "BufferReader.h"

#include <iostream>

Network::BufferReader::BufferReader(Network::BasicBuffer & Buffer) :
	m_Buffer(Buffer)
{
}

bool Network::BufferReader::bRead(bool & bValue)
{
//~ 	std::cout << "Reading bool from the circular buffer." << std::endl;
	
	if(m_Buffer.stGetSize() < sizeof(bool))
	{
		return false;
	}
	
	size_t stReturn = m_Buffer.stRead(reinterpret_cast< u_int8_t * >(&bValue), sizeof(bool));
	
//~ 	std::cout << "\tValue = " << bValue << std::endl;

	return stReturn == sizeof(bool);
}

bool Network::BufferReader::bRead(u_int8_t & u8Value)
{
//~ 	std::cout << "Reading u_int8_t from the circular buffer." << std::endl;
	
	if(m_Buffer.stGetSize() < sizeof(u_int8_t))
	{
		return false;
	}
	
	size_t stReturn = m_Buffer.stRead(reinterpret_cast< u_int8_t * >(&u8Value), sizeof(u_int8_t));
	
//~ 	std::cout << "\tValue = " << u8Value << std::endl;

	return stReturn == sizeof(u_int8_t);
}

bool Network::BufferReader::bRead(int8_t & s8Value)
{
//~ 	std::cout << "Reading int8_t from the circular buffer." << std::endl;
	
	if(m_Buffer.stGetSize() < sizeof(int8_t))
	{
		return false;
	}
	
	size_t stReturn = m_Buffer.stRead(reinterpret_cast< u_int8_t * >(&s8Value), sizeof(int8_t));
	
//~ 	std::cout << "\tValue = " << s8Value << std::endl;

	return stReturn == sizeof(int8_t);
}

bool Network::BufferReader::bRead(u_int16_t & u16Value)
{
//~ 	std::cout << "Reading u_int16_t from the circular buffer." << std::endl;
	
	if(m_Buffer.stGetSize() < sizeof(u_int16_t))
	{
		return false;
	}
	
	size_t stReturn = m_Buffer.stRead(reinterpret_cast< u_int8_t * >(&u16Value), sizeof(u_int16_t));
	
//~ 	std::cout << "\tValue = " << u16Value << std::endl;

	return stReturn == sizeof(u_int16_t);
}

bool Network::BufferReader::bRead(int16_t & s16Value)
{
//~ 	std::cout << "Reading int16_t from the circular buffer." << std::endl;
	
	if(m_Buffer.stGetSize() < sizeof(int16_t))
	{
		return false;
	}
	
	size_t stReturn = m_Buffer.stRead(reinterpret_cast< u_int8_t * >(&s16Value), sizeof(int16_t));
	
//~ 	std::cout << "\tValue = " << s16Value << std::endl;

	return stReturn == sizeof(int16_t);
}

bool Network::BufferReader::bRead(u_int32_t & u32Value)
{
//~ 	std::cout << "Reading u_int32_t from the circular buffer." << std::endl;
	
	if(m_Buffer.stGetSize() < sizeof(u_int32_t))
	{
		return false;
	}
	
	size_t stReturn = m_Buffer.stRead(reinterpret_cast< u_int8_t * >(&u32Value), sizeof(u_int32_t));
	
//~ 	std::cout << "\tValue = " << u32Value << std::endl;

	return stReturn == sizeof(u_int32_t);
}

bool Network::BufferReader::bRead(int32_t & s32Value)
{
//~ 	std::cout << "Reading int32_t from the circular buffer." << std::endl;
	
	if(m_Buffer.stGetSize() < sizeof(int32_t))
	{
		return false;
	}
	
	size_t stReturn = m_Buffer.stRead(reinterpret_cast< u_int8_t * >(&s32Value), sizeof(int32_t));
	
//~ 	std::cout << "\tValue = " << s32Value << std::endl;

	return stReturn == sizeof(int32_t);
}

bool Network::BufferReader::bRead(Glib::ustring & sValue)
{
//~ 	std::cout << "Reading Glib::ustring from the circular buffer." << std::endl;
	
	u_int8_t u8Char = 0;
	
	do
	{
		if(m_Buffer.stRead(&u8Char, sizeof(u_int8_t)) == sizeof(u_int8_t))
		{
//~ 			std::cout << "\tRead '" << std::hex << static_cast< u_int32_t >(u8Char) << std::dec << "'" << std::endl;
			if(u8Char == 0)
			{
				break;
			}
			else
			{
				sValue += static_cast< char >(u8Char);
			}
		}
		else
		{
			return false;
		}
	} while(true);
//~ 	std::cout << "\tValue = " << sValue << std::endl;
	
	return true;
}

bool Network::BufferReader::bRead(void * pvMemory, std::size_t stSize)
{
	if(m_Buffer.stGetSize() < stSize)
	{
		return false;
	}
	
	return m_Buffer.stRead(reinterpret_cast< u_int8_t * >(pvMemory), stSize) == stSize;
}
