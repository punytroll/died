#ifndef DIED_CLIENTFACTORY_H
#define DIED_CLIENTFACTORY_H

#include <boost/shared_ptr.hpp>

#include <Network/MessageFactory.h>
#include <Network/SocketFactory.h>

namespace DiED
{
	class ClientFactory : public Network::SocketFactory
	{
	public:
		ClientFactory(Network::MessageFactory & MessageFactory);
		virtual boost::shared_ptr< Network::Socket > GetSocket(int iSocket);
	private:
		Network::MessageFactory & m_MessageFactory;
	};
}

#endif
