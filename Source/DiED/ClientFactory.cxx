#include "ClientFactory.h"

#include "Client.h"

DiED::ClientFactory::ClientFactory(DiED::InternalEnvironment & InternalEnvironment) :
	m_InternalEnvironment(InternalEnvironment)
{
}

//~ DiED::ClientFactory::~ClientFactory(void)
//~ {
//~ }

//~ boost::shared_ptr< Network::Socket > DiED::ClientFactory::GetSocket(int iSocket)
//~ {
//~ 	return boost::shared_ptr< Network::Socket >(new DiED::Client(iSocket, m_InternalEnvironment.GetMessageFactory(), m_InternalEnvironment));
//~ }
