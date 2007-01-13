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

#ifndef NETWORK_SOCKET_H
#define NETWORK_SOCKET_H

#include <sigc++/sigc++.h>

#include <glibmm/iochannel.h>

namespace Network
{
	extern const int g_iInvalidSocket;
	
	typedef Glib::ustring address_t;
	typedef u_int16_t port_t;
	
	/**
	 * @brief A class encapsulating a socket.
	 * 
	 * This class provides C++ access to standard UNIX sockets.
	 **/
	class Socket : virtual public sigc::trackable
	{
	public:
		enum Status
		{
			DISCONNECTED,
			CONNECTING,
			CONNECTED,
			DISCONNECTING,
			LISTENING
		};
		
		Socket(void);
		Socket(int iSocket);
		virtual ~Socket(void);
		bool bIsOpen(void) const;
		int iGetError(void) const;
		void vClose(void);
		void vSetSocket(int iSocket);
		Network::address_t GetAddress(void);
		Network::port_t GetPort(void);
		
		/**
		 * @brief Opens a listening socket on a specified port.
		 * @param ServicePort The port that the service should be listening on.
		 * 
		 * This call performs a sequence of actions necessary for setting up a listening port.
		 **/
		void vOpen(const Network::port_t & ServicePort);
	protected:
		void vMonitor(void);
		void vRequestOnOut(void);
		void vIgnoreOnOut(void);
		void vGetError(void);
		
		// virtual functions to make inherited classes act on socket events
		virtual void vOnConnecting(void);
		virtual void vOnConnected(void);
		virtual void vOnDisconnecting(void);
		
		/**
		 * @brief Virtual funtion that indicates that the socket has gone in DISCONNECTED state.
		 **/
		virtual void vOnDisconnected(void);
		virtual void vOnIn(void);
		virtual void vOnOut(void);
		
		int m_iSocket;
		int m_iError;
	private:
		bool bNotify(const Glib::IOCondition & Condition);
		Glib::RefPtr< Glib::IOSource > m_OSource;
	};
}

#endif
