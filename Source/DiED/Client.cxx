#include "Client.h"

#include <iostream>

#include "BasicMessage.h"

DiED::Client::Client(DiED::InternalEnvironment & InternalEnvironment) :
	m_u32KnownClientsMessageID(0),
	m_InternalEnvironment(InternalEnvironment)
{
	std::cout << "[DiED/Client]: Created new Client." << std::endl;
}

DiED::Client::~Client(void)
{
	std::cout << "[DiED/Client]: Deleted Client." << std::endl;
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
	
	DiEDMessage.vExecute(*this);
	vOnMessageExecuted();
	m_MessageStream->pop_front();
}

void DiED::Client::vSetMessageStream(boost::shared_ptr< Network::MessageStream > MessageStream)
{
//~ 	std::cout << "[DiED/Client]: Setting MessageStream." << std::endl;
	m_MessageStream = MessageStream;
	m_MessageStream->MessageBegin.connect(sigc::mem_fun(*this, &DiED::Client::vOnMessageBegin));
	m_MessageStream->MessageReady.connect(sigc::mem_fun(*this, &DiED::Client::vOnMessageReady));
}

DiED::Client & DiED::Client::operator<<(const Network::BasicMessage & Message)
{
	if(m_MessageStream.get() != 0)
	{
		m_MessageStream->operator<<(Message);
	}
	
	return *this;
}
