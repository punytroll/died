#ifndef DIED_SERVER_H
#define DIED_SERVER_H

#include <Network/Server.h>

namespace DiED
{
	class Server : public Network::Server
	{
	public:
		Server(Network::SocketFactory & ClientFactory);
	};
}

#endif
