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

#ifndef SERVER_H
#define SERVER_H

#include <fcntl.h>
#include <netinet/ip.h>
#include <sys/socket.h>

#include <boost/shared_ptr.hpp>

#include "Socket.h"

namespace Network
{
	template < typename SocketType >
	class Server : public Network::Socket
	{
	public:
		// constructor
		Server(boost::shared_ptr< Network::MessageFactory > MessageFactory) :
			m_MessageFactory(MessageFactory)
		{
		}
		
		// connection
		void vOpen(const Network::port_t & ServicePort)
		{
			if(bIsOpen() == true)
			{
				return;
			}
			// -1 instead of g_iInvalidSocket because the two things are not connected to each other
			if((m_iSocket = ::socket(PF_INET, SOCK_STREAM, 0)) == -1)
			{
				m_iSocket = g_iInvalidSocket;
				vGetError();
				
				return;
			}
			
			sockaddr SocketAddress;
			sockaddr_in & SocketInformation = reinterpret_cast< sockaddr_in & >(SocketAddress);
			
			SocketInformation.sin_family = PF_INET;
			SocketInformation.sin_port = htons(ServicePort);
			SocketInformation.sin_addr.s_addr = INADDR_ANY;
			if(::bind(m_iSocket, &SocketAddress, sizeof(sockaddr_in)) == -1)
			{
				::close(m_iSocket);
				m_iSocket = g_iInvalidSocket;
				vGetError();
				
				return;
			}
			if(::listen(m_iSocket, 5) == -1)
			{
				::close(m_iSocket);
				m_iSocket = g_iInvalidSocket;
				vGetError();
				
				return;
			}
			if(::fcntl(m_iSocket, F_SETFL, O_NONBLOCK) == -1)
			{
				::close(m_iSocket);
				m_iSocket = g_iInvalidSocket;
				vGetError();
				
				return;
			}
			vMonitor();
		}
		
		// signal is fired when a new connection has been established via the socket
		sigc::signal< void, boost::shared_ptr< SocketType > > Accepted;
	protected:
		virtual void vOnIn(void)
		{
			Accepted(boost::shared_ptr< SocketType >(new SocketType(accept(m_iSocket, 0, 0), m_MessageFactory)));
		}
	public:
		boost::shared_ptr< Network::MessageFactory > m_MessageFactory;
	};
}

#endif
