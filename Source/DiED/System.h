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
		
		// functions which will send messages to the connected clients
		void vInput(const Glib::ustring & sString);
		
		// implementation of the InternalEnvironment interface
		virtual void vConnectionRequest(DiED::User & User, const DiED::clientid_t & ClientID);
		virtual void vConnectionAccept(DiED::User & User, const DiED::clientid_t & LocalClientID, const DiED::clientid_t & RemoteClientID);
		virtual void vKnownClients(DiED::User & User, const DiED::messageid_t & MessageID, const std::vector< DiED::clientid_t > & ConnectedClientIDs, const std::vector< DiED::clientid_t > & DisconnectedClientIDs);
		virtual void vClientsRegistered(DiED::User & User, const DiED::messageid_t & MessageID);
		virtual void vConnectionEstablished(DiED::User & User, const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort);
		virtual void vInsertText(DiED::User & User, const Glib::ustring & sString);
		virtual std::vector< DiED::clientid_t > GetConnectedClientIDs(void);
		virtual std::vector< DiED::clientid_t > GetDisconnectedClientIDs(void);
		
		// signals
		sigc::signal< void, DiED::Client & > ClientConnected;
	protected:
		void vSendMessage(boost::shared_ptr< Network::BasicMessage > Message);
		void vInsertText(DiED::User & User, const Glib::ustring & sString, bool bWriteToEnvironment);
		virtual void vAccepted(boost::shared_ptr< Network::MessageStream > MessageStream);
	private:
		void vAssignClientID(DiED::Client & Client, const DiED::clientid_t & ClientID);
		boost::shared_ptr< DiED::Client > RegisterClient(const DiED::clientid_t & ClientID = 0);
		boost::shared_ptr< DiED::MessageFactory > m_MessageFactory;
		boost::shared_ptr< DiED::ClientFactory > m_ClientFactory;
		boost::shared_ptr< DiED::Client > m_Client;
		DiED::Server m_Server;
		std::multimap< DiED::clientid_t, boost::shared_ptr< DiED::Client > > m_Clients;
		DiED::ExternalEnvironment * m_pExternalEnvironment;
		Network::port_t m_ServicePort;
	};
}

#endif
