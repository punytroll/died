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
		virtual void vInsertText(DiED::User & User, const Glib::ustring & sString);
		
		// signals
		sigc::signal< void, DiED::Client & > ClientConnected;
	protected:
		void vSendMessage(Network::BasicMessage & Message);
		void vInsertText(DiED::User & User, const Glib::ustring & sString, bool bWriteToEnvironment);
		virtual void vAccepted(boost::shared_ptr< Network::Socket > Client);
	private:
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
