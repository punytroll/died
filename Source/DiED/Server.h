#ifndef DIED_SERVER_H
#define DIED_SERVER_H

#include <Network/MessageStream.h>
#include <Network/Server.h>

namespace DiED
{
	class Server : public Network::Server< Network::MessageStream >
	{
	public:
		Server(boost::shared_ptr< Network::MessageFactory > MessageFactory);
	};
}

#endif
