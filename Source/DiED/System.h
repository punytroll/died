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
		DiED::clientid_t GetLocalClientID(void);
		
		// functions which will send messages to the connected clients
		void vInput(const Glib::ustring & sString);
		
		// implementation of the InternalEnvironment interface
		virtual void vConnectionRequest(DiED::User & User, const DiED::clientid_t & ClientID);
		virtual void vConnectionAccept(DiED::User & User, const DiED::clientid_t & AccepterClientID, const DiED::clientid_t & RequesterClientID);
		virtual void vKnownClients(DiED::User & User, const DiED::messageid_t & MessageID, const std::vector< DiED::clientid_t > & ConnectedClientIDs, const std::vector< DiED::clientid_t > & DisconnectedClientIDs);
		virtual void vClientsRegistered(DiED::User & User, const DiED::messageid_t & MessageID);
		virtual void vConnectionEstablished(DiED::User & User, const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort);
		virtual void vConnectionLost(DiED::User & User, const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort);
		virtual void vInsertText(DiED::User & User, const Glib::ustring & sString);
		
		// helper functions
		virtual std::vector< DiED::clientid_t > GetConnectedClientIDs(void);
		virtual std::vector< DiED::clientid_t > GetDisconnectedClientIDs(void);
		virtual DiED::User::Status GetStatus(const DiED::clientid_t & ClientID1, const DiED::clientid_t & ClientID2);
		virtual DiED::Client * pGetClient(const DiED::clientid_t & ClientID);
		
		// signals
		sigc::signal< void, DiED::Client & > ClientConnected;
	protected:
		void vSendMessage(boost::shared_ptr< DiED::BasicMessage > Message, bool bSendOnlyToConnected = false);
		void vInsertText(DiED::User & User, const Glib::ustring & sString, bool bWriteToEnvironment);
		virtual void vAccepted(boost::shared_ptr< Network::MessageStream > MessageStream);
	private:
		virtual void vSetStatus(const DiED::clientid_t & ClientID1, const DiED::clientid_t & ClientID2, const DiED::User::Status & Status);
		
		boost::shared_ptr< DiED::Client > GetNewPreliminaryClient(void);
		boost::shared_ptr< DiED::Client > RegisterClient(boost::shared_ptr< DiED::Client > Client, const DiED::clientid_t & _ClientID = 0);
		
		// private callbacks
		void vPongReceived(boost::shared_ptr< DiED::Client > Client);
		
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
