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

#ifndef CLIENT_H
#define CLIENT_H

#include <deque>

#include <boost/ref.hpp>
#include <boost/shared_ptr.hpp>

#include "BasicValue.h"
#include "CircularBufferStd.h"
#include "Socket.h"

namespace Network
{
	class Stream : public Network::Socket
	{
	public:
		// constructors and destructors
		Stream(void);
		Stream(int iSocket);
		
		// connection
		void vOpen(const Network::address_t & ConnectAddress, const Network::port_t & ConnectPort);
		
		// input/output streaming of Values
		Stream & operator>>(Network::BasicValue & Value);
		Stream & operator<<(const Network::BasicValue & Value);
		
		// signals
		sigc::signal< void, size_t > BytesSent;
	protected:
		void vRead(Network::BasicValue & Value);
		virtual void vOnIn(void);
		virtual void vOnOut(void);
	private:
		u_int8_t * m_pu8Buffer;
		Network::CircularBuffer m_IBuffer;
		Network::CircularBuffer m_OBuffer;
		bool m_bConnectingInProgress;
		std::deque< boost::reference_wrapper< Network::BasicValue > > m_Values;
	};
}

#endif
