#ifndef DIED_CLIENT_H
#define DIED_CLIENT_H

#include <list>

#include <Network/MessageStream.h>
#include <Common.h>

#include "Messages.h"
#include "InternalEnvironment.h"
#include "MessageTarget.h"

namespace DiED
{
	class Client : public DiED::User, public DiED::MessageTarget
	{
	public:
		// constructor and destructor
		Client(DiED::InternalEnvironment & InternalEnvironment);
		virtual ~Client(void);
		
		// socket
		void vSetMessageStream(boost::shared_ptr< Network::MessageStream > MessageStream);
		boost::shared_ptr< Network::MessageStream > GetMessageStream(void);
		
		// connection stuff
		Network::port_t GetPort(void);
		Network::address_t GetAddress(void);
		void vSetPort(const Network::port_t & Port);
		
		// status
		virtual void vSetStatus(const DiED::clientid_t & ClientID, DiED::User::Status Status);
		
		// messages
		void vSend(boost::shared_ptr< DiED::BasicMessage > Message);
		void vConnectionRequest(const DiED::clientid_t & ClientID, const Network::port_t & Port);
		void vConnectionAccept(const DiED::clientid_t & LocalClientID, const DiED::clientid_t & RemoteClientID);
		void vKnownClients(bool bAskForKnownClients = false);
		void vClientsRegistered(const DiED::messageid_t & MessageID);
		void vConnectionEstablished(const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort);
		void vConnectionLost(const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort);
		void vPing(sigc::slot< void > PongTimeoutSlot);
		virtual void vExecuteTopMessage(void);
		
		//signals
		sigc::signal< void > MessageStreamSet;
	protected:
		virtual void vOnMessageReady(void);
		virtual void vOnMessageBegin(void);
		virtual void vOnMessageExecuted(void);
		void vOnDisconnected(void);
		void vBytesSent(size_t stSize);
		
		// message handler
		virtual void vHandleInsertText(const Glib::ustring & sString);
		virtual void vHandleConnectionRequest(const DiED::clientid_t & ClientID, const Network::port_t & Port);
		virtual void vHandleConnectionAccept(const DiED::clientid_t & LocalClientID, const DiED::clientid_t & RemoteClientID);
		virtual void vHandleKnownClients(const DiED::messageid_t & MessageID, const std::vector< DiED::clientid_t > & ConnectedClientIDs, const std::vector< DiED::clientid_t > & DisconnectedClientIDs);
		virtual void vHandleClientsRegistered(const DiED::messageid_t & MessageID);
		virtual void vHandleConnectionEstablished(const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort);
		virtual void vHandleConnectionLost(const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort);
		virtual void vHandlePing(const DiED::messageid_t & PingID);
		virtual void vHandlePong(const DiED::messageid_t & PingID);
		virtual void vHandleEvent(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID);
		virtual void vHandleEventReceived(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID);
	private:
		DiED::InternalEnvironment & m_InternalEnvironment;
	protected:
		boost::shared_ptr< Network::MessageStream > m_MessageStream;
	private:
		void vHandleAnswer(void);
		
		bool m_bRequestingConnection;
		Network::address_t m_Address;
		Network::port_t m_Port;
		sigc::connection m_BytesSentConnection;
		sigc::connection m_MessageBeginConnection;
		sigc::connection m_MessageReadyConnection;
		sigc::connection m_OnDisconnectedConnection;
		
		// message queues
		std::deque< boost::shared_ptr< DiED::BasicMessage > > m_AwaitingConfirmationQueue;
		std::deque< boost::shared_ptr< DiED::EventMessage > > m_EventQueue;
		std::map< DiED::messageid_t, boost::shared_ptr< sigc::signal< void > > > m_PongTimeoutSignals;
	};
}

#endif
