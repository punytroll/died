#include "ClientFactory.h"

#include "Client.h"

GUI::ClientFactory::ClientFactory(DiED::InternalEnvironment & InternalEnvironment) :
	DiED::ClientFactory(InternalEnvironment)
{
	std::cout << "[GUI/ClientFactory]: Create ClientFactory." << std::endl;
}

GUI::ClientFactory::~ClientFactory(void)
{
	std::cout << "[GUI/ClientFactory]: Delete ClientFactory." << std::endl;
}

boost::shared_ptr< DiED::Client > GUI::ClientFactory::GetClient(void)
{
//~ 	std::cout << "[GUI/ClientFactory]: Creating a client." << std::endl;
	
	return boost::shared_ptr< DiED::Client >(new GUI::Client(m_InternalEnvironment));
}
