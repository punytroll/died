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
		m_Client = GetNewPreliminaryClient();
		m_Client->vSetPort(m_ServicePort);
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
		std::cout << "[Client]: Connection failed." << std::endl;
		
		return false;
	}
	else
	{
//~ 		std::cout << "[Client]: Connected to " << ConnectAddress << ':' << ConnectPort << std::endl;
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

void DiED::System::vInput(const Glib::ustring & sString)
{
	boost::shared_ptr< DiED::BasicMessage > Message(new DiED::InputMessage(m_Client->GetID(), 0, 0, sString));
	
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
	
//~ 	std::cout << "LineW = " << iLine << " ; CharacterW = " << iCharacter << std::endl;
	
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
//~ 	std::cout << "DeltaLine = " << iDeltaLine << " ; DeltaCharacter = " << iDeltaCharacter << std::endl;
	
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

void DiED::System::vConnectionRequest(DiED::User & User, const DiED::clientid_t & ClientID)
{
	std::cout << "Processing ConnectionRequest message from " << User.GetID() << std::endl;
	
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
		// now we can answer the message with ConnectionAccept according to specification
		Client->vConnectionAccept(m_Client->GetID(), Client->GetID());
		// send KnownClients to this client
		Client->vKnownClients();
	}
	else
	{
		std::cout << "TODO: test connection with ping." << std::endl;
		// => the local client is known in the network it is connecting to
		iClient->second->vSetMessageStream(Client->GetMessageStream());
		iClient->second->vConnectionAccept(m_Client->GetID(), ClientID);
		Client->vSetMessageStream(boost::shared_ptr< Network::MessageStream >());
		// TODO: what to do with the iPreliminaryClient ... it is invalid and emtpy (no socket) but MUST NOT be deleted from here
	}
	std::cout << "           ConnectionRequest message from " << User.GetID() << '\n' << std::endl;
}

void DiED::System::vConnectionAccept(DiED::User & User, const DiED::clientid_t & AccepterClientID, const DiED::clientid_t & RequesterClientID)
{
	std::cout << "Processing ConnectionAccept message from " << User.GetID() << std::endl;
	
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
		std::cout << "[System]: We got a RequesterClientID == 0." << std::endl;
		Client->vSetMessageStream(boost::shared_ptr< Network::MessageStream >());
	}
	if(m_Client->GetID() != RequesterClientID)
	{
		// in this case the local client is a newling to the network
		if(m_Client->GetID() != 0)
		{
			// this seems to be a reconnection to a network and we have a client id, meaning m_Client already is in the client list
			//  => if this happens we need to examine the case
			std::cout << "VERY BAD: m_Client->GetID() = " << m_Client->GetID() << " ; RequesterClientID = " << RequesterClientID << "  " << __FILE__ << ':' << __LINE__ << std::endl;
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
	std::cout << "           ConnectionAccept message from " << User.GetID() << '\n' << std::endl;
}

void DiED::System::vKnownClients(DiED::User & User, const DiED::messageid_t & MessageID, const std::vector< DiED::clientid_t > & ConnectedClientIDs, const std::vector< DiED::clientid_t > & DisconnectedClientIDs)
{
	std::cout << "Processing KnownClients message from " << User.GetID() << std::endl;
	
	// iterating through the connected list to set the status of the sender to the client ID appropriately
	std::vector< DiED::clientid_t >::const_iterator iClient(ConnectedClientIDs.begin());
	
	while(iClient != ConnectedClientIDs.end())
	{
		boost::shared_ptr< DiED::Client > Client(RegisterClient(boost::shared_ptr< DiED::Client >(), *iClient));
		
		if(Client.get() == 0)
		{
			std::cout << "VERY BAD: " << __FILE__ << ':' << __LINE__ << ": ClientID = " << *iClient << std::endl;
		}
		// RegisterClient will give a client that is Disconnected to ALL clients.
		vSetStatus(User.GetID(), Client->GetID(), DiED::User::Connected);
		++iClient;
	}
	iClient = DisconnectedClientIDs.begin();
	while(iClient != DisconnectedClientIDs.end())
	{
		boost::shared_ptr< DiED::Client > Client(RegisterClient(boost::shared_ptr< DiED::Client >(), *iClient));
		
		if(Client.get() == 0)
		{
			std::cout << "VERY BAD: " << __FILE__ << ':' << __LINE__ << ": ClientID = " << *iClient << std::endl;
		}
		++iClient;
	}
	
	boost::shared_ptr< DiED::Client > Client(m_Clients.find(User.GetID())->second);
	
	if(MessageID == 0)
	{
		Client->vKnownClients();
	}
	else
	{
		Client->vClientsRegistered(MessageID);
		vSetStatus(User.GetID(), m_Client->GetID(), DiED::User::Connected);
		
		// TODO send via vSendMessage
		std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.begin());
		
		while(iClient != m_Clients.end())
		{
			if((iClient->first == User.GetID()) || (iClient->first == m_Client->GetID()) || (m_Client->GetStatus(iClient->first) == DiED::Client::Disconnected))
			{
				++iClient;
				
				continue;
			}
			iClient->second->vConnectionEstablished(Client->GetID(), Client->GetAddress(), Client->GetPort());
			++iClient;
		}
	}
	std::cout << "           KnownClients message from " << User.GetID() << '\n' << std::endl;
}

void DiED::System::vClientsRegistered(DiED::User & User, const DiED::messageid_t & MessageID)
{
	std::cout << "Processing ClientsRegistered message from " << User.GetID() << std::endl;
	
	vSetStatus(User.GetID(), m_Client->GetID(), DiED::User::Connected);
	
	boost::shared_ptr< DiED::Client > Client(m_Clients.find(User.GetID())->second);
	std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.begin());
	
	// send via SendMessage
	while(iClient != m_Clients.end())
	{
		if((iClient->first == User.GetID()) || (iClient->first == m_Client->GetID()) || (m_Client->GetStatus(iClient->first) != DiED::Client::Connected))
		{
			++iClient;
			
			continue;
		}
		iClient->second->vConnectionEstablished(Client->GetID(), Client->GetAddress(), Client->GetPort());
		++iClient;
	}
	std::cout << "           ClientsRegistered message from " << User.GetID() << '\n' << std::endl;
}

void DiED::System::vConnectionEstablished(DiED::User & User, const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort)
{
	std::cout << "Processing ConnectionEstablished message from " << User.GetID() << std::endl;
	
	boost::shared_ptr< DiED::Client > Client;
	std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.find(ClientID));
	
	if(iClient == m_Clients.end())
	{
		Client = RegisterClient(boost::shared_ptr< DiED::Client >(), ClientID);
		if(Client.get() == 0)
		{
			std::cout << "VERY BAD: " << __FILE__ << ':' << __LINE__ << ": ClientID = " << ClientID << std::endl;
		}
	}
	else
	{
		Client = iClient->second;
	}
	vSetStatus(User.GetID(), Client->GetID(), DiED::Client::Connected);
	// "Client" is set to an existing client with the client ID "ClientID"
	switch(m_Client->GetStatus(ClientID))
	{
	case DiED::Client::Connected:
		{
			std::cout << "TODO: ConnectionEstablishedMessage: Client connected." << std::endl;
			Client->vPing(sigc::bind(sigc::mem_fun(*this, &DiED::System::vPongTimeout), Client));
			
			break;
		}
	case DiED::Client::Disconnected:
		{
			if(ClientPort == 0)
			{
				vSendMessage(boost::shared_ptr< DiED::BasicMessage >(new ConnectionLostMessage(Client->GetID(), ClientAddress, ClientPort)), true);
			}
			else
			{
				boost::shared_ptr< Network::MessageStream > MessageStream(new Network::MessageStream(m_MessageFactory));
				
				MessageStream->vOpen(ClientAddress, ClientPort);
				if(MessageStream->bIsOpen() == false)
				{
					vSendMessage(boost::shared_ptr< DiED::BasicMessage >(new ConnectionLostMessage(Client->GetID(), ClientAddress, ClientPort)), true);
				}
				else
				{
					Client->vSetMessageStream(MessageStream);
					Client->vSetPort(ClientPort);
					vSetStatus(m_Client->GetID(), Client->GetID(), DiED::User::Connecting);
					Client->vConnectionRequest(m_Client->GetID(), m_ServicePort);
				}
			}
			
			break;
		}
	case DiED::User::Connecting:
		{
			std::cout << "Already connecting to client " << Client->GetID() << std::endl;
			
			break;
		}
	default:
		{
			break;
		}
	}
	std::cout << "           ConnectionEstablished message from " << User.GetID() << '\n' << std::endl;
}

void DiED::System::vConnectionLost(DiED::User & User, const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort)
{
	std::cout << "Processing ConnectionLost message from " << User.GetID() << std::endl;
	
	boost::shared_ptr< DiED::Client > Client(RegisterClient(boost::shared_ptr< DiED::Client >(), ClientID));
	
	vSetStatus(User.GetID(), Client->GetID(), DiED::User::Disconnected);
	switch(m_Client->GetStatus(Client->GetID()))
	{
	case DiED::User::Disconnected:
		{
			// if we are disconnected to the specific client
			if((ClientPort != 0) && ((ClientAddress != Client->GetAddress()) || ClientPort != Client->GetPort()))
			{
				// only if something in the connection parameters changed we will retry a connection
				boost::shared_ptr< Network::MessageStream > MessageStream(new Network::MessageStream(m_MessageFactory));
				
				MessageStream->vOpen(ClientAddress, ClientPort);
				if(MessageStream->bIsOpen() == false)
				{
					std::cout << "[Client]: Connecting to " << ClientAddress << ':' << ClientPort << " failed. " << __FILE__ << __LINE__ << std::endl;
					vSendMessage(boost::shared_ptr< DiED::BasicMessage >(new DiED::ConnectionLostMessage(Client->GetID(), ClientAddress, ClientPort)), true);
				}
				else
				{
					Client->vSetMessageStream(MessageStream);
					Client->vSetPort(ClientPort);
					vSetStatus(m_Client->GetID(), Client->GetID(), DiED::User::Connecting);
					Client->vConnectionRequest(m_Client->GetID(), m_ServicePort);
				}
			}
			
			break;
		}
	case DiED::User::Connected:
		{
			boost::shared_ptr< DiED::Client > UserClient(RegisterClient(boost::shared_ptr< DiED::Client >(), User.GetID()));
			
			if(UserClient->GetPort() != 0)
			{
				Client->vConnectionEstablished(User.GetID(), UserClient->GetAddress(), UserClient->GetPort());
			}
			Client->vPing(sigc::bind(sigc::mem_fun(*this, &DiED::System::vPongTimeout), Client));
			
			break;
		}
	case DiED::User::Connecting:
		{
			break;
		}
	default:
		{
			std::cout << "TODO: " << __FILE__ << ':' << __LINE__ << std::endl;
			
			break;
		}
	}
	std::cout << "           ConnectionLost message from " << User.GetID() << '\n' << std::endl;
}

void DiED::System::vPongTimeout(boost::shared_ptr< DiED::Client > Client)
{
//~ 	std::cout << "Pong received from " << Client->GetID() << "." << std::endl;
}

std::vector< DiED::clientid_t > DiED::System::GetConnectedClientIDs(void)
{
	return m_Client->GetConnectedClientIDs();
}

std::vector< DiED::clientid_t > DiED::System::GetDisconnectedClientIDs(void)
{
	return m_Client->GetDisconnectedClientIDs();
}

DiED::User::Status DiED::System::GetStatus(const DiED::clientid_t & ClientID1, const DiED::clientid_t & ClientID2)
{
	boost::shared_ptr< DiED::Client > Client2;
	
	if(ClientID2 == 0)
	{
		Client2 = m_Client;
	}
	else
	{
		std::map< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.find(ClientID2));
		
		if(iClient == m_Clients.end())
		{
			return DiED::User::Deleted;
		}
		Client2 = iClient->second;
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
void DiED::System::vSetStatus(const DiED::clientid_t & ClientID1, const DiED::clientid_t & ClientID2, const DiED::User::Status & Status)
{
	boost::shared_ptr< DiED::Client > Client1;
	boost::shared_ptr< DiED::Client > Client2;
	
	if(ClientID1 == 0)
	{
		Client1 = m_Client;
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
	Client1->vSetStatus(ClientID2, Status);
	Client2->vSetStatus(ClientID1, Status);
}

void DiED::System::vSendMessage(boost::shared_ptr< DiED::BasicMessage > Message, bool bSendOnlyToConnected)
{
	std::multimap< DiED::clientid_t, boost::shared_ptr< DiED::Client > >::iterator iClient(m_Clients.begin());
	
	while(iClient != m_Clients.end())
	{
		if((iClient->second != m_Client) && ((bSendOnlyToConnected == false) || (m_Client->GetStatus(iClient->first) == DiED::User::Connected)))
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
//~ 	std::cout << "[DiED/System] Created a preliminary client at " << Client.get() << '.' << std::endl;
	
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
	
//~ 	std::cout << "[DiED/System] Registering Client " << Client.get() << " with ClientID " << ClientID << std::endl;
	
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
		iClient->second->vSetStatus(ClientID, DiED::User::Disconnected);
		Client->vSetStatus(iClient->first, DiED::User::Disconnected);
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
