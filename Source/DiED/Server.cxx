#include "Server.h"

DiED::Server::Server(Network::SocketFactory & ClientFactory) :
	Network::Server(ClientFactory)
{
}
