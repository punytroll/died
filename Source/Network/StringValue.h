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

#ifndef NETWORK_STRINGVALUE_H
#define NETWORK_STRINGVALUE_H

#include "BasicValue.h"

namespace Network
{
	class StringValue : public Network::BasicValue
	{
	public:
		StringValue(void);
		StringValue(const Glib::ustring & Value);
		virtual size_t stGetSize(void) const;
		virtual void vReadFrom(boost::shared_ptr< Network::BasicReader > Reader);
		virtual void vWriteTo(boost::shared_ptr< Network::BasicWriter > Writer) const;
		operator const Glib::ustring &(void) const;
		const Glib::ustring & Get(void) const;
	protected:
		Glib::ustring m_String;
	};
}

#endif
