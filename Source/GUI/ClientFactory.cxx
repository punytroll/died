#include "ClientFactory.h"

#include "Client.h"

GUI::ClientFactory::ClientFactory(DiED::InternalEnvironment & InternalEnvironment) :
	DiED::ClientFactory(InternalEnvironment)
{
}

boost::shared_ptr< Network::Socket > GUI::ClientFactory::GetSocket(int iSocket)
{
	return boost::shared_ptr< Network::Socket >(new GUI::Client(iSocket, m_InternalEnvironment.GetMessageFactory(), m_InternalEnvironment));
}
