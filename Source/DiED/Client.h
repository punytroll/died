#ifndef DIED_CLIENT_H
#define DIED_CLIENT_H

#include <Network/MessageStream.h>

namespace DiED
{
	class Client : public Network::MessageStream
	{
	public:
		Client(Network::MessageFactory & MessageFactory);
		Client(int iSocket, Network::MessageFactory & MessageFactory);
	};
}

#endif
