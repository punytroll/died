/* DiED - A distributed Editor.
 * Copyright (C) 2005 Hagen M�bius & Aram Altschudjian
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

#include "BasicValue.h"

Network::BasicValue::BasicValue(void) :
	m_bReady(false)
{
}

Network::BasicValue::~BasicValue(void)
{
}

bool Network::BasicValue::bIsReady(void) const
{
	return m_bReady;
}

size_t Network::BasicValue::stGetSize(void) const
{
	return 0;
}

void Network::BasicValue::vSetReady(bool bReady)
{
	m_bReady = bReady;
	if(bReady == true)
	{
		Ready();
	}
}
