#include "Server.h"

#include <fcntl.h>
#include <netinet/ip.h>
#include <sys/socket.h>

Network::Server::Server(void)
{
}

void Network::Server::vOpen(const Network::port_t & ServicePort)
{
	if(bIsOpen() == true)
	{
		return;
	}
	// -1 instead of g_iInvalidSocket because the two things are not connected to each other
	if((m_iSocket = ::socket(PF_INET, SOCK_STREAM, 0)) == -1)
	{
		m_iSocket = g_iInvalidSocket;
		vGetError();
		
		return;
	}
	
	sockaddr SocketAddress;
	sockaddr_in & SocketInformation = reinterpret_cast< sockaddr_in & >(SocketAddress);
	
	SocketInformation.sin_family = PF_INET;
	SocketInformation.sin_port = htons(ServicePort);
	SocketInformation.sin_addr.s_addr = INADDR_ANY;
	if(::bind(m_iSocket, &SocketAddress, sizeof(sockaddr_in)) == -1)
	{
		::close(m_iSocket);
		m_iSocket = g_iInvalidSocket;
		vGetError();
		
		return;
	}
	if(::listen(m_iSocket, 5) == -1)
	{
		::close(m_iSocket);
		m_iSocket = g_iInvalidSocket;
		vGetError();
		
		return;
	}
	if(::fcntl(m_iSocket, F_SETFL, O_NONBLOCK) == -1)
	{
		::close(m_iSocket);
		m_iSocket = g_iInvalidSocket;
		vGetError();
		
		return;
	}
	vMonitor();
}

void Network::Server::vSetSocketFactory(boost::shared_ptr< Network::SocketFactory > SocketFactory)
{
	m_SocketFactory = SocketFactory;
}

void Network::Server::vOnIn(void)
{
	if(m_SocketFactory)
	{
		Accepted(m_SocketFactory->GetSocket(accept(m_iSocket, 0, 0)));
	}
}
