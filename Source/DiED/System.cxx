#include "System.h"

#include <iostream>

#include "../Common.h"
#include "Client.h"
#include "Messages.h"

DiED::System::System(void) :
	m_MessageFactory(),
	m_ClientFactory(m_MessageFactory),
	m_Server(m_ClientFactory)
{
}

DiED::System::~System(void)
{
}

void DiED::System::vAccepted(boost::shared_ptr< Network::Socket > Client)
{
	m_Clients.push_back(Client);
}

bool DiED::System::bListen(u_int16_t u16ServicePort)
{
	m_Server.vOpen(u16ServicePort);
	if(m_Server.bIsOpen() == false)
	{
		std::cerr << "[Server]: Error setting up the server. [" << sErrorCodeToString(m_Server.iGetError()) << "]." << std::endl;
		
		return false;
	}
	else
	{
		m_Server.Accepted.connect(sigc::mem_fun(*this, &DiED::System::vAccepted));
	}
	
	return true;
}

bool DiED::System::bConnectTo(const std::string & sConnectAddress, u_int16_t u16ConnectPort)
{
	DiED::Client * pClient = new DiED::Client(m_MessageFactory);
	boost::shared_ptr< Network::Socket > Client(pClient);
	
	pClient->vOpen(sConnectAddress, u16ConnectPort);
	if(pClient->bIsOpen() == false)
	{
		std::cout << "[Client]: Connection failed." << std::endl;
	}
	else
	{
		std::cout << "[Client]: Connected to " << sConnectAddress << ':' << u16ConnectPort << std::endl;
		
		pClient->operator<<(DiED::HelloMessage());
	}
	m_Clients.push_back(Client);
}
