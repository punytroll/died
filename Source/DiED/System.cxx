#include "System.h"

#include <iostream>

#include "../Common.h"
#include "Client.h"
#include "Messages.h"

DiED::System::System(void) :
	m_MessageFactory(new DiED::MessageFactory()),
	m_Client(new DiED::Client(*this)),
	m_ServicePort(0)
{
	m_Clients.insert(std::make_pair(m_Client->GetClientID(), m_Client));
}

DiED::System::~System(void)
{
}

void DiED::System::vSetExternalEnvironment(DiED::ExternalEnvironment * pExternalEnvironment)
{
	m_pExternalEnvironment = pExternalEnvironment;
}

boost::shared_ptr< Network::MessageFactory > DiED::System::GetMessageFactory(void)
{
	return m_MessageFactory;
}

void DiED::System::vSetClientFactory(boost::shared_ptr< DiED::ClientFactory > ClientFactory)
{
	m_ClientFactory = ClientFactory;
	m_Server.vSetSocketFactory(ClientFactory);
}

bool DiED::System::bListen(const Network::port_t & ServicePort)
{
	m_ServicePort = ServicePort;
	m_Server.vOpen(m_ServicePort);
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

bool DiED::System::bConnectTo(const Network::address_t & ConnectAddress, const Network::port_t & ConnectPort)
{
	boost::shared_ptr< DiED::Client > Client(boost::dynamic_pointer_cast< DiED::Client >(m_ClientFactory->GetSocket()));
	
	Client->vOpen(ConnectAddress, ConnectPort);
	if(Client->bIsOpen() == false)
	{
		std::cout << "[Client]: Connection failed." << std::endl;
		
		return false;
	}
	else
	{
		std::cout << "[Client]: Connected to " << ConnectAddress << ':' << ConnectPort << std::endl;
		Client->operator<<(DiED::ConnectMessage(m_Client->GetClientID(), m_ServicePort));
	}
	m_Clients.insert(std::make_pair(Client->GetClientID(), Client));
	
	return true;
}

void DiED::System::vInput(const Glib::ustring & sString)
{
	DiED::InputMessage Message(sString);
	
	vSendMessage(Message);
	vInsertText(*m_Client, sString, false);
}

void DiED::System::vInsertText(DiED::User & User, const Glib::ustring & sString)
{
	vInsertText(User, sString, true);
}

void DiED::System::vInsertText(DiED::User & User, const Glib::ustring & sString, bool bWriteToEnvironment)
{
	int iLine(User.iGetLine());
	int iCharacter(User.iGetCharacter());
	
	if((m_pExternalEnvironment != 0) && (bWriteToEnvironment == true))
	{
		m_pExternalEnvironment->vInsertText(sString, iLine, iCharacter);
	}
	
	int iDeltaLine = 0;
	int iDeltaCharacter = 0;
	
	for(Glib::ustring::size_type stI = 0; stI < sString.length(); ++stI)
	{
		if(sString[stI] == '\n')
		{
			++iDeltaLine;
			iDeltaCharacter = 0;
		}
		else
		{
			++iDeltaCharacter;
		}
	}
	
	std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.begin());
	
	while(iClient != m_Clients.end())
	{
		DiED::User & OtherUser = *(iClient->second);
		
		if((&User == &OtherUser) || (iLine < OtherUser.iGetLine()) || ((iLine == OtherUser.iGetLine()) && (iCharacter < OtherUser.iGetCharacter())))
		{
			if(iDeltaLine == 0)
			{
				OtherUser.vModifyCaretPosition(iDeltaLine, iDeltaCharacter);
			}
			else
			{
				OtherUser.vModifyCaretPosition(iDeltaLine, OtherUser.iGetCharacter() - iCharacter + iDeltaCharacter);
			}
		}
		++iClient;
	}
}

//~ void DiED::System::vConnect(DiED::User & User, const DiED::clientid_t & ClientID, const Network::port_t & Port)
//~ {
//~ 	std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.find(ClientID));
//~ 	
//~ 	if(iClient == m_Clients.end())
//~ 	{
//~ 		
//~ 	}
//~ 	else
//~ 	{
//~ 	}
//~ }

void DiED::System::vSendMessage(Network::BasicMessage & Message)
{
	std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.begin());
	
	while(iClient != m_Clients.end())
	{
		Network::MessageStream & Stream(*(iClient->second));
		
		Stream << Message;
		++iClient;
	}
}

void DiED::System::vAccepted(boost::shared_ptr< Network::Socket > Socket)
{
	boost::shared_ptr< DiED::Client > NewClient(boost::dynamic_pointer_cast< DiED::Client >(Socket));
	
	m_Clients.insert(std::make_pair(NewClient->GetClientID(), NewClient));
//~ 	ClientConnected(*NewClient);
}
