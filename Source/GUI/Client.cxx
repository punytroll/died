#include "Client.h"

GUI::Client::Client(DiED::InternalEnvironment & InternalEnvironment) :
	DiED::Client(InternalEnvironment),
	m_bHoldMessagesBack(false)
{
//~ 	std::cout << "[GUI/Client]: Created new Client." << std::endl;
}

GUI::Client::Client(int iSocket, DiED::InternalEnvironment & InternalEnvironment) :
	DiED::Client(iSocket, InternalEnvironment),
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
