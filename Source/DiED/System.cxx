#include "System.h"

#include <iostream>

#include "../Common.h"
#include "Client.h"
#include "Messages.h"

const unsigned int g_uiReconnectTryTimeout = 4000;

DiED::System::System(void) :
	m_MessageFactory(new DiED::MessageFactory()),
	m_Server(m_MessageFactory),
	m_ServicePort(0)
{
}

DiED::System::~System(void)
{
	std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.begin());
	
	while(iClient != m_Clients.end())
	{
		boost::shared_ptr< DiED::Client > Client(iClient->second);
		
		m_Clients.erase(iClient);
		iClient = m_Clients.begin();
	}
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
		m_Client = GetNewPreliminaryClient();
		m_Client->vSetPort(m_ServicePort);
		m_Client->StatusChanged.connect(sigc::mem_fun(*this, &DiED::System::vClientStatusChanged));
	}
}

bool DiED::System::bListen(const Network::port_t & ServicePort)
{
	m_Server.vOpen(ServicePort);
	if(m_Server.bIsOpen() == false)
	{
		std::cerr << "[Server]: Error setting up the server. [" << sErrorCodeToString(m_Server.iGetError()) << "]." << std::endl;
		
		return false;
	}
	else
	{
		m_ServicePort = ServicePort;
		m_Server.Accepted.connect(sigc::mem_fun(*this, &DiED::System::vAccepted));
	}
	if(m_Client.get() != 0)
	{
		m_Client->vSetPort(m_ServicePort);
	}
	
	return true;
}

bool DiED::System::bConnectTo(const Network::address_t & ConnectAddress, const Network::port_t & ConnectPort)
{
	boost::shared_ptr< Network::MessageStream > MessageStream(new Network::MessageStream(m_MessageFactory));
	
	MessageStream->vOpen(ConnectAddress, ConnectPort);
	if(MessageStream->bIsOpen() == false)
	{
		LOG(Info, "DiED/System", "Connection failed.");
		
		return false;
	}
	else
	{
		LOG(Info, "DiED/System", "Connected to " << ConnectAddress << ':' << ConnectPort);
	}
	
	boost::shared_ptr< DiED::Client > Client(GetNewPreliminaryClient());
	
	Client->vSetMessageStream(MessageStream);
	Client->vSetPort(ConnectPort);
	Client->vConnectionRequest(m_Client->GetID(), m_ServicePort);
	
	return true;
}

DiED::clientid_t DiED::System::GetLocalClientID(void)
{
	if(m_Client.get() != 0)
	{
		return m_Client->GetID();
	}
	
	return 0;
}

void DiED::System::vDelete(int iLine, int iCharacter)
{
	int iOldLine(m_Client->iGetLine());
	int iOldCharacter(m_Client->iGetCharacter());
	int iLineRelative(iLine - iOldLine);
	int iCharacterRelative(iCharacter - iOldCharacter);
	int iLineAbsolute(0);
	int iCharacterAbsolute(0);
	
	if((iLineRelative == 0) && (iCharacterRelative == 0))
	{
		return;
	}
	if(iLineRelative < 0)
	{
		iLineAbsolute = iLineRelative - iOldLine;
	}
	else
	{
		iLineAbsolute = m_pExternalEnvironment->iGetNumberOfLines() - iLine;
	}
	if(iCharacterRelative < 0)
	{
		iCharacterAbsolute = - iCharacterRelative - iOldCharacter;
	}
	else
	{
		iCharacterAbsolute = m_pExternalEnvironment->iGetLengthOfLine(iLine) - iCharacter;
	}
	m_Client->vSetLine(iLine);
	m_Client->vSetCharacter(iCharacter);
	
	std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.begin());
	DiED::messageid_t EventCounter(m_Client->GetNextEventCounter());
	
	while(iClient != m_Clients.end())
	{
		if(iClient->first != m_Client->GetID())
		{
			iClient->second->vDelete(iLineRelative, iCharacterRelative, iLineAbsolute, iCharacterAbsolute, EventCounter);
		}
		++iClient;
	}
}

void DiED::System::vPosition(int iLine, int iCharacter)
{
	LOG(Verbose, "DiED/System", "Changing the local position to Line = " << iLine << " ; Character = " << iCharacter);
	
	int iOldLine(m_Client->iGetLine());
	int iOldCharacter(m_Client->iGetCharacter());
	
	LOG(Verbose, "DiED/System", "\told position      @ Line = " << iOldLine << " ; Character = " << iOldCharacter);
	
	int iLineRelative(iLine - iOldLine);
	int iCharacterRelative(iCharacter - iOldCharacter);
	
	LOG(Verbose, "DiED/System", "\trelative position @ Line = " << iLineRelative << " ; Character = " << iCharacterRelative);
	
	int iLineAbsolute(0);
	int iCharacterAbsolute(0);
	
	if((iLineRelative == 0) && (iCharacterRelative == 0))
	{
		return;
	}
	if(iLineRelative < 0)
	{
		iLineAbsolute = iLineRelative - iOldLine;
	}
	else
	{
		iLineAbsolute = m_pExternalEnvironment->iGetNumberOfLines() - iLine;
	}
	if(iCharacterRelative < 0)
	{
		iCharacterAbsolute = - iCharacterRelative - iOldCharacter;
	}
	else
	{
		iCharacterAbsolute = m_pExternalEnvironment->iGetLengthOfLine(iLine) - iCharacter;
	}
	LOG(Verbose, "DiED/System", "\tabsolute position @ Line = " << iLineAbsolute << " ; Character = " << iCharacterAbsolute);
	m_Client->vSetLine(iLine);
	m_Client->vSetCharacter(iCharacter);
	
	std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.begin());
	DiED::messageid_t EventCounter(m_Client->GetNextEventCounter());
	
	while(iClient != m_Clients.end())
	{
		if(iClient->first != m_Client->GetID())
		{
			iClient->second->vPosition(iLineRelative, iCharacterRelative, iLineAbsolute, iCharacterAbsolute, EventCounter);
		}
		++iClient;
	}
}

void DiED::System::vInsert(const Glib::ustring & sString)
{
	std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.begin());
	DiED::messageid_t EventCounter(m_Client->GetNextEventCounter());
	
	while(iClient != m_Clients.end())
	{
		if(iClient->first != m_Client->GetID())
		{
			iClient->second->vInsert(sString, EventCounter);
		}
		++iClient;
	}
	vInsert(*m_Client, sString, false);
}

void DiED::System::vInsert(DiED::User & User, const Glib::ustring & sString, bool bWriteToEnvironment)
{
	int iLine(User.iGetLine());
	int iCharacter(User.iGetCharacter());
	
	LOG(Verbose, "DiED/System", "LineW = " << iLine << " ; CharacterW = " << iCharacter);
	
	if((m_pExternalEnvironment != 0) && (bWriteToEnvironment == true))
	{
		m_pExternalEnvironment->vInsert(sString, iLine, iCharacter);
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
	LOG(Verbose, "DiED/System", "DeltaLine = " << iDeltaLine << " ; DeltaCharacter = " << iDeltaCharacter);
	
	std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.begin());
	
	while(iClient != m_Clients.end())
	{
		DiED::User & OtherUser = *(iClient->second);
		
		if(OtherUser.iGetLine() >= iLine)
		{
			if(OtherUser.iGetLine() > iLine)
			{
				OtherUser.vModifyCaretPosition(iDeltaLine, 0);
			}
			else
			{
				if(OtherUser.iGetCharacter() >= iCharacter)
				{
					if(OtherUser.iGetCharacter() == iCharacter)
					{
						if(&OtherUser == &User)
						{
							if(iDeltaLine == 0)
							{
								OtherUser.vModifyCaretPosition(iDeltaLine, iDeltaCharacter);
							}
							else
							{
								OtherUser.vModifyCaretPosition(iDeltaLine, iDeltaCharacter - iCharacter);
							}
						}
					}
					else
					{
						if(iDeltaLine == 0)
						{
							OtherUser.vModifyCaretPosition(iDeltaLine, iDeltaCharacter);
						}
						else
						{
							OtherUser.vModifyCaretPosition(iDeltaLine, iDeltaCharacter - iCharacter);
						}
					}
				}
			}
		}
		++iClient;
	}
}

void DiED::System::vHandleConnectionRequest(DiED::User & User, const DiED::clientid_t & ClientID, const Network::port_t & Port)
{
	LOG(Debug, "DiED/System", "Processing ConnectionRequest message from " << User.GetID());
	
	// first of all: if we have no ID yet (not connected to a network) and somebody connects to us we have to generate an ID ourself
	if(m_Client->GetID() == 0)
	{
		// this assures that, after this point, the local client has NOT the client ID '0'
		//  => NO client has the client ID '0'
		//  => the only exception is the local client _BEFORE_ any network interaction with other clients occures
		RegisterClient(m_Client);
	}
	
	// ConnectionRequest messages are not allowed if the client is not in the PreliminaryClients map
	//  => this means, only Clients that have freshly been accepted by the server are allowed
	std::map< DiED::User *, boost::shared_ptr< DiED::Client > >::iterator iPreliminaryClient(m_PreliminaryClients.find(&User));
	
	if(iPreliminaryClient == m_PreliminaryClients.end())
	{
		// no preliminary client => ignore the message
		return;
	}
	else
	{
		LOG(Debug, "DiED/System", "\tConnection request from " << iPreliminaryClient->second->GetAddress() << ':' << iPreliminaryClient->second->GetPort());
	}
	// NOTE: although after a succeeded ConnectionRequest handling we should erase the iPreliminaryClient from the map
	//       in this case this is not a good idea because the "second" property contains the only boost::shared_ptr to
	//       the Client we called this message from. It would delete the caller!
	
	boost::shared_ptr< DiED::Client > Client(iPreliminaryClient->second);
	
	// the client asking for a connection can be either:
	//  - known, which means a client with the client ID 'ClientID' already exists in the clients list
	//  - unknown, when no such client exists
	//  => since no client can have client ID '0' (see above) a newling will be recognized here
	std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.find(ClientID));
	
	if(iClient == m_Clients.end())
	{
		// => the local client is unknown in the network it is connecting to
		RegisterClient(Client);
		Client->vSetPort(Port);
		// now we can answer the message with ConnectionAccept according to specification
		Client->vConnectionAccept(m_Client->GetID(), Client->GetID());
		// send KnownClients to this client
		Client->vKnownClients();
	}
	else
	{
		// => the local client is known in the network it is connecting to
		if(GetStatus(0, Client->GetID()) == DiED::Connected)
		{
			LOG(TODO, "DiED/System", "Test connection with ping.");
		}
		iClient->second->vSetMessageStream(Client->GetMessageStream());
		iClient->second->vConnectionAccept(m_Client->GetID(), ClientID);
		Client->vSetMessageStream(boost::shared_ptr< Network::MessageStream >());
		Client->vSetPort(Port);
		// TODO: what to do with the iPreliminaryClient ... it is invalid and emtpy (no socket) but MUST NOT be deleted from here
	}
	LOG(Debug, "DiED/System", "           ConnectionRequest message from " << User.GetID() << '\n');
}

void DiED::System::vHandleConnectionAccept(DiED::User & User, const DiED::clientid_t & AccepterClientID, const DiED::clientid_t & RequesterClientID)
{
	LOG(Debug, "DiED/System", "Processing ConnectionAccept message from " << User.GetID());
	
	std::map< DiED::User *, boost::shared_ptr< DiED::Client > >::iterator iPreliminaryClient(m_PreliminaryClients.find(&User));
	boost::shared_ptr< DiED::Client > Client;
	
	if(iPreliminaryClient == m_PreliminaryClients.end())
	{
		// this can happen if we connect to a client we already know the ClientID of and thus is registered already
		//  => User IS the valid client, so we only need to resolve it
		std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.find(AccepterClientID));
		
		if(iClient == m_Clients.end())
		{
			// this is very bad indeed
			std::cout << "VERY BAD: " << __FILE__ << ':' << __LINE__ << " ; AccepterClientID = " << AccepterClientID << " ; User.ClientID = " << User.GetID() << std::endl;
			
			throw;
		}
		else
		{
			Client = iClient->second;
		}
	}
	else
	{
		Client = iPreliminaryClient->second;
		RegisterClient(Client, AccepterClientID);
	}
	
	// if we get assigned a client ID '0' there is nothing for it but to close the socket
	if(RequesterClientID == 0)
	{
		LOG(Warning, "DiED/System", "We got a RequesterClientID == 0.");
		Client->vSetMessageStream(boost::shared_ptr< Network::MessageStream >());
	}
	if(m_Client->GetID() != RequesterClientID)
	{
		// in this case the local client is a newling to the network
		if(m_Client->GetID() != 0)
		{
			// this seems to be a reconnection to a network and we have a client id, meaning m_Client already is in the client list
			//  => if this happens we need to examine the case
			LOG(Error, "DiED/System", "VERY BAD: m_Client->GetID() = " << m_Client->GetID() << " ; RequesterClientID = " << RequesterClientID << "  " << __FILE__ << ':' << __LINE__);
		}
		else
		{
			RegisterClient(m_Client, RequesterClientID);
		}
		// newlings are not expected to do anything else
	}
	else
	{
		// the local client is not a newling => answer with KnownClients
		Client->vKnownClients(true);
	}
	LOG(Debug, "DiED/System", "           ConnectionAccept message from " << User.GetID() << '\n');
}

void DiED::System::vHandleKnownClients(DiED::User & User, const DiED::messageid_t & MessageID, const std::vector< ClientInfo > & ClientInfos)
{
	LOG(Debug, "DiED/System", "Processing KnownClients message from " << User.GetID());
	
	// iterating through the connected list to set the status of the sender to the client ID appropriately
	std::vector< DiED::ClientInfo >::const_iterator iClientInfo(ClientInfos.begin());
	
	while(iClientInfo != ClientInfos.end())
	{
		boost::shared_ptr< DiED::Client > Client;
		
		if(pGetClient(iClientInfo->ClientID) == 0)
		{
			Client = RegisterClient(boost::shared_ptr< DiED::Client >(), iClientInfo->ClientID);
			if(Client.get() == 0)
			{
				LOG(Error, "DiED/System", "VERY BAD: " << __FILE__ << ':' << __LINE__ << ": ClientID = " << iClientInfo->ClientID);
			}
			Client->vSetEventCounter(iClientInfo->EventCounter);
			// TODO: uncomment these lines, once we transmit the document.
//~ 			Client->vSetLine(iClientInfo->iLine);
//~ 			Client->vSetCharacter(iClientInfo->iCharacter);
			LOG_NO_NL(Debug, "DiED/System", "New client " << iClientInfo->ClientID << " is ");
		}
		else
		{
			Client = RegisterClient(boost::shared_ptr< DiED::Client >(), iClientInfo->ClientID);
			if(Client.get() == 0)
			{
				LOG(Error, "DiED/System", "VERY BAD: " << __FILE__ << ':' << __LINE__ << ": ClientID = " << iClientInfo->ClientID);
			}
			if(iClientInfo->Status == DiED::Deleted)
			{
				Client->vSetEventCounter(iClientInfo->EventCounter);
				// TODO: uncomment these lines, once we transmit the document.
//~ 				Client->vSetLine(iClientInfo->iLine);
//~ 				Client->vSetCharacter(iClientInfo->iCharacter);
			}
			LOG_NO_NL(Debug, "DiED/System", "Known client " << iClientInfo->ClientID << " is ");
		}
		LOG_PURE(Debug, "DiED/System", sStatusToString(iClientInfo->Status) << " to client " << User.GetID() << std::endl);
		if(iClientInfo->Status != DiED::Deleted)
		{
			vSetStatus(User.GetID(), Client->GetID(), iClientInfo->Status);
		}
		else
		{
			vSetStatus(User.GetID(), Client->GetID(), DiED::Disconnected);
		}
		++iClientInfo;
	}
	
	boost::shared_ptr< DiED::Client > Client(m_Clients.find(User.GetID())->second);
	
	if(Client.get() == 0)
	{
		LOG(Error, "DiED/System", "VERY BAD: " << __FILE__ << ':' << __LINE__ << ": ClientID = " << iClientInfo->ClientID);
	}
	if(MessageID == 0)
	{
		Client->vKnownClients();
	}
	else
	{
		Client->vClientsRegistered(MessageID);
		vSetStatus(User.GetID(), m_Client->GetID(), DiED::Connected);
		
		std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.begin());
		
		while(iClient != m_Clients.end())
		{
			if((iClient->first == User.GetID()) || (iClient->first == m_Client->GetID()) || (m_Client->GetStatus(iClient->first) == DiED::Disconnected))
			{
				++iClient;
				
				continue;
			}
			iClient->second->vConnectionEstablished(Client->GetID(), Client->GetAddress(), Client->GetPort());
			++iClient;
		}
	}
	LOG(Debug, "DiED/System", "           KnownClients message from " << User.GetID() << '\n');
}

void DiED::System::vHandleClientsRegistered(DiED::User & User, const DiED::messageid_t & MessageID)
{
	LOG(Debug, "DiED/System", "Processing ClientsRegistered message from " << User.GetID());
	
	vSetStatus(User.GetID(), m_Client->GetID(), DiED::Connected);
	
	boost::shared_ptr< DiED::Client > Client(m_Clients.find(User.GetID())->second);
	std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.begin());
	
	// send via SendMessage
	while(iClient != m_Clients.end())
	{
		if((iClient->first == User.GetID()) || (iClient->first == m_Client->GetID()) || (m_Client->GetStatus(iClient->first) != DiED::Connected))
		{
			++iClient;
			
			continue;
		}
		iClient->second->vConnectionEstablished(Client->GetID(), Client->GetAddress(), Client->GetPort());
		++iClient;
	}
	LOG(Debug, "DiED/System", "           ClientsRegistered message from " << User.GetID() << '\n');
}

void DiED::System::vHandleConnectionEstablished(DiED::User & User, const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort)
{
	LOG(Debug, "DiED/System", "Processing ConnectionEstablished message from " << User.GetID());
	
	boost::shared_ptr< DiED::Client > Client;
	std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.find(ClientID));
	
	if(iClient == m_Clients.end())
	{
		Client = RegisterClient(boost::shared_ptr< DiED::Client >(), ClientID);
		if(Client.get() == 0)
		{
			LOG(Error, "DiED/System", "VERY BAD: " << __FILE__ << ':' << __LINE__ << ": ClientID = " << ClientID);
		}
	}
	else
	{
		Client = iClient->second;
	}
	vSetStatus(User.GetID(), Client->GetID(), DiED::Connected);
	// "Client" is set to an existing client with the client ID "ClientID"
	switch(m_Client->GetStatus(ClientID))
	{
	case DiED::Connected:
		{
			if((ClientAddress != Client->GetAddress()) || (ClientPort != Client->GetPort()))
			{
				Client->vPing(sigc::bind(sigc::bind(sigc::mem_fun(*this, &DiED::System::vPongTimeoutOnConnectionEstablished), ClientAddress, ClientPort), Client));
			}
			
			break;
		}
	case DiED::Disconnected:
		{
			if(ClientPort == 0)
			{
				std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.begin());
				
				while(iClient != m_Clients.end())
				{
					if((iClient->second != m_Client) && (m_Client->GetStatus(iClient->first) == DiED::Connected))
					{
						iClient->second->vConnectionLost(Client->GetID(), ClientAddress, ClientPort);
					}
					++iClient;
				}
			}
			else
			{
				boost::shared_ptr< Network::MessageStream > MessageStream(new Network::MessageStream(m_MessageFactory));
				
				MessageStream->vOpen(ClientAddress, ClientPort);
				if(MessageStream->bIsOpen() == false)
				{
					if((ClientAddress != Client->GetAddress()) || (ClientPort != Client->GetPort()))
					{
						// TODO: vAnnounceConnectionLost
						std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.begin());
						
						while(iClient != m_Clients.end())
						{
							if(m_Client->GetStatus(iClient->first) == DiED::Connected)
							{
								iClient->second->vConnectionLost(Client->GetID(), ClientAddress, ClientPort);
							}
							++iClient;
						}
					}
				}
				else
				{
					Client->vSetMessageStream(MessageStream);
					Client->vSetPort(ClientPort);
					vSetStatus(m_Client->GetID(), Client->GetID(), DiED::Connecting);
					Client->vConnectionRequest(m_Client->GetID(), m_ServicePort);
				}
			}
			
			break;
		}
	case DiED::Connecting:
		{
			LOG(Info, "DiED/System", "Already connecting to client " << Client->GetID());
			
			break;
		}
	default:
		{
			break;
		}
	}
	LOG(Debug, "DiED/System", "           ConnectionEstablished message from " << User.GetID() << '\n');
}

void DiED::System::vHandleConnectionLost(DiED::User & User, const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort)
{
	LOG(Debug, "DiED/System", "Processing ConnectionLost message from " << User.GetID());
	
	boost::shared_ptr< DiED::Client > Client(RegisterClient(boost::shared_ptr< DiED::Client >(), ClientID));
	
	vSetStatus(User.GetID(), Client->GetID(), DiED::Disconnected);
	switch(m_Client->GetStatus(Client->GetID()))
	{
	case DiED::Disconnected:
		{
			// if we are disconnected to the specific client
			if((ClientPort != 0) && (Client->GetAddress() != "") && ((ClientAddress != Client->GetAddress()) || ClientPort != Client->GetPort()))
			{
				// only if something in the connection parameters changed we will retry a connection
				boost::shared_ptr< Network::MessageStream > MessageStream(new Network::MessageStream(m_MessageFactory));
				
				MessageStream->vOpen(ClientAddress, ClientPort);
				if(MessageStream->bIsOpen() == false)
				{
					LOG(Info, "DiED/System", "Connecting to " << ClientAddress << ':' << ClientPort << " failed. " << __FILE__ << __LINE__);
					
					// TODO: vAnnounceConnectionLost
					std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.begin());
					
					while(iClient != m_Clients.end())
					{
						if((iClient->second != m_Client) && (m_Client->GetStatus(iClient->first) == DiED::Connected))
						{
							iClient->second->vConnectionLost(Client->GetID(), ClientAddress, ClientPort);
						}
						++iClient;
					}
				}
				else
				{
					Client->vSetMessageStream(MessageStream);
					Client->vSetPort(ClientPort);
					vSetStatus(m_Client->GetID(), Client->GetID(), DiED::Connecting);
					Client->vConnectionRequest(m_Client->GetID(), m_ServicePort);
				}
			}
			
			break;
		}
	case DiED::Connected:
		{
			boost::shared_ptr< DiED::Client > UserClient(RegisterClient(boost::shared_ptr< DiED::Client >(), User.GetID()));
			
			if(UserClient->GetPort() != 0)
			{
				Client->vConnectionEstablished(User.GetID(), UserClient->GetAddress(), UserClient->GetPort());
			}
			Client->vPing(sigc::bind(sigc::mem_fun(*this, &DiED::System::vPongTimeout), Client));
			
			break;
		}
	case DiED::Connecting:
		{
			break;
		}
	default:
		{
			LOG(TODO, "DiED/System", __FILE__ << ':' << __LINE__);
			
			break;
		}
	}
	LOG(Debug, "DiED/System", "           ConnectionLost message from " << User.GetID() << '\n');
}

void DiED::System::vHandleInsert(DiED::User & User, const Glib::ustring & sString)
{
	LOG(Debug, "DiED/System", "Processing Insert message from " << User.GetID());
	vInsert(User, sString, true);
	LOG(Debug, "DiED/System", "           Insert message from " << User.GetID() << '\n');
}

void DiED::System::vHandleDelete(DiED::User & User, int iLineRelative, int iCharacterRelative, int iLineAbsolute, int iCharacterAbsolute)
{
	LOG(Debug, "DiED/System", "Processing Delete message from " << User.GetID());
	if(iLineRelative != 0)
	{
		if(iLineRelative > 0)
		{
			if((User.iGetLine() + iLineRelative + iLineAbsolute) != m_pExternalEnvironment->iGetNumberOfLines())
			{
				LOG(TODO, "DiED/System", "The absolute and relative values don't match.\n\t\tUser.iLine = " << User.iGetLine() << "; iLineRelative = " << iLineRelative << "; iLineAbsolute = " << iLineAbsolute << "; #Lines = " << m_pExternalEnvironment->iGetNumberOfLines());
			}
		}
		else
		{
			if((User.iGetLine() + iLineRelative + iLineAbsolute) != 0)
			{
				LOG(TODO, "DiED/System", "The absolute and relative values don't match.\n\t\tUser.iLine = " << User.iGetLine() << "; iLineRelative = " << iLineRelative << "; iLineAbsolute = " << iLineAbsolute << "; #Lines = " << m_pExternalEnvironment->iGetNumberOfLines());
			}
		}
	}
//~ 	if(iCharacterRelative > 0)
//~ 	{
//~ 		if((User.iGetCharacter() + iCharacterRelative + iCharacterAbsolute) != m_pExternalEnvironment->iGetLengthOfLine())
//~ 		{
//~ 			// TODO change iCharacterRelative
//~ 		}
//~ 	}
	m_pExternalEnvironment->vDelete(User.iGetLine(), User.iGetCharacter(), User.iGetLine() + iLineRelative, User.iGetCharacter() + iCharacterRelative);
	User.vModifyCaretPosition(iLineRelative, iCharacterRelative);
	LOG(Debug, "DiED/System", "           Delete message from " << User.GetID() << '\n');
}

void DiED::System::vHandlePosition(DiED::User & User, int iLineRelative, int iCharacterRelative, int iLineAbsolute, int iCharacterAbsolute)
{
	LOG(Debug, "DiED/System", "Processing Position message from " << User.GetID());
	LOG(Debug, "DiED/System", "\tUser @ Line = " << User.iGetLine() << " ; Character = " << User.iGetCharacter());
	if(iLineRelative != 0)
	{
		if(iLineRelative > 0)
		{
			if((User.iGetLine() + iLineRelative + iLineAbsolute) != m_pExternalEnvironment->iGetNumberOfLines())
			{
				LOG(TODO, "DiED/System", "The absolute and relative values don't match.\n\t\tUser.iLine = " << User.iGetLine() << "; iLineRelative = " << iLineRelative << "; iLineAbsolute = " << iLineAbsolute << "; #Lines = " << m_pExternalEnvironment->iGetNumberOfLines());
			}
		}
		else
		{
			if((User.iGetLine() + iLineRelative + iLineAbsolute) != 0)
			{
				LOG(TODO, "DiED/System", "The absolute and relative values don't match.\n\t\tUser.iLine = " << User.iGetLine() << "; iLineRelative = " << iLineRelative << "; iLineAbsolute = " << iLineAbsolute << "; #Lines = " << m_pExternalEnvironment->iGetNumberOfLines());
			}
		}
	}
//~ 	if(iCharacterRelative > 0)
//~ 	{
//~ 		if((User.iGetCharacter() + iCharacterRelative + iCharacterAbsolute) != m_pExternalEnvironment->iGetLengthOfLine())
//~ 		{
//~ 			// TODO change iCharacterRelative
//~ 		}
//~ 	}
	User.vModifyCaretPosition(iLineRelative, iCharacterRelative);
	LOG(Debug, "DiED/System", "           Position message from " << User.GetID() << '\n');
}

void DiED::System::vPongTimeout(boost::shared_ptr< DiED::Client > Client)
{
	LOG(Debug, "DiED/System", "Pong missing from " << Client->GetID() << ".");
}

void DiED::System::vPongTimeoutOnConnectionEstablished(boost::shared_ptr< DiED::Client > Client, const Network::address_t & ClientAddress, const Network::port_t & ClientPort)
{
	if(ClientPort == 0)
	{
		std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.begin());
		
		while(iClient != m_Clients.end())
		{
			if((iClient->second != m_Client) && (m_Client->GetStatus(iClient->first) == DiED::Connected))
			{
				iClient->second->vConnectionLost(Client->GetID(), ClientAddress, ClientPort);
			}
			++iClient;
		}
	}
	else
	{
		boost::shared_ptr< Network::MessageStream > MessageStream(new Network::MessageStream(m_MessageFactory));
		
		MessageStream->vOpen(ClientAddress, ClientPort);
		if(MessageStream->bIsOpen() == false)
		{
			std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.begin());
			
			while(iClient != m_Clients.end())
			{
				if((iClient->second != m_Client) && (m_Client->GetStatus(iClient->first) == DiED::Connected))
				{
					iClient->second->vConnectionLost(Client->GetID(), ClientAddress, ClientPort);
				}
				++iClient;
			}
		}
		else
		{
			Client->vSetMessageStream(MessageStream);
			Client->vSetPort(ClientPort);
			vSetStatus(m_Client->GetID(), Client->GetID(), DiED::Connecting);
			Client->vConnectionRequest(m_Client->GetID(), m_ServicePort);
		}
	}
}

std::set< DiED::clientid_t > DiED::System::GetConnectedClientIDs(void)
{
	return m_Client->GetConnectedClientIDs();
}

std::set< DiED::clientid_t > DiED::System::GetDisconnectedClientIDs(void)
{
	return m_Client->GetDisconnectedClientIDs();
}

std::vector< DiED::ClientInfo > DiED::System::GetClientInfos(void)
{
	std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.begin());
	std::vector< DiED::ClientInfo > ClientInfos;
	
	while(iClient != m_Clients.end())
	{
		ClientInfos.push_back(iClient->second->GetClientInfo());
		++iClient;
	}
	
	return ClientInfos;
}

DiED::clientstatus_t DiED::System::GetStatus(const DiED::clientid_t & _ClientID1, const DiED::clientid_t & _ClientID2)
{
	boost::shared_ptr< DiED::Client > Client1;
	boost::shared_ptr< DiED::Client > Client2;
	DiED::clientid_t ClientID1(_ClientID1);
	DiED::clientid_t ClientID2(_ClientID2);
	
	if(ClientID1 == 0)
	{
		Client1 = m_Client;
		ClientID1 = m_Client->GetID();
	}
	else
	{
		std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.find(ClientID1));
		
		if(iClient == m_Clients.end())
		{
			return DiED::Deleted;
		}
		Client1 = iClient->second;
	}
	if(ClientID2 == 0)
	{
		Client2 = m_Client;
		ClientID2 = m_Client->GetID();
	}
	else
	{
		std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.find(ClientID2));
		
		if(iClient == m_Clients.end())
		{
			return DiED::Deleted;
		}
		Client2 = iClient->second;
	}
	if((Client1.get() == 0) || (Client2.get() == 0) || (Client1 == Client2))
	{
		return DiED::Deleted;
	}
	
	return Client2->GetStatus(ClientID1);
}

DiED::Client * DiED::System::pGetClient(const DiED::clientid_t & ClientID)
{
	if(ClientID == 0)
	{
		return m_Client.get();
	}
	
	std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.find(ClientID));
	
	if(iClient == m_Clients.end())
	{
		return 0;
	}
	
	return iClient->second.get();
}

void DiED::System::vAnnounceConnectionLost(const DiED::clientid_t & ClientID)
{
	std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.find(ClientID));
	
	if(iClient == m_Clients.end())
	{
		return;
	}
	
	boost::shared_ptr< DiED::Client > Client(iClient->second);
	
	iClient = m_Clients.begin();
	while(iClient != m_Clients.end())
	{
		if(m_Client->GetStatus(iClient->first) == DiED::Connected)
		{
			iClient->second->vConnectionLost(ClientID, Client->GetAddress(), Client->GetPort());
		}
		++iClient;
	}
}

void DiED::System::vAnnounceMessage(boost::shared_ptr< DiED::BasicMessage > Message)
{
	std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.begin());
	
	while(iClient != m_Clients.end())
	{
		if(m_Client->GetStatus(iClient->first) == DiED::Connected)
		{
			iClient->second->vSend(Message);
		}
		++iClient;
	}
}

void DiED::System::vClientStatusChanged(const DiED::clientid_t & ClientID, const DiED::clientstatus_t & Status)
{
	if(Status == DiED::Disconnected)
	{
		Glib::signal_timeout().connect(sigc::bind(sigc::mem_fun(*this, &DiED::System::bTryReconnect), ClientID), g_uiReconnectTryTimeout + rand() % 2000);
	}
}

bool DiED::System::bTryReconnect(const DiED::clientid_t & ClientID)
{
	boost::shared_ptr< DiED::Client > Client(RegisterClient(boost::shared_ptr< DiED::Client >(), ClientID));
	
	if(Client.get() == 0)
	{
		return false;
	}
	if(GetStatus(ClientID, 0) != DiED::Disconnected)
	{
		return false;
	}
	
	boost::shared_ptr< Network::MessageStream > MessageStream(new Network::MessageStream(m_MessageFactory));
	
	MessageStream->vOpen(Client->GetAddress(), Client->GetPort());
	if(MessageStream->bIsOpen() == false)
	{
		vAnnounceConnectionLost(Client->GetID());
	}
	else
	{
		Client->vSetMessageStream(MessageStream);
		vSetStatus(m_Client->GetID(), Client->GetID(), DiED::Connecting);
		Client->vConnectionRequest(m_Client->GetID(), m_ServicePort);
	}
	
	return false;
}

/**
 * @brief The working horse for the Status relation.
 * @param ClientID1 The ID of one of the two related clients.
 * @param ClientID2 The ID of the other one of the two related clients.
 * @param Status The Status to be set between the two clients.
 *
 * This function will set the Status relation between the two clients identified by @a ClientID1 and @a ClientID2.
 *
 * You can pass '0' in order to identify the local client in any of the two client ID parameters but only one is useful.
 *
 * Since the Status relation is defined to be symmetric this call will result in two symmetric calls of DiED::Client::vSetStatus on the appropriate client objects.
 *
 * Since the Status relation is defined to be not reflexive, passing two identical client IDs or, in any case, two client IDs which represent the same client object, will result in no setting action. However keep in mind that the function needs time ( O(log(n)) ) to compute the client objects from the client IDs.
 **/
void DiED::System::vSetStatus(const DiED::clientid_t & _ClientID1, const DiED::clientid_t & _ClientID2, const DiED::clientstatus_t & Status)
{
	boost::shared_ptr< DiED::Client > Client1;
	boost::shared_ptr< DiED::Client > Client2;
	DiED::clientid_t ClientID1(_ClientID1);
	DiED::clientid_t ClientID2(_ClientID2);
	
	if(ClientID1 == 0)
	{
		Client1 = m_Client;
		ClientID1 = m_Client->GetID();
	}
	else
	{
		std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.find(ClientID1));
		
		if(iClient == m_Clients.end())
		{
			return;
		}
		Client1 = iClient->second;
	}
	if(ClientID2 == 0)
	{
		Client2 = m_Client;
		ClientID2 = m_Client->GetID();
	}
	else
	{
		std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.find(ClientID2));
		
		if(iClient == m_Clients.end())
		{
			return;
		}
		Client2 = iClient->second;
	}
	if(Client1 == Client2)
	{
		return;
	}
	if(Client1.get() != 0)
	{
		Client1->vSetStatus(ClientID2, Status);
	}
	if(Client2.get() != 0)
	{
		Client2->vSetStatus(ClientID1, Status);
	}
}

void DiED::System::vSendMessage(boost::shared_ptr< DiED::BasicMessage > Message, bool bSendOnlyToConnected)
{
	std::multimap< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.begin());
	
	while(iClient != m_Clients.end())
	{
		if((iClient->second != m_Client) && ((bSendOnlyToConnected == false) || (m_Client->GetStatus(iClient->first) == DiED::Connected)))
		{
			iClient->second->vSend(Message);
		}
		++iClient;
	}
}

void DiED::System::vAccepted(boost::shared_ptr< Network::MessageStream > MessageStream)
{
	boost::shared_ptr< DiED::Client > Client(GetNewPreliminaryClient());
	
	Client->vSetMessageStream(MessageStream);
}

boost::shared_ptr< DiED::Client > DiED::System::GetNewPreliminaryClient(void)
{
	boost::shared_ptr< DiED::Client > Client(m_ClientFactory->GetClient());
	
	m_PreliminaryClients[Client.get()] = Client;
	LOG(Verbose, "DiED/System", "Created a preliminary client at " << Client.get() << '.');
	
	return Client;
}

/**
 * @brief Registers a client.
 * @param Client A shared pointer with the client that is to be registered.
 * @param ClientID The ID that the client should have in the client list.
 * @return A boost::shared_ptr to the registered client. It is an uninitialized shared pointer if the registration was not successful.
 * 
 * This function is the heart of local client management. It allows you to register clients on different sets of preconditions.
 * 
 * Both parameters @a Client and @a ClientID are allowed to be unspecified. In case of @a Client this means an uninitialized shared pointer the "unspecified" value for @a ClientID is specified as '0', which is the default value.
 * 
 * Given both, @a Client and @a ClientID are unspecified: In this case a new client will be created by asking the ClientFactory for a new one. The client's ID will be randomized however assuring that it is unique in the client list.
 * 
 * If only @a Client is unspecified, @a ClientID will be evaluated for its uniqueness in the clients list. A new client will then be created by asking the ClientFactory for a new one and the client's ID will be set to @a ClientID.
 * 
 * If only @a ClientID is unspecified the client's ID will be randomized assuring its uniqueness in the clients list. @a Client will be inserted in the clients list indexed with @a ClientID. If @a Client exists in the preliminary clients list it will be erased from this list.
 *
 * Given both, @a Client and @a ClientID are specified, @a ClientID will be evaluated for its uniqueness in the clients list and Client will be inserted in the clients list indexed with @a ClientID. If @a Client exists in the preliminary clients list it will be erased from this list.
 *
 * Last but not least this function initializes the Status for all other clients in the client list. The new client will be Disconnected to all other clients and all other clients will be disconnected to the new client. (Symmetry)
 * 
 * The return value will be uninitialized if both @a Client and @a ClientID have been specified but @a ClientID is not unique. If @a Client was specified and the registration was successful the return value will equal @a Client.
 *
 * If @a ClientID is not unique and @a Client is unspecified this function will act as a retrieval function and will return the already registered client with client ID equal to ClientID.
 *
 * @note In this case the Status flags will not be touched.
 * 
 * This function assures that at no point in the process the client is in both lists.
 * 
 * @todo Is this last item even wanted/necessary?
 **/
boost::shared_ptr< DiED::Client > DiED::System::RegisterClient(boost::shared_ptr< DiED::Client > Client, const DiED::clientid_t & _ClientID)
{
	DiED::clientid_t ClientID(_ClientID);
	
	LOG(Verbose, "DiED/System", "[DiED/System] Registering Client " << Client.get() << " with ClientID " << ClientID);
	
	// client ID unspecified?
	if(ClientID == 0)
	{
		// randomize until valid and unique
		do
		{
			ClientID = rand();
		} while((ClientID == 0) || (ClientID == 0xFFFFFFFF) || (m_Clients.find(ClientID) != m_Clients.end()));
	}
	else
	{
		std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.find(ClientID));
		// if ClientID is specified but not unique
		if(iClient != m_Clients.end())
		{
			if(Client.get() == 0)
			{
				// if we don't care about WHAT client gets registered just return the one we have
				// NOTE: we don't touch the Status
				return iClient->second;
			}
			else
			{
				// if a specific client was to be registered with the existing ClientID we cannot perform the task
				return boost::shared_ptr< DiED::Client >();
			}
		}
	}
	// client unspecified?
	if(Client.get() == 0)
	{
		// make new one
		Client = m_ClientFactory->GetClient();
	}
	else
	{
		// if client is specified => lookup in preliminary clients list and erase if present
		std::map< DiED::User *, boost::shared_ptr< DiED::Client > >::iterator iPreliminaryClient(m_PreliminaryClients.find(Client.get()));
		
		if(iPreliminaryClient != m_PreliminaryClients.end())
		{
			m_PreliminaryClients.erase(iPreliminaryClient);
		}
		// the parameter Client is a safeguard aginst deletion from boost::shared_ptr
	}
	Client->vSetID(ClientID);
	
	// now setting the Status of all clients towards the newly registered client and the other way around
	// ATTENTION: here comes a vulnerable state, because the status of Client will be set for all other clients but Client is not yet in the list
	std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.begin());
	
	while(iClient != m_Clients.end())
	{
		// curerntly we cannot use the vSetStatus function of the system as the new client is not yet in the client list
		iClient->second->vSetStatus(ClientID, DiED::Disconnected);
		Client->vSetStatus(iClient->first, DiED::Disconnected);
		++iClient;
	}
	// now add the client
	m_Clients[ClientID] = Client;
	// and signal the world it is there
	if(m_pExternalEnvironment != 0)
	{
		m_pExternalEnvironment->vNewClient(*Client);
	}
	
	return Client;
}
