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

#include "MessageFactory.h"

#include <iostream>

#include "Messages.h"

boost::shared_ptr< Network::BasicMessage > DiED::MessageFactory::GetMessage(u_int32_t u32Type)
{
	switch(u32Type)
	{
	case DiED::_ConnectionRequestMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::ConnectionRequestMessage());
		}
	case DiED::_ConnectionAcceptMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::ConnectionAcceptMessage());
		}
	case DiED::_SessionSnapshotMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::SessionSnapshotMessage());
		}
	case DiED::_ClientsRegisteredMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::ClientsRegisteredMessage());
		}
	case DiED::_ConnectionEstablishedMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::ConnectionEstablishedMessage());
		}
	case DiED::_ConnectionLostMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::ConnectionLostMessage());
		}
	case DiED::_StatusConfirmMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::StatusConfirmMessage());
		}
	case DiED::_PingMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::PingMessage());
		}
	case DiED::_PongMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::PongMessage());
		}
	case DiED::_EventReceivedMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::EventReceivedMessage());
		}
	case DiED::_InsertEvent:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::InsertEvent());
		}
	case DiED::_DeleteEvent:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::DeleteEvent());
		}
	case DiED::_PositionEvent:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::PositionEvent());
		}
	case DiED::_LogOutNotificationEvent:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::LogOutNotificationEvent());
		}
	case DiED::_NoMessage:
	default:
		{
			LOG(Warning, "DiED/MessageFactory", "Encountered undefined message '0x" << std::setw(8) << std::setfill('0') << std::hex << u32Type << std::dec << "'.");
			LOG(Warning, "DiED/MessageFactory", "  Trying to ignore.");
			
			return boost::shared_ptr< Network::BasicMessage >(new DiED::NoMessage());
		}
	}
}
