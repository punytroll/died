#include "Stream.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <sys/socket.h>

Network::Stream::Stream(void)
{
}

Network::Stream::Stream(int iSocket) :
	Socket(iSocket)
{
}

void Network::Stream::vOpen(const std::string & sConnectAddress, u_int16_t u16ConnectPort)
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
	hostent * pHostEntity = ::gethostbyname(sConnectAddress.c_str());
	
	SocketInformation.sin_port = htons(u16ConnectPort);
	if(pHostEntity != 0)
	{
		SocketInformation.sin_family = pHostEntity->h_addrtype;
		::memcpy(&(SocketInformation.sin_addr), pHostEntity->h_addr_list[0], pHostEntity->h_length);
	}
	else
	{
		if(::inet_aton(sConnectAddress.c_str(), &(SocketInformation.sin_addr)) == 0)
		{
			::close(m_iSocket);
			m_iSocket = g_iInvalidSocket;
			vGetError();
			
			return;
		}
    }
	if(::connect(m_iSocket, &SocketAddress, sizeof(sockaddr_in)) == -1)
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
