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

#ifndef NETWORK_BASICVALUE_H
#define NETWORK_BASICVALUE_H

#include <boost/shared_ptr.hpp>

#include <sigc++/signal.h>

#include "BasicReader.h"
#include "BasicWriter.h"

namespace Network
{
	class BasicValue
	{
	public:
		BasicValue(void);
		virtual ~BasicValue(void);
		bool bIsReady(void) const;
		virtual size_t stGetSize(void) const;
		virtual void vReadFrom(Network::BasicReader & Reader) = 0;
		virtual void vWriteTo(Network::BasicWriter & Writer) const = 0;
		
		sigc::signal< void > Ready;
	protected:
		void vSetReady(bool bReady);
	private:
		bool m_bReady;
	};
}

#endif
