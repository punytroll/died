#ifndef SOCKETFACTORY_H
#define SOCKETFACTORY_H

#include <boost/shared_ptr.hpp>

#include "Socket.h"

namespace Network
{
	class SocketFactory
	{
	public:
		virtual boost::shared_ptr< Network::Socket > GetSocket(void) = 0;
		virtual boost::shared_ptr< Network::Socket > GetSocket(int iSocket) = 0;
	};
}

#endif
