#ifndef SERVER_H
#define SERVER_H

#include "Socket.h"
#include "SocketFactory.h"

namespace Network
{
	class Server : public Network::Socket
	{
	public:
		// constructors and destructors
		Server(void);
		
		// connection
		void vOpen(const Network::port_t & ServicePort);
		
		// setting the socket factory
		void vSetSocketFactory(boost::shared_ptr< Network::SocketFactory > SocketFactory);
		
		// signal is fired when a new connection has been established via the socket
		sigc::signal< void, boost::shared_ptr< Network::Socket > > Accepted;
	protected:
		virtual void vOnIn(void);
	private:
		boost::shared_ptr< Network::SocketFactory > m_SocketFactory;
	};
}

#endif
