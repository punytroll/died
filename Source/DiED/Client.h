#ifndef DIED_CLIENT_H
#define DIED_CLIENT_H

#include <Network/MessageStream.h>
#include <Common.h>

#include "InternalEnvironment.h"

namespace DiED
{
	class Client : public DiED::User
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
		
		void vInsertText(const Glib::ustring & sString);
		void vConnectionRequest(const DiED::clientid_t & ClientID, const Network::port_t & Port);
		void vConnectionAccept(const DiED::clientid_t & LocalClientID, const DiED::clientid_t & RemoteClientID);
		void vKnownClients(const DiED::messageid_t & MessageID, const std::vector< DiED::clientid_t > & ConnectedClientIDs, const std::vector< DiED::clientid_t > & DisconnectedClientIDs);
		void vClientsRegistered(const DiED::messageid_t & MessageID);
		void vConnectionEstablished(const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort);
		
		// messages
		DiED::Client & operator<<(boost::shared_ptr< Network::BasicMessage > Message);
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
	private:
		DiED::InternalEnvironment & m_InternalEnvironment;
	protected:
		boost::shared_ptr< Network::MessageStream > m_MessageStream;
	private:
		Network::port_t m_Port;
		sigc::connection m_BytesSentConnection;
		sigc::connection m_MessageBeginConnection;
		sigc::connection m_MessageReadyConnection;
		std::deque< boost::shared_ptr< Network::BasicMessage > > m_MessageQueue;
	};
}

#endif
