#include "ClientFactory.h"

#include "Client.h"

DiED::ClientFactory::ClientFactory(Network::MessageFactory & MessageFactory) :
	m_MessageFactory(MessageFactory)
{
}

boost::shared_ptr< Network::Socket > DiED::ClientFactory::GetSocket(int iSocket)
{
	return boost::shared_ptr< Network::Socket >(new DiED::Client(iSocket, m_MessageFactory));
}
