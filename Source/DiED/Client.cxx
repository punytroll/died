#include "Client.h"

#include <iostream>

#include "BasicMessage.h"

DiED::Client::Client(DiED::InternalEnvironment & InternalEnvironment) :
	Network::MessageStream(InternalEnvironment.GetMessageFactory()),
	m_InternalEnvironment(InternalEnvironment),
	m_u32KnownClientsMessageID(0)
{
	std::cout << "[DiED/Client]: Created new Client." << std::endl;
}

DiED::Client::Client(int iSocket, DiED::InternalEnvironment & InternalEnvironment) :
	Network::MessageStream(iSocket, InternalEnvironment.GetMessageFactory()),
	m_InternalEnvironment(InternalEnvironment),
	m_u32KnownClientsMessageID(0)
{
	std::cout << "[DiED/Client]: Created new Client from socket." << std::endl;
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
//~ 	std::cout << "[DiED/Client]: Retrieved message with type " << Message->u32GetType() << '.' << std::endl;
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
	boost::shared_ptr< Network::BasicMessage > Message(front());
	
//~ 	std::cout << "[DiED/Client]: Executing message with type " << Message->u32GetType() << '.' << std::endl;
	
	DiED::BasicMessage & DiEDMessage = dynamic_cast< DiED::BasicMessage & >(*Message);
	
	DiEDMessage.vExecute(*this);
	vOnMessageExecuted();
	pop_front();
}
