#include "ClientFactory.h"

#include "Client.h"

DiED::ClientFactory::ClientFactory(DiED::InternalEnvironment & InternalEnvironment) :
	m_InternalEnvironment(InternalEnvironment)
{
//~ 	std::cout << "[DiED/ClientFactory]: Create ClientFactory." << std::endl;
}

DiED::ClientFactory::~ClientFactory(void)
{
//~ 	std::cout << "[DiED/ClientFactory]: Delete ClientFactory." << std::endl;
}

boost::shared_ptr< Network::Socket > DiED::ClientFactory::GetSocket(void)
{
	return boost::shared_ptr< Network::Socket >(new DiED::Client(m_InternalEnvironment));
}

boost::shared_ptr< Network::Socket > DiED::ClientFactory::GetSocket(int iSocket)
{
	return boost::shared_ptr< Network::Socket >(new DiED::Client(iSocket, m_InternalEnvironment));
}
