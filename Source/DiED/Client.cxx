#include "Client.h"

DiED::Client::Client(Network::MessageFactory & MessageFactory) :
	Network::MessageStream(MessageFactory)
{
}

DiED::Client::Client(int iSocket, Network::MessageFactory & MessageFactory) :
	Network::MessageStream(iSocket, MessageFactory)
{
}
