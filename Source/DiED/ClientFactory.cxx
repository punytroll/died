#include "ClientFactory.h"

#include "Client.h"

DiED::ClientFactory::ClientFactory(DiED::InternalEnvironment & InternalEnvironment) :
	m_InternalEnvironment(InternalEnvironment)
{
	LOG(Object, "DiED/ClientFactory", "ClientFactory created.");
}

DiED::ClientFactory::~ClientFactory(void)
{
	LOG(Object, "DiED/ClientFactory", "ClientFactory deleted.");
}

boost::shared_ptr< DiED::Client > DiED::ClientFactory::GetClient(void)
{
	return boost::shared_ptr< DiED::Client >(new DiED::Client(m_InternalEnvironment));
}
