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
		virtual void vHandleKnownClients(const DiED::messageid_t & MessageID, const std::vector< DiED::clientid_t > & ConnectedClientIDs, const std::vector< DiED::clientid_t > & DisconnectedClientIDs) = 0;
		virtual void vHandleClientsRegistered(const DiED::messageid_t & MessageID) = 0;
		virtual void vHandleConnectionEstablished(const DiED::messageid_t & MessageID, const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort) = 0;
		virtual void vHandleConnectionLost(const DiED::messageid_t & MessageID, const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort) = 0;
		virtual void vHandleStatusConfirm(const DiED::messageid_t & MessageID) = 0;
		virtual void vHandlePing(const DiED::messageid_t & PingID) = 0;
		virtual void vHandlePong(const DiED::messageid_t & PingID) = 0;
		virtual void vHandleEvent(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID, boost::shared_ptr< DiED::EventAction > EventAction) = 0;
		virtual void vHandleEventReceived(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID) = 0;
		
		virtual void vHandlePingConfirmationTimeout(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters) = 0;
		virtual void vHandleEventConfirmationTimeout(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters) = 0;
	};
}

#endif
