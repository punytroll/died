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
		m_Client = RegisterClient();
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
//~ 		std::cout << "[Client]: Connected to " << ConnectAddress << ':' << ConnectPort << std::endl;
	}
	
	boost::shared_ptr< DiED::Client > Client(RegisterClient());
	
	Client->vSetMessageStream(MessageStream);
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
		
		std::vector< DiED::clientid_t > DisconnectedClientIDs(GetDisconnectedClientIDs());
		std::vector< DiED::clientid_t >::iterator iTempClient(find(DisconnectedClientIDs.begin(), DisconnectedClientIDs.end(), Client.GetClientID()));
		
		if(iTempClient != DisconnectedClientIDs.end())
		{
			DisconnectedClientIDs.erase(iTempClient);
		}
		Client << KnownClientsMessage(Client.m_u32KnownClientsMessageID, GetConnectedClientIDs(), DisconnectedClientIDs);
	}
	else
	{
		// !(A || B) == !A && !B  ==>  ((iClient != m_Clients.end()) && (ClientID != 0)) ==> so iClient is valid
		iClient->second->vSetMessageStream(Client.GetMessageStream());
		Client.vSetMessageStream(boost::shared_ptr< Network::MessageStream >());
		iClient->second->operator<<(ConnectionAcceptMessage(iClient->first, m_Client->GetClientID()));
	}
}

void DiED::System::vAssignClientID(DiED::Client & Client, const DiED::clientid_t & ClientID)
{
	DiED::clientid_t OldClientID(Client.GetClientID());
	
	if(OldClientID == ClientID)
	{
		// nothing to do, eh!?
		return;
	}
	if(OldClientID != 0)
	{
		// we should not not change the client ID of a client that already has a "good" client ID ...
		//  => special cases/why not/what to do???
		std::cout << "VERY BAD: " << __FILE__ << ':' << __LINE__ << std::endl;
	}
	if(ClientID == 0)
	{
		// we will not set a client ID to 0 but what to do???
		std::cout << "VERY BAD: " << __FILE__ << ':' << __LINE__ << std::endl;
	}
	
	// since multiple clients with client ID are allowed we need to find the right one
	std::multimap< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.lower_bound(OldClientID));
	std::multimap< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iLastClient(m_Clients.upper_bound(OldClientID));
	
	while((iClient != iLastClient) && (iClient->second.get() != &Client))
	{
		++iClient;
	}
	// the client "Client" is not in our client list => strange
	if(iClient == iLastClient)
	{
		std::cout << "VERY BAD!! " << __FILE__ << ':' << __LINE__ << std::endl;
		
		throw;
	}
	
	// save the client locally
	boost::shared_ptr< DiED::Client > ClientPtr(iClient->second);
	
	// delete the old client
	m_Clients.erase(iClient);
	//set the new client ID
	ClientPtr->vSetClientID(ClientID);
	// insert the new client with the new client ID
	m_Clients.insert(std::make_pair(ClientPtr->GetClientID(), ClientPtr));
	
	// go through the client list and set the connection status
	//  => this will finish the task of RegisterClient() 
	iClient = m_Clients.begin();
	while(iClient != m_Clients.end())
	{
		if((iClient->first != 0) && (iClient->second.get() != &Client) && (iClient->second->GetStatus(ClientID) == DiED::User::Deleted))
		{
			iClient->second->vSetStatus(ClientID, DiED::User::Disconnected);
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
	else
	{
		std::vector< DiED::clientid_t > DisconnectedClientIDs(GetDisconnectedClientIDs());
		std::vector< DiED::clientid_t >::iterator iTempClient(find(DisconnectedClientIDs.begin(), DisconnectedClientIDs.end(), Client.GetClientID()));
		
		if(iTempClient != DisconnectedClientIDs.end())
		{
			DisconnectedClientIDs.erase(iTempClient);
		}
		Client << KnownClientsMessage(0, GetConnectedClientIDs(), DisconnectedClientIDs);
	}
}

void DiED::System::vKnownClients(DiED::User & User, const DiED::messageid_t & MessageID, const std::vector< DiED::clientid_t > & ConnectedClientIDs, const std::vector< DiED::clientid_t > & DisconnectedClientIDs)
{
//~ 	std::cout << "Processing KnownClients message." << std::endl;
	
	// iterating through the connected list to set the status of the sender to the client ID appropriately
	std::vector< DiED::clientid_t >::const_iterator iClient(ConnectedClientIDs.begin());
	
	while(iClient != ConnectedClientIDs.end())
	{
		boost::shared_ptr< DiED::Client > Client(RegisterClient(*iClient));
		
		if(Client.get() == 0)
		{
			std::cout << "VERY BAD: " << __FILE__ << ':' << __LINE__ << ": ClientID = " << *iClient << std::endl;
		}
		Client->vSetStatus(User.GetClientID(), DiED::Client::Connected);
		User.vSetStatus(*iClient, DiED::Client::Connected);
		++iClient;
	}
	iClient = DisconnectedClientIDs.begin();
	while(iClient != DisconnectedClientIDs.end())
	{
		boost::shared_ptr< DiED::Client > Client(RegisterClient(*iClient));
		
		if(Client.get() == 0)
		{
			std::cout << "VERY BAD: " << __FILE__ << ':' << __LINE__ << ": ClientID = " << *iClient << std::endl;
		}
		++iClient;
	}
	
	boost::shared_ptr< DiED::Client > Client(m_Clients.find(User.GetClientID())->second);
	
	if(MessageID == 0)
	{
		// send KnownClients to this client
		Client->m_u32KnownClientsMessageID = rand();
		
		std::vector< DiED::clientid_t > DisconnectedClientIDs(GetDisconnectedClientIDs());
		std::vector< DiED::clientid_t >::iterator iTempClient(find(DisconnectedClientIDs.begin(), DisconnectedClientIDs.end(), Client->GetClientID()));
		
		if(iTempClient != DisconnectedClientIDs.end())
		{
			DisconnectedClientIDs.erase(iTempClient);
		}
		Client->operator<<(KnownClientsMessage(Client->m_u32KnownClientsMessageID, GetConnectedClientIDs(), DisconnectedClientIDs));
	}
	else
	{
		Client->operator<<(ClientsRegisteredMessage(MessageID));
		Client->vSetStatus(m_Client->GetClientID(), DiED::User::Connected);
		m_Client->vSetStatus(User.GetClientID(), DiED::User::Connected);
		
		std::multimap< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.begin());
		
		while(iClient != m_Clients.end())
		{
			if((iClient->first == User.GetClientID()) || (iClient->first == m_Client->GetClientID()) || (m_Client->GetStatus(iClient->first) == DiED::Client::Disconnected))
			{
//~ 				std::cout << "Client " << iClient->first << " is not messaged." << std::endl;
				++iClient;
				
				continue;
			}
//~ 			std::cout << "Sending ConnectionEstablishedMessage(" << Client->GetClientID() << ", " << Client->GetAddress() << ", " << Client->GetPort() << ") to " << iClient->first << " == " << iClient->second->GetClientID() << std::endl;
			iClient->second->operator<<(ConnectionEstablishedMessage(Client->GetClientID(), Client->GetAddress(), Client->GetPort()));
			++iClient;
		}
	}
}

void DiED::System::vClientsRegistered(DiED::User & User, const DiED::messageid_t & MessageID)
{
//~ 	std::cout << "Executing ClientsRegisteredMessage(" << MessageID << ") from " << User.GetClientID() << std::endl;
	m_Client->vSetStatus(User.GetClientID(), DiED::Client::Connected);
	User.vSetStatus(m_Client->GetClientID(), DiED::Client::Connected);
	
	boost::shared_ptr< DiED::Client > Client(m_Clients.find(User.GetClientID())->second);
	std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.begin());
	
	while(iClient != m_Clients.end())
	{
		if((iClient->first == User.GetClientID()) || (iClient->first == m_Client->GetClientID()) || (m_Client->GetStatus(iClient->first) == DiED::Client::Disconnected))
		{
//~ 			std::cout << "Client " << iClient->first << " is not messaged." << std::endl;
			++iClient;
			
			continue;
		}
//~ 		std::cout << "Sending ConnectionEstablishedMessage(" << Client->GetClientID() << ", " << Client->GetAddress() << ", " << Client->GetPort() << ") to " << iClient->first << " == " << iClient->second->GetClientID() << std::endl;
		iClient->second->operator<<(ConnectionEstablishedMessage(Client->GetClientID(), Client->GetAddress(), Client->GetPort()));
		++iClient;
	}
}

void DiED::System::vConnectionEstablished(DiED::User & User, const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort)
{
	boost::shared_ptr< DiED::Client > Client;
	std::multimap< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.find(ClientID));
	
	if(iClient == m_Clients.end())
	{
		Client = RegisterClient(ClientID);
		if(Client.get() == 0)
		{
			std::cout << "VERY BAD: " << __FILE__ << ':' << __LINE__ << ": ClientID = " << ClientID << std::endl;
		}
	}
	else
	{
		Client = iClient->second;
		// TODO
	}
	Client->vSetStatus(User.GetClientID(), DiED::Client::Connected);
	User.vSetStatus(Client->GetClientID(), DiED::Client::Connected);
	// "Client" is set to an existing client with the client ID "ClientID"
	if(m_Client->GetStatus(ClientID) == DiED::Client::Disconnected)
	{
		if(ClientPort == 0)
		{
			std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.begin());
			
			while(iClient != m_Clients.end())
			{
				// skip all clients NOT Connected to local (this includes Disconnected clients and unknown clients which in turn includes local)
				if(m_Client->GetStatus(iClient->first) != DiED::Client::Connected)
				{
					++iClient;
					
					continue;
				}
				iClient->second->operator<<(ConnectionLostMessage(Client->GetClientID()));
				++iClient;
			}
		}
		else
		{
			boost::shared_ptr< Network::MessageStream > MessageStream(new Network::MessageStream(m_MessageFactory));
			
			MessageStream->vOpen(ClientAddress, ClientPort);
			if(MessageStream->bIsOpen() == false)
			{
				std::cout << "[Client]: Connection failed. TODO: Send ConnectionLostMessage." << std::endl;
			}
			else
			{
//~ 				std::cout << "[Client]: Connected to " << ClientAddress << ':' << ClientPort << std::endl;
				Client->vSetMessageStream(MessageStream);
				Client->operator<<(DiED::ConnectionRequestMessage(m_Client->GetClientID(), m_ServicePort));
			}
		}
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
	std::multimap< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.begin());
	
	while(iClient != m_Clients.end())
	{
		DiED::Client & Client(*(iClient->second));
		
		Client << Message;
		++iClient;
	}
}

void DiED::System::vAccepted(boost::shared_ptr< Network::MessageStream > MessageStream)
{
	boost::shared_ptr< DiED::Client > Client(RegisterClient());
	
	Client->vSetMessageStream(MessageStream);
}

boost::shared_ptr< DiED::Client > DiED::System::RegisterClient(const DiED::clientid_t & ClientID)
{
	std::multimap< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iTheClient(m_Clients.find(ClientID));
	
	// if either no client with this client ID exists or the requested client ID is '0'
	//  => Client ID '0' is allowed multiple times
	if((iTheClient == m_Clients.end()) || (ClientID == 0))
	{
		boost::shared_ptr< DiED::Client > Client(m_ClientFactory->GetClient());
		
		if(ClientID != 0)
		{
			Client->vSetClientID(ClientID);
		}
		m_Clients.insert(std::make_pair(Client->GetClientID(), Client));
		m_pExternalEnvironment->vNewClient(*Client);
		
		// no set the status to disconnected for all clients (except those with client ID '0')
		std::multimap< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.begin());
		
		while(iClient != m_Clients.end())
		{
			if((iClient->first != ClientID) && (iClient->first != 0))
			{
				// don't set any connection if the client's client ID is zero
				//  => will be done by vAssignClientID
				if(ClientID != 0)
				{
					iClient->second->vSetStatus(ClientID, DiED::User::Disconnected);
				}
				Client->vSetStatus(iClient->first, DiED::User::Disconnected);
			}
			++iClient;
		}
		
		return Client;
	}
	else
	{
		// !(A || B) == (!A && !B)  ==>  ((iTheClient != m_Clients.end) && (ClientID != 0))
		//  => registering an existing client => just return the existing one
		return iTheClient->second;
	}
}
