#ifndef DIED_SYSTEM_H
#define DIED_SYSTEM_H

#include "ClientFactory.h"
#include "MessageFactory.h"
#include "Server.h"

namespace DiED
{
	class System
	{
	public:
		System(void);
		virtual ~System(void);
		bool bListen(u_int16_t u16ServicePort);
		bool bConnectTo(const std::string & sConnectAddress, u_int16_t u16ConnectPort);
	protected:
		virtual void vAccepted(boost::shared_ptr< Network::Socket > Client);
	private:
		DiED::MessageFactory m_MessageFactory;
		DiED::ClientFactory m_ClientFactory;
		DiED::Server m_Server;
		std::vector< boost::shared_ptr< Network::Socket > > m_Clients;
	};
}

#endif
