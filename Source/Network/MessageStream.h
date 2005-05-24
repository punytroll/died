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

#ifndef NETWORK_MESSAGESTREAM_H
#define NETWORK_MESSAGESTREAM_H

#include <deque>

#include "Value.h"
#include "MessageFactory.h"
#include "NotifyValue.h"
#include "Stream.h"

namespace Network
{
	class MessageStream : public Network::Stream, public std::deque< boost::shared_ptr< Network::BasicMessage > >
	{
	public:
		typedef std::deque< boost::shared_ptr< Network::BasicMessage > > iterator;
		
		MessageStream(boost::shared_ptr< Network::MessageFactory > MessageFactory);
		MessageStream(int iSocket, boost::shared_ptr< Network::MessageFactory > MessageFactory);
		MessageStream & operator>>(boost::shared_ptr< Network::BasicMessage > Message);
		MessageStream & operator<<(const Network::BasicMessage & Message);
		
		sigc::signal< void > OnDisconnected;
		
		sigc::signal< void > MessageBegin;
		sigc::signal< void > MessageReady;
	private:
		// no copy and assignment for streams
		MessageStream(const MessageStream & MessageStream);
		MessageStream & operator=(const MessageStream & MessageStream);
		
		// override vOnDisconnected from Network::Socket
		void vOnDisconnected(void);
		
		void vMessageTypeReady(void);
		void vMessageReady(void);
		
		boost::shared_ptr< Network::MessageFactory > m_MessageFactory;
		Network::Value< u_int32_t > m_MessageType;
		Network::NotifyValue m_NotifyValue;
	};
}

#endif
