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

#ifndef DIED_MESSAGETARGET_H
#define DIED_MESSAGETARGET_H

#include <Common.h>

//~ #include "EventAction.h"
#include "User.h"

namespace DiED
{
	class EventAction;
	
	class MessageTarget
	{
	public:
		virtual void vHandleConnectionRequest(const DiED::clientid_t & ClientID, const Network::port_t & Port) = 0;
		virtual void vHandleConnectionAccept(const DiED::clientid_t & AccepterClientID, const DiED::clientid_t & RequesterClientID) = 0;
		virtual void vHandleSessionSnapshot(const DiED::messageid_t & MessageID, const std::vector< DiED::ClientInfo > & ClientInfos, bool bDocumentValid, const Glib::ustring & sDocument) = 0;
		virtual void vHandleClientsRegistered(const DiED::messageid_t & MessageID) = 0;
		virtual void vHandleConnectionEstablished(const DiED::messageid_t & MessageID, const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort) = 0;
		virtual void vHandleConnectionLost(const DiED::messageid_t & MessageID, const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort) = 0;
		virtual void vHandleStatusConfirm(const DiED::messageid_t & MessageID) = 0;
		virtual void vHandlePing(const DiED::messageid_t & PingID) = 0;
		virtual void vHandlePong(const DiED::messageid_t & PingID) = 0;
		virtual void vHandleEvent(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID, boost::shared_ptr< DiED::EventAction > EventAction) = 0;
		virtual void vHandleEventReceived(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID) = 0;
		
		virtual void vHandleConnectionRequestConfirmationTimeout(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters) = 0;
		virtual void vHandleSessionSnapshotConfirmationTimeout(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters) = 0;
		virtual void vHandleConnectionEstablishedConfirmationTimeout(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters) = 0;
		virtual void vHandleConnectionLostConfirmationTimeout(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters) = 0;
		virtual void vHandlePingConfirmationTimeout(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters) = 0;
		virtual void vHandleEventConfirmationTimeout(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters) = 0;
	};
}

#endif
