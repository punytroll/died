#include "Client.h"

#include <iostream>

#include "BasicMessage.h"

DiED::Client::Client(boost::shared_ptr< Network::MessageFactory > MessageFactory, DiED::InternalEnvironment & InternalEnvironment) :
	Network::MessageStream(MessageFactory),
	m_InternalEnvironment(InternalEnvironment)
{
	std::cout << "[DiED/Client]: Created new Client." << std::endl;
}

DiED::Client::Client(int iSocket, boost::shared_ptr< Network::MessageFactory > MessageFactory, DiED::InternalEnvironment & InternalEnvironment) :
	Network::MessageStream(iSocket, MessageFactory),
	m_InternalEnvironment(InternalEnvironment)
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

void DiED::Client::vMessageReady(void)
{
	boost::shared_ptr< Network::BasicMessage > Message(rbegin());
	
//~ 	std::cout << "[DiED/Client]: Retrieved message with type " << Message->u32GetType() << '.' << std::endl;
	vExecuteTopMessage();
}

void DiED::Client::vExecuteTopMessage(void)
{
	boost::shared_ptr< Network::BasicMessage > Message(PopMessage());
	
//~ 	std::cout << "[DiED/Client]: Executing message with type " << Message->u32GetType() << '.' << std::endl;
	
	DiED::BasicMessage & DiEDMessage = dynamic_cast< DiED::BasicMessage & >(*Message);
	
	DiEDMessage.vExecute(*this);
}
