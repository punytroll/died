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
		
		// messages
		void vPing(sigc::slot< void > PongSlot);
		DiED::Client & operator<<(boost::shared_ptr< DiED::BasicMessage > Message);
		virtual void vExecuteTopMessage(void);
		
		u_int32_t m_u32KnownClientsMessageID;
		
		//signals
		sigc::signal< void > MessageStreamSet;
	protected:
		// callbacks from template pattern
//~ 		virtual void vOnConnect(void);
//~ 		virtual void vOnDisconnect(void);
		virtual void vOnMessageReady(void);
		virtual void vOnMessageBegin(void);
		virtual void vOnMessageExecuted(void);
		void vBytesSent(size_t stSize);
		
		// message handler
		virtual void vInsertText(const Glib::ustring & sString);
		virtual void vConnectionRequest(const DiED::clientid_t & ClientID, const Network::port_t & Port);
		virtual void vConnectionAccept(const DiED::clientid_t & LocalClientID, const DiED::clientid_t & RemoteClientID);
		virtual void vKnownClients(const DiED::messageid_t & MessageID, const std::vector< DiED::clientid_t > & ConnectedClientIDs, const std::vector< DiED::clientid_t > & DisconnectedClientIDs);
		virtual void vClientsRegistered(const DiED::messageid_t & MessageID);
		virtual void vConnectionEstablished(const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort);
		virtual void vConnectionLost(const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort);
		virtual void vPing(const DiED::messageid_t & PingID);
		virtual void vPong(const DiED::messageid_t & PingID);
	private:
		DiED::InternalEnvironment & m_InternalEnvironment;
	protected:
		boost::shared_ptr< Network::MessageStream > m_MessageStream;
	private:
		Network::port_t m_Port;
		Network::address_t m_Address;
		sigc::connection m_BytesSentConnection;
		sigc::connection m_MessageBeginConnection;
		sigc::connection m_MessageReadyConnection;
		
		// message queues
		std::deque< boost::shared_ptr< DiED::BasicMessage > > m_AwaitingConfirmationQueue;
		std::deque< boost::shared_ptr< DiED::EventMessage > > m_EventQueue;
		std::map< DiED::messageid_t, boost::shared_ptr< sigc::signal< void > > > m_PongSignals;
	};
}

#endif
