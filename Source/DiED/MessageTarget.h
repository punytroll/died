#ifndef DIED_MESSAGETARGET_H
#define DIED_MESSAGETARGET_H

#include <glibmm/ustring.h>

#include <Common.h>

#include "User.h"

namespace DiED
{
	class MessageTarget
	{
	public:
		virtual void vHandleInsertText(const Glib::ustring & sString) = 0;
		virtual void vHandleConnectionRequest(const DiED::clientid_t & ClientID, const Network::port_t & Port) = 0;
		virtual void vHandleConnectionAccept(const DiED::clientid_t & LocalClientID, const DiED::clientid_t & RemoteClientID) = 0;
		virtual void vHandleKnownClients(const DiED::messageid_t & MessageID, const std::vector< DiED::clientid_t > & ConnectedClientIDs, const std::vector< DiED::clientid_t > & DisconnectedClientIDs) = 0;
		virtual void vHandleClientsRegistered(const DiED::messageid_t & MessageID) = 0;
		virtual void vHandleConnectionEstablished(const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort) = 0;
		virtual void vHandleConnectionLost(const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort) = 0;
		virtual void vHandlePing(const DiED::messageid_t & PingID) = 0;
		virtual void vHandlePong(const DiED::messageid_t & PingID) = 0;
	};
}

#endif
