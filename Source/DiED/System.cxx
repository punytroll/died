#include "System.h"

#include <iostream>

#include "../Common.h"
#include "Client.h"
#include "Messages.h"

DiED::System::System(void) :
	m_MessageFactory(new DiED::MessageFactory())
{
}

DiED::System::~System(void)
{
}

void DiED::System::vSetClientFactory(boost::shared_ptr< DiED::ClientFactory > ClientFactory)
{
	m_ClientFactory = ClientFactory;
	if(m_ClientFactory)
	{
		m_ClientFactory->vSetMessageFactory(m_MessageFactory);
		m_Server.vSetSocketFactory(m_ClientFactory);
	}
}

void DiED::System::vSetExternalEnvironment(DiED::ExternalEnvironment * pExternalEnvironment)
{
	m_pExternalEnvironment = pExternalEnvironment;
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
	DiED::Client * pClient = new DiED::Client(m_MessageFactory, *this);
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
		pClient->operator<<(DiED::PingMessage());
	}
	m_Clients.push_back(Client);
}

void DiED::System::vInput(const Glib::ustring & sString)
{
	DiED::InputMessage Message(sString);
	
	vSendMessage(Message);
}

void DiED::System::vInsertText(const Glib::ustring & sString, int iLine, int iCharacter)
{
	if(m_pExternalEnvironment != 0)
	{
		m_pExternalEnvironment->vInsertText(sString, iLine, iCharacter);
	}
}

void DiED::System::vSendMessage(Network::BasicMessage & Message)
{
	std::vector< boost::shared_ptr< Network::Socket > >::iterator iClient = m_Clients.begin();
	
	while(iClient != m_Clients.end())
	{
		Network::MessageStream & Stream(dynamic_cast< Network::MessageStream & >(**iClient));
		
		Stream << Message;
		++iClient;
	}
}

void DiED::System::vAccepted(boost::shared_ptr< Network::Socket > Client)
{
	m_Clients.push_back(Client);
}
