#include "Server.h"

DiED::Server::Server(boost::shared_ptr< Network::MessageFactory > MessageFactory) :
	Network::Server< Network::MessageStream >(MessageFactory)
{
}
