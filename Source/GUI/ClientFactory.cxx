#include "ClientFactory.h"

#include "Client.h"

GUI::ClientFactory::ClientFactory(DiED::InternalEnvironment & InternalEnvironment) :
	DiED::ClientFactory(InternalEnvironment)
{
//~ 	std::cout << "[GUI/ClientFactory]: Create ClientFactory." << std::endl;
}

GUI::ClientFactory::~ClientFactory(void)
{
//~ 	std::cout << "[GUI/ClientFactory]: Delete ClientFactory." << std::endl;
}

boost::shared_ptr< Network::Socket > GUI::ClientFactory::GetSocket(void)
{
	return boost::shared_ptr< Network::Socket >(new GUI::Client(m_InternalEnvironment));
}

boost::shared_ptr< Network::Socket > GUI::ClientFactory::GetSocket(int iSocket)
{
	return boost::shared_ptr< Network::Socket >(new GUI::Client(iSocket, m_InternalEnvironment));
}
