#include "Stream.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <sys/socket.h>

#include <iostream>

Network::Stream::Stream(void) :
	m_pu8Buffer(new u_int8_t[2048]),
	m_Buffer(2048)
{
}

Network::Stream::Stream(int iSocket) :
	Socket(iSocket),
	m_pu8Buffer(new u_int8_t[2048]),
	m_Buffer(2048)
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

Network::Stream & Network::Stream::operator>>(Network::BasicValue & Value)
{
	m_Values.push_back(boost::ref(Value));
	
	return *this;
}

Network::Stream & Network::Stream::operator<<(const Network::BasicValue & Value)
{
	Value.vWriteTo(m_Buffer.GetWriter());
	
	return *this;
}

void Network::Stream::vOnIn(void)
{
	size_t stSize = recv(m_iSocket, m_pu8Buffer, 2048, 0);
	
	if(stSize == -1)
	{
		vGetError();
		
		return;
	}
	if(stSize == 0)
	{
		std::cout << "Remote disconnected." << std::endl;
		vClose();
		
		return;
	}
	std::cout << "Read " << stSize << " bytes from the socket." << std::endl;
	m_Buffer.vWrite(m_pu8Buffer, stSize);
	
	std::deque< boost::reference_wrapper< Network::BasicValue > >::iterator iValue(m_Values.begin());
	
	while(iValue != m_Values.end())
	{
		Network::BasicValue & Value(*iValue);
		
		vRead(Value);
		if(Value.bIsReady() == false)
		{
			break;
		}
		m_Values.erase(iValue);
		iValue = m_Values.begin();
	}
}

void Network::Stream::vRead(Network::BasicValue & Value)
{
	Value.vReadFrom(m_Buffer.GetReader());
}
