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

#include "BasicMessage.h"

#include <iostream>

Network::BasicMessage::BasicMessage(u_int32_t u32Type) :
	m_Type(u32Type)
{
//~ 	std::cout << "[Network/BasicMessage]: Created BasicMessage [Type = " << u32Type << "]" << std::endl;
}

Network::BasicMessage::~BasicMessage(void)
{
//~ 	std::cout << "[Network/BasicMessage]: Deleted BasicMessage." << std::endl;
}

void Network::BasicMessage::vRegisterValue(Network::BasicValue & Value)
{
	m_Values.push_back(boost::ref(Value));
}

void Network::BasicMessage::vReadFrom(Network::Stream & Stream)
{
	std::vector< boost::reference_wrapper< Network::BasicValue > >::iterator iValue(m_Values.begin());
	
	while(iValue != m_Values.end())
	{
		Stream >> *iValue;
		++iValue;
	}
}

void Network::BasicMessage::vWriteTo(Network::Stream & Stream) const
{
	Stream << m_Type;
	
	std::vector< boost::reference_wrapper< Network::BasicValue > >::const_iterator iValue(m_Values.begin());
	
	while(iValue != m_Values.end())
	{
		Stream << *iValue;
		++iValue;
	}
}

size_t Network::BasicMessage::stGetSize(void) const
{
	std::vector< boost::reference_wrapper< Network::BasicValue > >::const_iterator iValue(m_Values.begin());
	size_t stSize = m_Type.stGetSize();
	
	while(iValue != m_Values.end())
	{
		stSize += iValue->get().stGetSize();
		++iValue;
	}
	
	return stSize;
}

Network::BasicMessage::type_t Network::BasicMessage::GetType(void) const
{
	return m_Type;
}
