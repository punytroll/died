#include "Client.h"

#include <iostream>

#include "BasicMessage.h"

DiED::Client::Client(DiED::InternalEnvironment & InternalEnvironment) :
	m_u32KnownClientsMessageID(0),
	m_InternalEnvironment(InternalEnvironment)
{
//~ 	std::cout << "[DiED/Client]: Created new Client." << std::endl;
}

DiED::Client::~Client(void)
{
//~ 	std::cout << "[DiED/Client]: Deleted Client." << std::endl;
}

void DiED::Client::vSetMessageStream(boost::shared_ptr< Network::MessageStream > MessageStream)
{
//~ 	std::cout << "[DiED/Client]: Setting MessageStream for Client " << GetClientID() << " to " << MessageStream.get() << std::endl;
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
	return m_MessageStream->GetAddress();
}

void DiED::Client::vInsertText(const Glib::ustring & sString)
{
	m_InternalEnvironment.vInsertText(*this, sString);
}

void DiED::Client::vConnectionRequest(const DiED::clientid_t & ClientID, const Network::port_t & Port)
{
	m_Port = Port;
	m_InternalEnvironment.vConnectionRequest(*this, ClientID);
}

void DiED::Client::vConnectionAccept(const DiED::clientid_t & LocalClientID, const DiED::clientid_t & RemoteClientID)
{
	m_InternalEnvironment.vConnectionAccept(*this, LocalClientID, RemoteClientID);
}

void DiED::Client::vKnownClients(const DiED::messageid_t & MessageID, const std::vector< DiED::clientid_t > & ConnectedClientIDs, const std::vector< DiED::clientid_t > & DisconnectedClientIDs)
{
	m_InternalEnvironment.vKnownClients(*this, MessageID, ConnectedClientIDs, DisconnectedClientIDs);
}

void DiED::Client::vClientsRegistered(const DiED::messageid_t & MessageID)
{
	m_InternalEnvironment.vClientsRegistered(*this, MessageID);
}

void DiED::Client::vConnectionEstablished(const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort)
{
	m_InternalEnvironment.vConnectionEstablished(*this, ClientID, ClientAddress, ClientPort);
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
	boost::shared_ptr< Network::BasicMessage > Message(m_MessageStream->front());
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

DiED::Client & DiED::Client::operator<<(boost::shared_ptr< Network::BasicMessage > Message)
{
	if(m_MessageStream.get() != 0)
	{
		m_MessageQueue.push_back(Message);
		m_MessageStream->operator<<(*Message);
	}
	else
	{
		std::cout << "VERY BAD: " << __FILE__ << ':' << __LINE__ << std::endl;
		
		throw;
	}
	
	return *this;
}

void DiED::Client::vBytesSent(size_t stSize)
{
	// TODO
}
