#include "Client.h"

#include <iostream>

DiED::Client::Client(Network::MessageFactory & MessageFactory) :
	Network::MessageStream(MessageFactory)
{
	std::cout << "[Client]: Created new Client." << std::endl;
}

DiED::Client::Client(int iSocket, Network::MessageFactory & MessageFactory) :
	Network::MessageStream(iSocket, MessageFactory)
{
	std::cout << "[Client]: Created new Client from socket." << std::endl;
}
