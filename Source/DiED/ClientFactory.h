#ifndef DIED_CLIENTFACTORY_H
#define DIED_CLIENTFACTORY_H

#include <boost/shared_ptr.hpp>

#include <Network/SocketFactory.h>

#include "InternalEnvironment.h"

namespace DiED
{
	class ClientFactory : public Network::SocketFactory
	{
	public:
		ClientFactory(DiED::InternalEnvironment & InternalEnvironment);
		virtual ~ClientFactory(void);
		virtual boost::shared_ptr< Network::Socket > GetSocket(int iSocket);
	protected:
		DiED::InternalEnvironment & m_InternalEnvironment;
	};
}

#endif
