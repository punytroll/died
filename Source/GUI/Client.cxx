#include "Client.h"

GUI::Client::Client(boost::shared_ptr< Network::MessageFactory > MessageFactory, DiED::InternalEnvironment & InternalEnvironment) :
	DiED::Client(MessageFactory, InternalEnvironment),
	m_bHoldMessagesBack(false)
{
//~ 	std::cout << "[GUI/Client]: Created new Client." << std::endl;
}

GUI::Client::Client(int iSocket, boost::shared_ptr< Network::MessageFactory > MessageFactory, DiED::InternalEnvironment & InternalEnvironment) :
	DiED::Client(iSocket, MessageFactory, InternalEnvironment),
	m_bHoldMessagesBack(false)
{
//~ 	std::cout << "[GUI/Client]: Created new Client from socket." << std::endl;
}

GUI::Client::~Client(void)
{
	std::cout << "[GUI/Client]: Deleted Client." << std::endl;
}

void GUI::Client::vMessageReady(void)
{
	if(m_bHoldMessagesBack == false)
	{
		DiED::Client::vMessageReady();
	}
}
