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

#include "MessageStream.h"

#include <iostream>

Network::MessageStream::MessageStream(boost::shared_ptr< Network::MessageFactory > MessageFactory) :
	m_MessageFactory(MessageFactory)
{
	Network::Stream & Stream(*this);
	
	Stream >> m_MessageType;
	m_MessageType.Ready.connect(sigc::mem_fun(*this, &Network::MessageStream::vMessageTypeReady));
	m_NotifyValue.Ready.connect(sigc::mem_fun(*this, &Network::MessageStream::vMessageReady));
}

Network::MessageStream::MessageStream(int iSocket, boost::shared_ptr< Network::MessageFactory > MessageFactory) :
	Stream(iSocket),
	m_MessageFactory(MessageFactory)
{
	Network::Stream & Stream(*this);
	
	Stream >> m_MessageType;
	m_MessageType.Ready.connect(sigc::mem_fun(*this, &Network::MessageStream::vMessageTypeReady));
	m_NotifyValue.Ready.connect(sigc::mem_fun(*this, &Network::MessageStream::vMessageReady));
}

Network::MessageStream & Network::MessageStream::operator>>(boost::shared_ptr< Network::BasicMessage > Message)
{
	push_back(Message);
	MessageBegin();
//~ 	std::cout << "-> ReadFrom" << std::endl;
	Message->vReadFrom(*this);
//~ 	std::cout << "<- ReadFrom" << std::endl;
	
	return *this;
}

Network::MessageStream & Network::MessageStream::operator<<(const Network::BasicMessage & Message)
{
//~ 	if(Message.bIsForSending() == true)
//~ 	{
		Message.vWriteTo(*this);
//~ 		std::cout << "[Network/MessageStream]: Requesting OnOut." << std::endl;
		vRequestOnOut();
//~ 	}
//~ 	else
//~ 	{
//~ 		std::cout << "[Network/MessageStream]: Trying to send Message [" << Message.GetType() << "] which is not for sending." << std::endl;
//~ 	}
	
	return *this;
}

void Network::MessageStream::vOnDisconnected(void)
{
	Network::Stream::vOnDisconnected();
	OnDisconnected();
}

void Network::MessageStream::vMessageTypeReady(void)
{
//~ 	std::cout << "MessageType " << m_MessageType << " read." << std::endl;
	*this >> m_MessageFactory->GetMessage(m_MessageType);
	Stream::operator>>(m_NotifyValue);
	Stream::operator>>(m_MessageType);
}

void Network::MessageStream::vMessageReady(void)
{
	if(size() > 0)
	{
		MessageReady();
	}
}
