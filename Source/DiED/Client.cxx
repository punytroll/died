#include "Client.h"

#include <iostream>

#include <boost/weak_ptr.hpp>

#include "BasicMessage.h"

const u_int16_t g_u16TimeOutMilliSeconds = 2000;

DiED::Client::Client(DiED::InternalEnvironment & InternalEnvironment) :
	m_InternalEnvironment(InternalEnvironment),
	m_Address("localhost"),
	m_Port(0),
	m_StatusMessageCounter(0),
	m_EventCounter(1),
	m_stBytesSent(0)
{
//~ 	std::cout << "[DiED/Client]: Created new Client." << std::endl;
}

DiED::Client::~Client(void)
{
//~ 	std::cout << "[DiED/Client]: Deleted Client." << std::endl;
}

void DiED::Client::vSetMessageStream(boost::shared_ptr< Network::MessageStream > MessageStream)
{
	std::cout << "Setting the MessageStream for " << GetID() << " to " << MessageStream.get() << std::endl;
	// disconnect old relations to the current MessageStream
	m_BytesSentConnection.disconnect();
	m_MessageBeginConnection.disconnect();
	m_MessageReadyConnection.disconnect();
	m_OnDisconnectedConnection.disconnect();
	// set the new MessageStream (the old one will get deleted by boost::shared_ptr if appropriate)
	if(m_MessageStream.get() != MessageStream.get())
	{
		m_QueuedQueue.erase(m_QueuedQueue.begin(), m_QueuedQueue.end());
	}
	m_MessageStream = MessageStream;
	// if we have a valid new MessageStream set up new relations
	if(m_MessageStream.get() != 0)
	{
		m_Address = m_MessageStream->GetAddress();
		m_BytesSentConnection = m_MessageStream->BytesSent.connect(sigc::mem_fun(*this, &DiED::Client::vBytesSent));
		m_MessageBeginConnection = m_MessageStream->MessageBegin.connect(sigc::mem_fun(*this, &DiED::Client::vOnMessageBegin));
		m_MessageReadyConnection = m_MessageStream->MessageReady.connect(sigc::mem_fun(*this, &DiED::Client::vOnMessageReady));
		m_OnDisconnectedConnection = m_MessageStream->OnDisconnected.connect(sigc::mem_fun(*this, &DiED::Client::vOnDisconnected));
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
	if((m_MessageStream.get() != 0) && (m_MessageStream->bIsOpen() == true))
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
	vHandleAnswer();
	m_InternalEnvironment.vConnectionRequest(*this, ClientID, Port);
}

void DiED::Client::vHandleConnectionAccept(const DiED::clientid_t & AccepterClientID, const DiED::clientid_t & RequesterClientID)
{
	vHandleAnswer();
	m_InternalEnvironment.vConnectionAccept(*this, AccepterClientID, RequesterClientID);
}

void DiED::Client::vHandleKnownClients(const DiED::messageid_t & MessageID, const std::vector< DiED::clientid_t > & ConnectedClientIDs, const std::vector< DiED::clientid_t > & DisconnectedClientIDs)
{
	vHandleAnswer();
	m_InternalEnvironment.vKnownClients(*this, MessageID, ConnectedClientIDs, DisconnectedClientIDs);
}

void DiED::Client::vHandleClientsRegistered(const DiED::messageid_t & MessageID)
{
	vHandleAnswer();
	m_InternalEnvironment.vClientsRegistered(*this, MessageID);
}

void DiED::Client::vHandleConnectionEstablished(const DiED::messageid_t & MessageID, const DiED::clientid_t & ClientID, const Network::address_t & _ClientAddress, const Network::port_t & ClientPort)
{
	vHandleAnswer();
	
	Network::address_t ClientAddress(_ClientAddress);
	Network::address_t ThisClientAddress(GetAddress());
	
	std::cout << "ConnectionEstablished: ClientAddress = " << ClientAddress << " ; ThisClientAddress = " << ThisClientAddress << std::endl;
	if(((ClientAddress == "127.0.0.1") || (ClientAddress == "localhost")) && (ThisClientAddress != "127.0.0.1") && (ThisClientAddress != "localhost"))
	{
		ClientAddress = ThisClientAddress;
	}
	m_InternalEnvironment.vConnectionEstablished(*this, ClientID, ClientAddress, ClientPort);
	vSend(boost::shared_ptr< DiED::BasicMessage >(new DiED::StatusConfirmMessage(MessageID)));
}

void DiED::Client::vHandleConnectionLost(const DiED::messageid_t & MessageID, const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort)
{
	vHandleAnswer();
	m_InternalEnvironment.vConnectionLost(*this, ClientID, ClientAddress, ClientPort);
	vSend(boost::shared_ptr< DiED::BasicMessage >(new DiED::StatusConfirmMessage(MessageID)));
}

void DiED::Client::vHandleStatusConfirm(const DiED::messageid_t & MessageID)
{
	vHandleAnswer();
}

void DiED::Client::vHandlePing(const DiED::messageid_t & PingID)
{
	vHandleAnswer();
	vSend(boost::shared_ptr< DiED::BasicMessage >(new DiED::PongMessage(PingID)));
}

void DiED::Client::vHandlePong(const DiED::messageid_t & PingID)
{
	vHandleAnswer();
}

void DiED::Client::vHandleEvent(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID, boost::shared_ptr< DiED::EventAction > EventAction)
{
//~ 	vHandleAnswer();
	vSend(boost::shared_ptr< DiED::BasicMessage >(new DiED::EventReceivedMessage(CreatorID, EventID)));
	
	DiED::Client * pCreator(dynamic_cast< DiED::Client * >(m_InternalEnvironment.pGetClient(CreatorID)));
	
	if(pCreator == 0)
	{
		std::cout << "VERY BAD: " << __FILE__ << ':' << __LINE__ << std::endl;
		
		return;
	}
	if(pCreator == m_InternalEnvironment.pGetClient(0))
	{
		return;
	}
	if(EventID < pCreator->m_EventCounter)
	{
		if(LostClientID == 0)
		{
			return;
		}
		else
		{
			boost::shared_ptr< DiED::BasicMessage > Message(EventAction->GetMessage(CreatorID, EventID, 0));
			DiED::Client * pLostClient(dynamic_cast< DiED::Client * >(m_InternalEnvironment.pGetClient(LostClientID)));
			
			if(pLostClient != 0)
			{
				pLostClient->vSend(Message);
			}
			if((pLostClient == 0) || (m_InternalEnvironment.GetStatus(LostClientID, 0) == DiED::Disconnected))
			{
				std::set< DiED::clientid_t > ConnectedClientIDs(m_InternalEnvironment.GetConnectedClientIDs());
				std::set< DiED::clientid_t >::iterator iClient(ConnectedClientIDs.begin());
				
				while(iClient != ConnectedClientIDs.end())
				{
					if(*iClient != CreatorID)
					{
						DiED::Client * pClient(dynamic_cast< DiED::Client * >(m_InternalEnvironment.pGetClient(*iClient)));
						
						if(pClient == 0)
						{
							std::cout << "VERY BAD: " << __FILE__ << ':' << __LINE__ << std::endl;
						}
						else
						{
							pClient->vSend(Message);
						}
					}
					++iClient;
				}
			}
		}
	}
	else
	{
		boost::shared_ptr< DiED::BasicMessage > Message(EventAction->GetMessage(CreatorID, EventID, 0));
		std::set< DiED::clientid_t > ClientIDs(GetDisconnectedClientIDs());
		
		if((LostClientID != 0) && (GetStatus(LostClientID) == DiED::Connected))
		{
			ClientIDs.insert(LostClientID);
		}
		
		std::set< DiED::clientid_t > DisconnectedClientIDs(m_InternalEnvironment.GetDisconnectedClientIDs());
		std::set< DiED::clientid_t > Set;
		
		std::set_intersection(DisconnectedClientIDs.begin(), DisconnectedClientIDs.end(), ClientIDs.begin(), ClientIDs.end(), std::inserter(Set, Set.end()));
		
		std::set< DiED::clientid_t >::iterator iClient;
		std::set< DiED::clientid_t >::iterator iClientEnd;
		
		if(Set.size() != 0)
		{
			std::set< DiED::clientid_t > ConnectedClientIDs(m_InternalEnvironment.GetConnectedClientIDs());
			
			Set.erase(Set.begin(), Set.end());
			std::set_union(ConnectedClientIDs.begin(), ConnectedClientIDs.end(), ClientIDs.begin(), ClientIDs.end(), std::inserter(Set, Set.end()));
			iClient = Set.begin();
			iClientEnd = Set.end();
		}
		else
		{
			iClient = ClientIDs.begin();
			iClientEnd = ClientIDs.end();
		}
		while(iClient != iClientEnd)
		{
			if(*iClient != CreatorID)
			{
				DiED::Client * pClient(dynamic_cast< DiED::Client * >(m_InternalEnvironment.pGetClient(*iClient)));
				
				if(pClient == 0)
				{
					std::cout << "VERY BAD: " << __FILE__ << ':' << __LINE__ << std::endl;
				}
				else
				{
					pClient->vSend(Message);
				}
			}
			++iClient;
		}
		pCreator->vAddEventAction(EventID, EventAction);
	}
}

void DiED::Client::vAddEventAction(const DiED::messageid_t & EventID, boost::shared_ptr< DiED::EventAction > EventAction)
{
	std::map< DiED::messageid_t, boost::shared_ptr< DiED::EventAction > >::iterator iAction(m_ActionBuffer.find(EventID));
	
	if(iAction == m_ActionBuffer.end())
	{
		m_ActionBuffer[EventID] = EventAction;
	}
	do
	{
		iAction = m_ActionBuffer.begin();
		if(iAction->first >= m_EventCounter)
		{
			break;
		}
		m_ActionBuffer.erase(iAction);
	} while(true);
	do
	{
		if((iAction == m_ActionBuffer.end()) || (iAction->first > m_EventCounter))
		{
			break;
		}
		++m_EventCounter;
		iAction->second->vExecute(*this);
		m_ActionBuffer.erase(iAction);
		iAction = m_ActionBuffer.begin();
	} while(true);
}

void DiED::Client::vHandleEventReceived(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID)
{
	// TODO: not specified yet, but I'm taking an educated guess
	vHandleAnswer();
}

void DiED::Client::vHandleAnswer(void)
{
	if(m_AwaitingConfirmationQueue.size() == 0)
	{
		return;
	}
	
//~ 	std::cout << "ClientID = " << m_InternalEnvironment.pGetClient(0)->GetID() << std::endl;
	std::cout << "HandleAnswers: " << m_AwaitingConfirmationQueue.size() << " elements waiting for confirmation from " << GetID() << "." << std::endl;
	std::deque< DiED::Client::WaitingMessage >::iterator iDebugMessage(m_AwaitingConfirmationQueue.begin());
	
	while(iDebugMessage != m_AwaitingConfirmationQueue.end())
	{
		std::cout << "                   " << iDebugMessage->m_Message->sGetString() << std::endl;
		++iDebugMessage;
	}
	
	// TODO: this front() does not look good
	RemoveWaitingMessage(boost::dynamic_pointer_cast< DiED::BasicMessage >(m_MessageStream->front())->GetConfirmationParameters());
	std::cout << "             : " << m_AwaitingConfirmationQueue.size() << " elements." << std::endl;
}

DiED::Client::WaitingMessage DiED::Client::RemoveWaitingMessage(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters)
{
	DiED::Client::WaitingMessage Return;
	
	if(ConfirmationParameters.get() != 0)
	{
		std::deque< DiED::Client::WaitingMessage >::iterator iMessage(m_AwaitingConfirmationQueue.begin());
		
		while(iMessage != m_AwaitingConfirmationQueue.end())
		{
			if(iMessage->m_Message->bIsConfirmedBy(ConfirmationParameters) == true)
			{
				Return = *iMessage;
				m_AwaitingConfirmationQueue.erase(iMessage);
				
				break;
			}
			++iMessage;
		}
	}
	
	return Return;
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
	
	boost::shared_ptr< DiED::BasicMessage > Message(boost::dynamic_pointer_cast< DiED::BasicMessage >(*iMessage));
	boost::shared_ptr< Network::MessageStream > MessageStream(m_MessageStream);
	
	Message->vExecute(*this);
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
			m_MessageStream->operator<<(*Message);
			if(Message->bRequiresConfirmation() == true)
			{
				DiED::Client::WaitingMessage WaitingMessage;
				
				WaitingMessage.m_Message = Message;
				m_AwaitingConfirmationQueue.push_back(WaitingMessage);
			}
			m_QueuedQueue.push_back(Message);
			MessageQueued(Message);
		}
		else
		{
			std::cout << "BAD: " << __FILE__ << ':' << __LINE__ << std::endl;
		}
	}
	if(m_InternalEnvironment.GetStatus(GetID()) == DiED::Connected)
	{
		if((m_MessageStream.get() != 0) && (m_MessageStream->bIsOpen() == true))
		{
			std::deque< boost::shared_ptr< DiED::EventMessage > >::iterator iEventMessage(m_EventQueue.begin());
			
			while(iEventMessage != m_EventQueue.end())
			{
				m_MessageStream->operator<<(**iEventMessage);
				if((*iEventMessage)->bRequiresConfirmation() == true)
				{
					DiED::Client::WaitingMessage WaitingMessage;
					
					WaitingMessage.m_Message = *iEventMessage;
					m_AwaitingConfirmationQueue.push_back(WaitingMessage);
				}
				m_QueuedQueue.push_back(*iEventMessage);
				MessageQueued(*iEventMessage);
				m_EventQueue.erase(iEventMessage);
				iEventMessage = m_EventQueue.begin();
			}
		}
		else
		{
			std::cout << "BAD: " << __FILE__ << ':' << __LINE__ << std::endl;
		}
	}
}

void DiED::Client::vConnectionRequest(const DiED::clientid_t & ClientID, const Network::port_t & ListenPort)
{
	vSend(boost::shared_ptr< DiED::BasicMessage >(new DiED::ConnectionRequestMessage(ClientID, ListenPort)));
}

void DiED::Client::vConnectionAccept(const DiED::clientid_t & AccepterClientID, const DiED::clientid_t & RequesterClientID)
{
	vSend(boost::shared_ptr< DiED::BasicMessage >(new DiED::ConnectionAcceptMessage(AccepterClientID, RequesterClientID)));
}

void DiED::Client::vKnownClients(bool bAskForKnownClients)
{
	boost::shared_ptr< DiED::BasicMessage > KnownClientsMessage;
	std::set< DiED::clientid_t > DisconnectedClientIDs(m_InternalEnvironment.GetDisconnectedClientIDs());
	std::set< DiED::clientid_t > ConnectedClientIDs(m_InternalEnvironment.GetConnectedClientIDs());
	std::set< DiED::clientid_t >::iterator iClient;
	
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
		KnownClientsMessage = boost::shared_ptr< DiED::BasicMessage >(new DiED::KnownClientsMessage(rand(), ConnectedClientIDs, DisconnectedClientIDs));
	}
	vSend(KnownClientsMessage);
}

void DiED::Client::vClientsRegistered(const DiED::messageid_t & MessageID)
{
	vSend(boost::shared_ptr< DiED::BasicMessage >(new DiED::ClientsRegisteredMessage(MessageID)));
}

void DiED::Client::vConnectionEstablished(const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort)
{
	vSend(boost::shared_ptr< DiED::BasicMessage >(new DiED::ConnectionEstablishedMessage(++m_StatusMessageCounter, ClientID, ClientAddress, ClientPort)));
}

void DiED::Client::vConnectionLost(const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort)
{
	vSend(boost::shared_ptr< DiED::BasicMessage >(new DiED::ConnectionLostMessage(++m_StatusMessageCounter, ClientID, ClientAddress, ClientPort)));
}

void DiED::Client::vPing(sigc::slot< void > PongSlot)
{
	vSend(boost::shared_ptr< DiED::BasicMessage >(new DiED::PingMessage(++m_StatusMessageCounter)));
	
	if(m_AwaitingConfirmationQueue.size() > 0)
	{
		DiED::Client::WaitingMessage & WaitingPing(m_AwaitingConfirmationQueue.back());
		
		WaitingPing.m_TimeoutSignal = boost::shared_ptr< sigc::signal< void > >(new sigc::signal< void >());
		WaitingPing.m_TimeoutSignal->connect(PongSlot);
	}
}

void DiED::Client::vPing(void)
{
	vSend(boost::shared_ptr< DiED::BasicMessage >(new DiED::PingMessage(++m_StatusMessageCounter)));
}

void DiED::Client::vInsertText(const Glib::ustring & sText, const DiED::messageid_t & EventID)
{
	vSend(boost::shared_ptr< DiED::BasicMessage >(new DiED::InsertTextEvent(m_InternalEnvironment.pGetClient(0)->GetID(), EventID, 0, sText)));
}

DiED::messageid_t DiED::Client::GetNextEventCounter(void)
{
	return m_EventCounter++;
}

void DiED::Client::vBytesSent(size_t stSize)
{
	m_stBytesSent += stSize;
//~ 	std::cout << "[DiED/Client]: Client " << GetID() << " has sent " << stSize << " bytes over the socket:" << std::endl;
	
	std::deque< boost::shared_ptr< DiED::BasicMessage > >::iterator iMessage(m_QueuedQueue.begin());
	
	while(iMessage != m_QueuedQueue.end())
	{
		size_t stMessageSize((*iMessage)->stGetSize());
		
//~ 		std::cout << "\tMessage " << (*iMessage)->sGetString() << " has size " << stMessageSize;
		if(m_stBytesSent < stMessageSize)
		{
//~ 			std::cout << " ... Aborting." << std::endl;
			
			return;
		}
		m_stBytesSent -= stMessageSize;
//~ 		std::cout << " ... " << m_stBytesSent << " bytes remaining." << std::endl;
		MessageSent(*iMessage);
		if((*iMessage)->bRequiresConfirmation() == true)
		{
			(*iMessage)->vInitiateTimeout(this, g_u16TimeOutMilliSeconds);
		}
		m_QueuedQueue.erase(iMessage);
		iMessage = m_QueuedQueue.begin();
	}
}

void DiED::Client::vOnDisconnected(void)
{
	m_InternalEnvironment.vSetStatus(0, GetID(), DiED::Disconnected);
}

void DiED::Client::vSetStatus(const DiED::clientid_t & ClientID, DiED::clientstatus_t Status)
{
	DiED::User::vSetStatus(ClientID, Status);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///                          ConfirmationTimeout-Handler                                        ///
///////////////////////////////////////////////////////////////////////////////////////////////////

void DiED::Client::vHandleConnectionRequestConfirmationTimeout(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters)
{
	DiED::Client::WaitingMessage WaitingMessage(RemoveWaitingMessage(ConfirmationParameters));
	
	if(WaitingMessage.m_Message.get() == 0)
	{
		std::cout << "VERY BAD: " << __FILE__ << ':' << __LINE__ << std::endl;
		
		return;
	}
	if(WaitingMessage.m_TimeoutSignal.get() != 0)
	{
		WaitingMessage.m_TimeoutSignal->emit();
	}
	if(m_MessageStream.get() != 0)
	{
		m_MessageStream->vClose();
	}
}

void DiED::Client::vHandleKnownClientsConfirmationTimeout(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters)
{
	DiED::Client::WaitingMessage WaitingMessage(RemoveWaitingMessage(ConfirmationParameters));
	
	if(WaitingMessage.m_Message.get() == 0)
	{
		std::cout << "VERY BAD: " << __FILE__ << ':' << __LINE__ << std::endl;
		
		return;
	}
	if(WaitingMessage.m_TimeoutSignal.get() != 0)
	{
		WaitingMessage.m_TimeoutSignal->emit();
	}
	if(m_MessageStream.get() != 0)
	{
		m_MessageStream->vClose();
	}
}

void DiED::Client::vHandleConnectionEstablishedConfirmationTimeout(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters)
{
	DiED::Client::WaitingMessage WaitingMessage(RemoveWaitingMessage(ConfirmationParameters));
	
	if(WaitingMessage.m_Message.get() == 0)
	{
		std::cout << "VERY BAD: " << __FILE__ << ':' << __LINE__ << std::endl;
		
		return;
	}
	if(WaitingMessage.m_TimeoutSignal.get() != 0)
	{
		WaitingMessage.m_TimeoutSignal->emit();
	}
	if(m_MessageStream.get() != 0)
	{
		m_MessageStream->vClose();
	}
	m_InternalEnvironment.vAnnounceConnectionLost(GetID());
}

void DiED::Client::vHandleConnectionLostConfirmationTimeout(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters)
{
	DiED::Client::WaitingMessage WaitingMessage(RemoveWaitingMessage(ConfirmationParameters));
	
	if(WaitingMessage.m_Message.get() == 0)
	{
		std::cout << "VERY BAD: " << __FILE__ << ':' << __LINE__ << std::endl;
		
		return;
	}
	if(WaitingMessage.m_TimeoutSignal.get() != 0)
	{
		WaitingMessage.m_TimeoutSignal->emit();
	}
	if(m_MessageStream.get() != 0)
	{
		m_MessageStream->vClose();
	}
	m_InternalEnvironment.vAnnounceConnectionLost(GetID());
}

void DiED::Client::vHandlePingConfirmationTimeout(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters)
{
	DiED::Client::WaitingMessage WaitingMessage(RemoveWaitingMessage(ConfirmationParameters));
	
	if(WaitingMessage.m_Message.get() == 0)
	{
		std::cout << "VERY BAD: " << __FILE__ << ':' << __LINE__ << std::endl;
		
		return;
	}
	if(WaitingMessage.m_TimeoutSignal.get() != 0)
	{
		WaitingMessage.m_TimeoutSignal->emit();
	}
	if(m_MessageStream.get() != 0)
	{
		m_MessageStream->vClose();
	}
}

void DiED::Client::vHandleEventConfirmationTimeout(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters)
{
	DiED::Client::WaitingMessage WaitingMessage(RemoveWaitingMessage(ConfirmationParameters));
	
	if(WaitingMessage.m_Message.get() == 0)
	{
		std::cout << "VERY BAD: " << __FILE__ << ':' << __LINE__ << std::endl;
		
		return;
	}
	if(WaitingMessage.m_TimeoutSignal.get() != 0)
	{
		WaitingMessage.m_TimeoutSignal->emit();
	}
	if(m_MessageStream.get() != 0)
	{
		m_MessageStream->vClose();
	}
	m_InternalEnvironment.vAnnounceConnectionLost(GetID());
	m_EventQueue.push_front(boost::dynamic_pointer_cast< DiED::EventMessage >(WaitingMessage.m_Message));
	
	boost::shared_ptr< DiED::EventMessage > NewMessage(boost::dynamic_pointer_cast< DiED::EventMessage >(WaitingMessage.m_Message->Clone()));
	
	NewMessage->vSetLostClientID(GetID());
	m_InternalEnvironment.vAnnounceMessage(NewMessage);
}
