#ifndef SERVER_H
#define SERVER_H

#include "Socket.h"
#include "SocketFactory.h"

namespace Network
{
	class Server : public Socket
	{
	public:
		Server(void);
		void vSetSocketFactory(boost::shared_ptr< Network::SocketFactory > SocketFactory);
		void vOpen(u_int16_t u16ServicePort);
		sigc::signal< void, boost::shared_ptr< Network::Socket > > Accepted;
	protected:
		virtual void vOnIn(void);
	private:
		boost::shared_ptr< Network::SocketFactory > m_SocketFactory;
	};
}

#endif
