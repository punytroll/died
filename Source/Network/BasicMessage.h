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

#ifndef NETWORK_MESSAGE_H
#define NETWORK_MESSAGE_H

#include "Stream.h"
#include "Value.h"

namespace Network
{
	class BasicMessage
	{
	public:
		typedef u_int32_t type_t;
		
		// constructor and destructor
		BasicMessage(Network::BasicMessage::type_t Type);
		virtual ~BasicMessage(void);
		
		// query functions
		Network::BasicMessage::type_t GetType(void) const;
		size_t stGetSize(void) const;
		
		// stream operations
		void vReadFrom(Network::Stream & Stream);
		void vWriteTo(Network::Stream & Stream) const;
	protected:
		void vRegisterValue(Network::BasicValue & BasicValue);
	private:
		Network::Value< Network::BasicMessage::type_t > m_Type;
		std::vector< boost::reference_wrapper< Network::BasicValue > > m_Values;
	};
}

#endif
