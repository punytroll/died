#ifndef DIED_CLIENTFACTORY_H
#define DIED_CLIENTFACTORY_H

#include <boost/shared_ptr.hpp>

#include <Network/MessageFactory.h>
#include <Network/SocketFactory.h>

#include "InternalEnvironment.h"

namespace DiED
{
	class ClientFactory : public Network::SocketFactory
	{
	public:
		ClientFactory(DiED::InternalEnvironment & InternalEnvironment);
		ClientFactory(boost::shared_ptr< Network::MessageFactory > MessageFactory, DiED::InternalEnvironment & InternalEnvironment);
		virtual ~ClientFactory(void);
		void vSetMessageFactory(boost::shared_ptr< Network::MessageFactory > MessageFactory);
		virtual boost::shared_ptr< Network::Socket > GetSocket(int iSocket);
	protected:
		DiED::InternalEnvironment & m_InternalEnvironment;
		boost::shared_ptr< Network::MessageFactory > m_MessageFactory;
	};
}

#endif
