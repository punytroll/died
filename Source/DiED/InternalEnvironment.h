#ifndef DIED_INTERNALENVIRONMENT_H
#define DIED_INTERNALENVIRONMENT_H

#include <Network/MessageFactory.h>
#include <Common.h>

#include "User.h"

namespace DiED
{
	class InternalEnvironment
	{
	public:
		virtual boost::shared_ptr< Network::MessageFactory > GetMessageFactory(void) = 0;
		virtual void vInsertText(DiED::User & User, const Glib::ustring & sString) = 0;
		virtual void vConnectionRequest(DiED::User & User, const DiED::clientid_t & ClientID) = 0;
		virtual void vConnectionAccept(DiED::User & User, const DiED::clientid_t & LocalClientID, const DiED::clientid_t & RemoteClientID) = 0;
		virtual void vKnownClients(DiED::User & User, const DiED::messageid_t & MessageID, const std::vector< DiED::clientid_t > & ConnectedClientIDs, const std::vector< DiED::clientid_t > & DisconnectedClientIDs) = 0;
		virtual void vClientsRegistered(DiED::User & User, const DiED::messageid_t & MessageID) = 0;
		virtual void vConnectionEstablished(DiED::User & User, const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort) = 0;
		virtual void vConnectionLost(DiED::User & User, const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort) = 0;
		
		// helper functions
		virtual std::vector< DiED::clientid_t > GetConnectedClientIDs(void) = 0;
		virtual std::vector< DiED::clientid_t > GetDisconnectedClientIDs(void) = 0;
		virtual DiED::User::Status GetStatus(const DiED::clientid_t & ClientID1, const DiED::clientid_t & ClientID2 = 0) = 0;
		virtual void vSetStatus(const DiED::clientid_t & ClientID1, const DiED::clientid_t & ClientID2, const DiED::User::Status & Status) = 0;
		virtual DiED::User * pGetClient(const DiED::clientid_t & ClientID) = 0;
	};
}

#endif
