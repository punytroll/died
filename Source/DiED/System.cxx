#include "System.h"

#include <iostream>

#include "../Common.h"
#include "Client.h"
#include "Messages.h"

DiED::System::System(void) :
	m_MessageFactory(new DiED::MessageFactory()),
	m_Server(m_MessageFactory),
	m_ServicePort(0)
{
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
	if(m_Client.get() == 0)
	{
		m_Client = ClientFactory->GetClient();
		vRegisterClient(m_Client);
	}
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
	boost::shared_ptr< Network::MessageStream > MessageStream(new Network::MessageStream(m_MessageFactory));
	
	MessageStream->vOpen(ConnectAddress, ConnectPort);
	if(MessageStream->bIsOpen() == false)
	{
		std::cout << "[Client]: Connection failed." << std::endl;
		
		return false;
	}
	else
	{
		std::cout << "[Client]: Connected to " << ConnectAddress << ':' << ConnectPort << std::endl;
	}
	
	boost::shared_ptr< DiED::Client > Client(m_ClientFactory->GetClient());
	
	Client->vSetMessageStream(MessageStream);
	vRegisterClient(Client);
	Client->operator<<(DiED::ConnectionRequestMessage(m_Client->GetClientID(), m_ServicePort));
	
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

void DiED::System::vConnectionRequest(DiED::User & User, const DiED::clientid_t & ClientID)
{
	// first of all: if we have no ID yet (not connected to a network) and somebody connects to us we have to generate an ID ourself
	if(m_Client->GetClientID() == 0)
	{
		vAssignClientID(*m_Client, rand());
	}
	
	std::multimap< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.find(ClientID));
	DiED::Client & Client(dynamic_cast< DiED::Client & >(User));
	
	if((iClient == m_Clients.end()) || (ClientID == 0))
	{
		vAssignClientID(Client, rand());
		Client << ConnectionAcceptMessage(Client.GetClientID(), m_Client->GetClientID());
		// send KnownClients to this client
		Client.m_u32KnownClientsMessageID = rand();
		
		std::vector< DiED::clientid_t > ConnectedClientIDs(GetConnectedClientIDs());
		std::vector< DiED::clientid_t > DisconnectedClientIDs(GetDisconnectedClientIDs());
		
		Client << KnownClientsMessage(Client.m_u32KnownClientsMessageID, ConnectedClientIDs, DisconnectedClientIDs);
	}
	else
	{
		std::cout << "TODO: " << __FILE__ << __LINE__ << std::endl;
	}
}

void DiED::System::vAssignClientID(DiED::Client & Client, const DiED::clientid_t & ClientID)
{
	if(ClientID == 0)
	{
		std::cout << "VERY BAD: " << __FILE__ << ':' << __LINE__ << std::endl;
	}
	
	DiED::clientid_t OldClientID(Client.GetClientID());
	
	if(OldClientID != 0)
	{
		std::cout << "VERY BAD: " << __FILE__ << ':' << __LINE__ << std::endl;
	}
	
	std::multimap< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.lower_bound(OldClientID));
	std::multimap< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iLastClient(m_Clients.upper_bound(OldClientID));
	
	while((iClient != iLastClient) && (&(*(iClient->second)) != &Client))
	{
		++iClient;
	}
	if(iClient == iLastClient)
	{
		std::cout << "VERY BAD!! " << __FILE__ << ':' << __LINE__ << std::endl;
		
		throw;
	}
	
	boost::shared_ptr< DiED::Client > ClientPtr(iClient->second);
	
	m_Clients.erase(iClient);
	ClientPtr->vSetClientID(ClientID);
	m_Clients.insert(std::make_pair(ClientID, ClientPtr));
	iClient = m_Clients.begin();
	while(iClient != m_Clients.end())
	{
		if((iClient->first != 0) && (&(*(iClient->second)) != &Client))
		{
			iClient->second->vSetStatus(ClientID, DiED::User::Disconnected);
			Client.vSetStatus(iClient->first, DiED::User::Disconnected);
		}
		++iClient;
	}
}

void DiED::System::vConnectionAccept(DiED::User & User, const DiED::clientid_t & LocalClientID, const DiED::clientid_t & RemoteClientID)
{
	DiED::Client & Client(dynamic_cast< DiED::Client & >(User));
	
	if(LocalClientID == 0)
	{
		Client.vSetMessageStream(boost::shared_ptr< Network::MessageStream >());
	}
	vAssignClientID(Client, RemoteClientID);
	if(m_Client->GetClientID() != LocalClientID)
	{
		vAssignClientID(*m_Client, LocalClientID);
	}
}

std::vector< DiED::clientid_t > DiED::System::GetConnectedClientIDs(void)
{
	return m_Client->GetConnectedClientIDs();
}

std::vector< DiED::clientid_t > DiED::System::GetDisconnectedClientIDs(void)
{
	return m_Client->GetDisconnectedClientIDs();
}

void DiED::System::vSendMessage(Network::BasicMessage & Message)
{
	std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.begin());
	
	while(iClient != m_Clients.end())
	{
		DiED::Client & Client(*(iClient->second));
		
		Client << Message;
		++iClient;
	}
}

void DiED::System::vAccepted(boost::shared_ptr< Network::MessageStream > MessageStream)
{
	boost::shared_ptr< DiED::Client > Client(m_ClientFactory->GetClient());
	
	Client->vSetMessageStream(MessageStream);
	vRegisterClient(Client);
}

void DiED::System::vRegisterClient(boost::shared_ptr< DiED::Client > Client)
{
	m_Clients.insert(std::make_pair(Client->GetClientID(), Client));
	m_pExternalEnvironment->vNewClient(*Client);
}
