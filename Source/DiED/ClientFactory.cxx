#include "ClientFactory.h"

#include "Client.h"

DiED::ClientFactory::ClientFactory(DiED::InternalEnvironment & InternalEnvironment) :
	m_InternalEnvironment(InternalEnvironment)
{
}

DiED::ClientFactory::ClientFactory(boost::shared_ptr< Network::MessageFactory > MessageFactory, DiED::InternalEnvironment & InternalEnvironment) :
	m_InternalEnvironment(InternalEnvironment),
	m_MessageFactory(MessageFactory)
{
}

DiED::ClientFactory::~ClientFactory(void)
{
}

void DiED::ClientFactory::vSetMessageFactory(boost::shared_ptr< Network::MessageFactory > MessageFactory)
{
	m_MessageFactory = MessageFactory;
}

boost::shared_ptr< Network::Socket > DiED::ClientFactory::GetSocket(int iSocket)
{
	return boost::shared_ptr< Network::Socket >(new DiED::Client(iSocket, m_MessageFactory, m_InternalEnvironment));
}
