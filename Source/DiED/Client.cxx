#include "Client.h"

#include <iostream>

#include "BasicMessage.h"

DiED::Client::Client(DiED::InternalEnvironment & InternalEnvironment) :
	m_InternalEnvironment(InternalEnvironment),
	m_Port(0)
{
//~ 	std::cout << "[DiED/Client]: Created new Client." << std::endl;
}

DiED::Client::~Client(void)
{
//~ 	std::cout << "[DiED/Client]: Deleted Client." << std::endl;
}

void DiED::Client::vSetMessageStream(boost::shared_ptr< Network::MessageStream > MessageStream)
{
	// disconnect old relations to the current MessageStream
	m_BytesSentConnection.disconnect();
	m_MessageBeginConnection.disconnect();
	m_MessageReadyConnection.disconnect();
	// set the new MessageStream (the old one will get deleted by boost::shared_ptr if appropriate)
	m_MessageStream = MessageStream;
	// if we have a valid new MessageStream set up new relations
	if(m_MessageStream.get() != 0)
	{
		m_BytesSentConnection = m_MessageStream->BytesSent.connect(sigc::mem_fun(*this, &DiED::Client::vBytesSent));
		m_MessageBeginConnection = m_MessageStream->MessageBegin.connect(sigc::mem_fun(*this, &DiED::Client::vOnMessageBegin));
		m_MessageReadyConnection = m_MessageStream->MessageReady.connect(sigc::mem_fun(*this, &DiED::Client::vOnMessageReady));
	}
}

boost::shared_ptr< Network::MessageStream > DiED::Client::GetMessageStream(void)
{
	return m_MessageStream;
}

Network::port_t DiED::Client::GetPort(void)
{
	return m_Port;
}

Network::address_t DiED::Client::GetAddress(void)
{
	if(m_MessageStream.get() != 0)
	{
		m_Address = m_MessageStream->GetAddress();
	}
	
	return m_Address;
}

void DiED::Client::vSetPort(const Network::port_t & Port)
{
	m_Port = Port;
}

void DiED::Client::vHandleInsertText(const Glib::ustring & sString)
{
	m_InternalEnvironment.vInsertText(*this, sString);
}

void DiED::Client::vHandleConnectionRequest(const DiED::clientid_t & ClientID, const Network::port_t & Port)
{
	m_Port = Port;
	m_InternalEnvironment.vConnectionRequest(*this, ClientID);
}

void DiED::Client::vHandleConnectionAccept(const DiED::clientid_t & LocalClientID, const DiED::clientid_t & RemoteClientID)
{
	m_InternalEnvironment.vConnectionAccept(*this, LocalClientID, RemoteClientID);
}

void DiED::Client::vHandleKnownClients(const DiED::messageid_t & MessageID, const std::vector< DiED::clientid_t > & ConnectedClientIDs, const std::vector< DiED::clientid_t > & DisconnectedClientIDs)
{
	m_InternalEnvironment.vKnownClients(*this, MessageID, ConnectedClientIDs, DisconnectedClientIDs);
}

void DiED::Client::vHandleClientsRegistered(const DiED::messageid_t & MessageID)
{
	m_InternalEnvironment.vClientsRegistered(*this, MessageID);
}

void DiED::Client::vHandleConnectionEstablished(const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort)
{
	m_InternalEnvironment.vConnectionEstablished(*this, ClientID, ClientAddress, ClientPort);
}

void DiED::Client::vHandleConnectionLost(const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort)
{
	m_InternalEnvironment.vConnectionLost(*this, ClientID, ClientAddress, ClientPort);
}

void DiED::Client::vHandlePing(const DiED::messageid_t & PingID)
{
	vSend(boost::shared_ptr< DiED::BasicMessage >(new DiED::PongMessage(PingID)));
}

void DiED::Client::vHandlePong(const DiED::messageid_t & PingID)
{
	std::map< DiED::messageid_t, boost::shared_ptr< sigc::signal< void > > >::iterator iPongSignal(m_PongSignals.find(PingID));
	
	std::cout << "Pong message with ID " << PingID << " dropped in. Calling the signals." << std::endl;
	if(iPongSignal != m_PongSignals.end())
	{
		iPongSignal->second->emit();
		m_PongSignals.erase(iPongSignal);
	}
}

void DiED::Client::vOnMessageReady(void)
{
	vExecuteTopMessage();
}

void DiED::Client::vOnMessageBegin(void)
{
}

void DiED::Client::vOnMessageExecuted(void)
{
}

void DiED::Client::vExecuteTopMessage(void)
{
	if(m_MessageStream.get() == 0)
	{
		return;
	}
	
	std::deque< boost::shared_ptr< Network::BasicMessage > >::iterator iMessage(m_MessageStream->begin());
	
	if(iMessage == m_MessageStream->end())
	{
		return;
	}
	
	boost::shared_ptr< Network::BasicMessage > Message(*iMessage);
	DiED::BasicMessage & DiEDMessage = dynamic_cast< DiED::BasicMessage & >(*Message);
	
	// this seems awkward but see below
	boost::shared_ptr< Network::MessageStream > MessageStream(m_MessageStream);
	
	DiEDMessage.vExecute(*this);
	vOnMessageExecuted();
	
	// here we call pop_front on the saved MessageStream. This allows:
	// - changing ownership during Message execution _and_
	// - releasing the top message _after_ executing it
	MessageStream->pop_front();
}

void DiED::Client::vSend(boost::shared_ptr< DiED::BasicMessage > Message)
{
	if(Message->bIsEventMessage() == true)
	{
		m_EventQueue.push_back(boost::dynamic_pointer_cast< DiED::EventMessage >(Message));
	}
	else
	{
		if((m_MessageStream.get() != 0) && (m_MessageStream->bIsOpen() == true))
		{
			// m_MessageStream->vEmptyBuffer();
			m_MessageStream->operator<<(*Message);
			if(Message->bRequiresConfirmation() == true)
			{
				m_AwaitingConfirmationQueue.push_back(Message);
			}
		}
		else
		{
			std::cout << "VERY BAD: " << __FILE__ << ':' << __LINE__ << std::endl;
		}
	}
	if(m_InternalEnvironment.GetStatus(GetID()) == DiED::User::Connected)
	{
		if((m_MessageStream.get() != 0) && (m_MessageStream->bIsOpen() == true))
		{
			std::deque< boost::shared_ptr< DiED::EventMessage > >::iterator iEventMessage(m_EventQueue.begin());
			
			while(iEventMessage != m_EventQueue.end())
			{
				m_MessageStream->operator<<(**iEventMessage);
				if((*iEventMessage)->bRequiresConfirmation() == true)
				{
					m_AwaitingConfirmationQueue.push_back(*iEventMessage);
				}
				m_EventQueue.erase(iEventMessage);
				iEventMessage = m_EventQueue.begin();
			}
		}
		else
		{
			std::cout << "VERY BAD: " << __FILE__ << ':' << __LINE__ << std::endl;
		}
	}
}

void DiED::Client::vConnectionRequest(const DiED::clientid_t & ClientID, const Network::port_t & ListenPort)
{
	vSend(boost::shared_ptr< DiED::BasicMessage >(new DiED::ConnectionRequestMessage(ClientID, ListenPort)));
}

void DiED::Client::vConnectionAccept(const DiED::clientid_t & LocalClientID, const DiED::clientid_t & RemoteClientID)
{
	vSend(boost::shared_ptr< DiED::BasicMessage >(new DiED::ConnectionAcceptMessage(RemoteClientID, LocalClientID)));
}

void DiED::Client::vKnownClients(bool bAskForKnownClients)
{
	boost::shared_ptr< DiED::BasicMessage > KnownClientsMessage;
	std::vector< DiED::clientid_t > DisconnectedClientIDs(m_InternalEnvironment.GetDisconnectedClientIDs());
	std::vector< DiED::clientid_t > ConnectedClientIDs(m_InternalEnvironment.GetConnectedClientIDs());
	std::vector< DiED::clientid_t >::iterator iClient;
	
	// somewhere in the lists is _this_ client
	//  => but specification says we don't want to send the receiver's client ID with this message regardless of the list
	//  => find it and erase it
	iClient = find(DisconnectedClientIDs.begin(), DisconnectedClientIDs.end(), GetID());
	if(iClient != DisconnectedClientIDs.end())
	{
		DisconnectedClientIDs.erase(iClient);
	}
	iClient = find(ConnectedClientIDs.begin(), ConnectedClientIDs.end(), GetID());
	if(iClient != ConnectedClientIDs.end())
	{
		ConnectedClientIDs.erase(iClient);
	}
	if(bAskForKnownClients == true)
	{
		KnownClientsMessage = boost::shared_ptr< DiED::BasicMessage >(new DiED::KnownClientsMessage(0, ConnectedClientIDs, DisconnectedClientIDs));
	}
	else
	{
		DiED::messageid_t MessageID(rand());
		
		KnownClientsMessage = boost::shared_ptr< DiED::BasicMessage >(new DiED::KnownClientsMessage(MessageID, ConnectedClientIDs, DisconnectedClientIDs));
		std::cout << "TODO: What to do with the MessageID? " << __FILE__ << ':' << __LINE__ << std::endl;
	}
	vSend(KnownClientsMessage);
}

void DiED::Client::vClientsRegistered(const DiED::messageid_t & MessageID)
{
	vSend(boost::shared_ptr< DiED::BasicMessage >(new DiED::ClientsRegisteredMessage(MessageID)));
}

void DiED::Client::vConnectionEstablished(const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort)
{
	vSend(boost::shared_ptr< DiED::BasicMessage >(new DiED::ConnectionEstablishedMessage(ClientID, ClientAddress, ClientPort)));
}

void DiED::Client::vConnectionLost(const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort)
{
	vSend(boost::shared_ptr< DiED::BasicMessage >(new DiED::ConnectionLostMessage(ClientID, ClientAddress, ClientPort)));
}

void DiED::Client::vPing(sigc::slot< void > PongSlot)
{
	DiED::messageid_t PingID;
	
	do
	{
		PingID = rand();
	} while(m_PongSignals.find(PingID) != m_PongSignals.end());
	m_PongSignals[PingID] = boost::shared_ptr< sigc::signal< void > >(new sigc::signal< void >());
	m_PongSignals[PingID]->connect(PongSlot);
	vSend(boost::shared_ptr< DiED::BasicMessage >(new DiED::PingMessage(PingID)));
}

void DiED::Client::vBytesSent(size_t stSize)
{
	// TODO
}
