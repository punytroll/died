#ifndef DIED_CLIENTFACTORY_H
#define DIED_CLIENTFACTORY_H

#include <boost/shared_ptr.hpp>

#include "Client.h"
#include "InternalEnvironment.h"

namespace DiED
{
	class ClientFactory
	{
	public:
		ClientFactory(DiED::InternalEnvironment & InternalEnvironment);
		virtual ~ClientFactory(void);
		virtual boost::shared_ptr< DiED::Client > GetClient(void);
	protected:
		DiED::InternalEnvironment & m_InternalEnvironment;
	};
}

#endif
