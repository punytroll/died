#ifndef GUI_CLIENTFACTORY_H
#define GUI_CLIENTFACTORY_H

#include <DiED/InternalEnvironment.h>
#include <DiED/ClientFactory.h>

namespace GUI
{
	class ClientFactory : public DiED::ClientFactory
	{
	public:
		ClientFactory(DiED::InternalEnvironment & InternalEnvironment);
		virtual ~ClientFactory(void);
		virtual boost::shared_ptr< Network::Socket > GetSocket(void);
		virtual boost::shared_ptr< Network::Socket > GetSocket(int iSocket);
	};
}

#endif
