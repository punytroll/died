#ifndef DIED_SYSTEM_H
#define DIED_SYSTEM_H

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
		bool bListen(u_int16_t u16ServicePort);
		bool bConnectTo(const std::string & sConnectAddress, u_int16_t u16ConnectPort);
		// functions which will send messages to the connected clients
		void vInput(const Glib::ustring & sString);
		// implementation of the InternalEnvironment interface
		virtual void vInsertText(DiED::User & User, const Glib::ustring & sString);
	protected:
		void vSendMessage(Network::BasicMessage & Message);
		virtual void vAccepted(boost::shared_ptr< Network::Socket > Client);
	private:
		boost::shared_ptr< DiED::MessageFactory > m_MessageFactory;
		boost::shared_ptr< DiED::ClientFactory > m_ClientFactory;
		DiED::Server m_Server;
		std::vector< boost::shared_ptr< Network::Socket > > m_Clients;
		DiED::ExternalEnvironment * m_pExternalEnvironment;
	};
}

#endif
