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

#ifndef DIED_INTERNALENVIRONMENT_H
#define DIED_INTERNALENVIRONMENT_H

#include <set>

#include <Network/MessageFactory.h>
#include <Common.h>

#include "Messages.h"
#include "User.h"

namespace DiED
{
	class InternalEnvironment
	{
	public:
		virtual ~InternalEnvironment(void)
		{
		}
		
		virtual boost::shared_ptr< Network::MessageFactory > GetMessageFactory(void) = 0;
		
		// messages and events for the environment
		virtual void vHandleConnectionRequest(DiED::User & User, const DiED::clientid_t & ClientID, const Network::port_t & Port) = 0;
		virtual void vHandleConnectionAccept(DiED::User & User, const DiED::clientid_t & AccepterClientID, const DiED::clientid_t & RequesterClientID) = 0;
		virtual void vHandleSessionSnapshot(DiED::User & User, const DiED::messageid_t & MessageID, const std::vector< ClientInfo > & ClientInfos, bool bDocumentValid, const Glib::ustring & sDocument) = 0;
		virtual void vHandleClientsRegistered(DiED::User & User, const DiED::messageid_t & MessageID) = 0;
		virtual void vHandleConnectionEstablished(DiED::User & User, const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort) = 0;
		virtual void vHandleConnectionLost(DiED::User & User, const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort) = 0;
		virtual void vHandleInsert(DiED::User & User, const Glib::ustring & sString) = 0;
		virtual void vHandleDelete(DiED::User & User, int iLineRelative, int iCharacterRelative, int iLineAbsolute, int iCharacterAbsolute) = 0;
		virtual void vHandlePosition(DiED::User & User, int iLineRelative, int iCharacterRelative, int iLineAbsolute, int iCharacterAbsolute) = 0;
		virtual void vHandleLogOutNotification(DiED::User & User) = 0;
		
		// helper functions
		virtual std::set< DiED::clientid_t > GetConnectedClientIDs(void) = 0;
		virtual std::set< DiED::clientid_t > GetDisconnectedClientIDs(void) = 0;
		virtual std::vector< DiED::ClientInfo > GetClientInfos(void) = 0;
		virtual DiED::clientstatus_t GetStatus(const DiED::clientid_t & ClientID1, const DiED::clientid_t & ClientID2 = 0) = 0;
		virtual void vSetStatus(const DiED::clientid_t & ClientID1, const DiED::clientid_t & ClientID2, const DiED::clientstatus_t & Status) = 0;
		virtual DiED::User * pGetClient(const DiED::clientid_t & ClientID) = 0;
		
		virtual void vAnnounceConnectionLost(const DiED::clientid_t & ClientID) = 0;
		virtual void vAnnounceMessage(boost::shared_ptr< DiED::BasicMessage > Message) = 0;
	};
}

#endif
