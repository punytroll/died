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

#include "StringValue.h"

Network::StringValue::StringValue(void) :
	m_String()
{
}

Network::StringValue::StringValue(const Glib::ustring & String) :
	m_String(String)
{
}

size_t Network::StringValue::stGetSize(void) const
{
	return m_String.bytes() + 1;
}

void Network::StringValue::vReadFrom(boost::shared_ptr< Network::BasicReader > Reader)
{
	vSetReady(Reader->bRead(m_String));
}

void Network::StringValue::vWriteTo(boost::shared_ptr< Network::BasicWriter > Writer) const
{
	Writer->bWrite(m_String);
}

Network::StringValue::operator const Glib::ustring &(void) const
{
	return m_String;
}

const Glib::ustring & Network::StringValue::Get(void) const
{
	return m_String;
}
