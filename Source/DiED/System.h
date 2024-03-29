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

#ifndef DIED_SYSTEM_H
#define DIED_SYSTEM_H

#include <map>

#include "ClientFactory.h"
#include "ExternalEnvironment.h"
#include "MessageFactory.h"
#include "Server.h"

namespace DiED
{
	class System : public DiED::InternalEnvironment
	{
	public:
		System(void);
		virtual ~System(void);
		void vSetClientFactory(boost::shared_ptr< DiED::ClientFactory > ClientFactory);
		void vSetExternalEnvironment(DiED::ExternalEnvironment * pExternalEnvironment);
		virtual boost::shared_ptr< Network::MessageFactory > GetMessageFactory(void);
		
		// functions which will setup the server or the connection to a network
		bool bListen(const Network::port_t & ServicePort);
		bool bConnectTo(const Network::address_t & ConnectAddress, const Network::port_t & ConnectPort);
		void vTryReconnect(const DiED::clientid_t & ClientID);
		DiED::clientid_t GetLocalClientID(void);
		
		// functions which will send messages to the connected clients
		void vInsert(const Glib::ustring & sString);
		void vDelete(int iLine, int iCharacter);
		void vPosition(int iLine, int iCharacter);
		void vLogOut(void);
		
		// helper functions
		virtual std::set< DiED::clientid_t > GetConnectedClientIDs(void);
		virtual std::set< DiED::clientid_t > GetDisconnectedClientIDs(void);
		virtual std::vector< DiED::ClientInfo > GetClientInfos(void);
		virtual DiED::clientstatus_t GetStatus(const DiED::clientid_t & ClientID1, const DiED::clientid_t & ClientID2);
		virtual DiED::Client * pGetClient(const DiED::clientid_t & ClientID);
		
		// "Announce" functions will forward the message in question to all connected clients
		virtual void vAnnounceConnectionLost(const DiED::clientid_t & ClientID);
		virtual void vAnnounceLogOutNotification(const DiED::messageid_t & EventID);
		virtual void vAnnounceMessage(boost::shared_ptr< DiED::BasicMessage > Message);
		
		// signals
		sigc::signal< void, DiED::Client & > ClientConnected;
	protected:
		void vSendMessage(boost::shared_ptr< DiED::BasicMessage > Message, bool bSendOnlyToConnected = false);
		void vInsert(DiED::User & User, const Glib::ustring & sString, bool bWriteToEnvironment);
		void vDelete(DiED::User & User, int iLineRelative, int iCharacterRelative, bool bForwardToEnvironment);
		virtual void vAccepted(boost::shared_ptr< Network::MessageStream > MessageStream);
		void vClientStatusChanged(const DiED::clientid_t & ClientID, const DiED::clientstatus_t & Status);
		bool bTryReconnect(const DiED::clientid_t & ClientID, bool bAnnounceConnectionLost);
	private:
		// implementation of the InternalEnvironment interface
		virtual void vHandleConnectionRequest(DiED::User & User, const DiED::clientid_t & ClientID, const Network::port_t & Port);
		virtual void vHandleConnectionAccept(DiED::User & User, const DiED::clientid_t & AccepterClientID, const DiED::clientid_t & RequesterClientID);
		virtual void vHandleSessionSnapshot(DiED::User & User, const DiED::messageid_t & MessageID, const std::vector< ClientInfo > & ClientInfos, bool bDocumentValid, const Glib::ustring & sDocument);
		virtual void vHandleClientsRegistered(DiED::User & User, const DiED::messageid_t & MessageID);
		virtual void vHandleConnectionEstablished(DiED::User & User, const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort);
		virtual void vHandleConnectionLost(DiED::User & User, const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort);
		virtual void vHandleInsert(DiED::User & User, const Glib::ustring & sString);
		virtual void vHandleDelete(DiED::User & User, int iLineRelative, int iCharacterRelative, int iLineAbsolute, int iCharacterAbsolute);
		virtual void vHandlePosition(DiED::User & User, int iLineRelative, int iCharacterRelative, int iLineAbsolute, int iCharacterAbsolute);
		virtual void vHandleLogOutNotification(DiED::User & User);
		
		// status manipulation ... TODO@ replace this with a symmetric map of references.
		virtual void vSetStatus(const DiED::clientid_t & ClientID1, const DiED::clientid_t & ClientID2, const DiED::clientstatus_t & Status);
		
		boost::shared_ptr< DiED::Client > GetNewPreliminaryClient(void);
		boost::shared_ptr< DiED::Client > RegisterClient(boost::shared_ptr< DiED::Client > Client, const DiED::clientid_t & _ClientID = 0);
		
		// private callbacks
		void vPongTimeout(boost::shared_ptr< DiED::Client > Client);
		void vPongTimeoutOnConnectionEstablished(boost::shared_ptr< DiED::Client > Client, const Network::address_t & ClientAddress, const Network::port_t & ClientPort);
		void vPongOnConnectionRequest(boost::reference_wrapper< DiED::User > User, const Network::port_t & ListenPort);
		void vPongTimeoutOnConnectionRequest(boost::reference_wrapper< DiED::User > User, const DiED::clientid_t & ClientID, const Network::port_t & ListenPort);
		
		boost::shared_ptr< DiED::MessageFactory > m_MessageFactory;
		boost::shared_ptr< DiED::ClientFactory > m_ClientFactory;
		boost::shared_ptr< DiED::Client > m_Client;
		DiED::Server m_Server;
		std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > > m_Clients;
		std::map< DiED::User * , boost::shared_ptr< DiED::Client > > m_PreliminaryClients;
		DiED::ExternalEnvironment * m_pExternalEnvironment;
		Network::port_t m_ServicePort;
	};
}

#endif
