#ifndef GUI_CLIENTFACTORY_H
#define GUI_CLIENTFACTORY_H

#include <DiED/InternalEnvironment.h>
#include <Network/SocketFactory.h>

namespace GUI
{
	class ClientFactory : public Network::SocketFactory
	{
	public:
		ClientFactory(DiED::InternalEnvironment & InternalEnvironment);
		virtual boost::shared_ptr< Network::Socket > GetSocket(int iSocket);
	private:
		DiED::InternalEnvironment & m_InternalEnvironment;
	};
}

#endif
