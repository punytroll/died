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
		virtual void vInsertText(const Glib::ustring & sString) = 0;
		virtual void vConnectionRequest(const DiED::clientid_t & ClientID, const Network::port_t & Port) = 0;
		virtual void vConnectionAccept(const DiED::clientid_t & LocalClientID, const DiED::clientid_t & RemoteClientID) = 0;
		virtual void vKnownClients(const DiED::messageid_t & MessageID, const std::vector< DiED::clientid_t > & ConnectedClientIDs, const std::vector< DiED::clientid_t > & DisconnectedClientIDs) = 0;
		virtual void vClientsRegistered(const DiED::messageid_t & MessageID) = 0;
		virtual void vConnectionEstablished(const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort) = 0;
	};
}

#endif
