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

boost::shared_ptr< DiED::Client > DiED::ClientFactory::GetClient(void)
{
	return boost::shared_ptr< DiED::Client >(new DiED::Client(m_InternalEnvironment));
}
