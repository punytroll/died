#include "Socket.h"

#include <arpa/inet.h>
#include <netinet/ip.h>
#include <sys/socket.h>

#include <iostream>

#include <Common.h>

const int Network::g_iInvalidSocket = -1;

Network::Socket::Socket(void) :
	m_iSocket(g_iInvalidSocket),
	m_iError(0),
	m_bOnDisconnected(true)
{
//~ 	std::cout << "[Network/Socket]: Created socket." << std::endl;
}

Network::Socket::Socket(int iSocket) :
	m_iSocket(iSocket),
	m_iError(0)
{
//~ 	std::cout << "[Network/Socket]: Created socket from iSocket." << std::endl;
	vMonitor();
}

Network::Socket::~Socket(void)
{
	if(bIsOpen() == true)
	{
		vClose();
	}
	std::cout << "[Network/Socket]: Deleted socket." << std::endl;
}

bool Network::Socket::bIsOpen(void) const
{
	return m_iSocket != g_iInvalidSocket;
}

int Network::Socket::iGetError(void) const
{
	return m_iError;
}

void Network::Socket::vClose(void)
{
	vIgnoreOnOut();
	close(m_iSocket);
	m_iSocket = g_iInvalidSocket;
	if(m_bOnDisconnected == true)
	{
		OnDisconnected();
	}
}

void Network::Socket::vMonitor(void)
{
	Glib::RefPtr< Glib::IOSource > IOSource = Glib::IOSource::create(m_iSocket, Glib::IO_IN | Glib::IO_PRI | Glib::IO_ERR | Glib::IO_HUP | Glib::IO_NVAL);
		
	IOSource->connect(sigc::mem_fun(*this, &Network::Socket::bNotify));
	IOSource->attach();
}

void Network::Socket::vRequestOnOut(void)
{
	if(m_OSource == false)
	{
//~ 		std::cout << "[Socket]: OnOut requested." << std::endl;
		m_OSource = Glib::IOSource::create(m_iSocket, Glib::IO_OUT);
		m_OSource->connect(sigc::mem_fun(*this, &Network::Socket::bNotify));
		m_OSource->attach();
	}
}

void Network::Socket::vIgnoreOnOut(void)
{
//~ 	std::cout << "[Socket]: OnOut ignored." << std::endl;
	if(m_OSource == true)
	{
		m_OSource->destroy();
		m_OSource.clear();
	}
}

bool Network::Socket::bNotify(const Glib::IOCondition & Condition)
{
	if((Condition & Glib::IO_IN) != 0)
	{
//~ 		std::cout << "IO_IN" << std::endl;
		vOnIn();
	}
	if((Condition & Glib::IO_OUT) != 0)
	{
//~ 		std::cout << "IO_OUT" << std::endl;
		vOnOut();
	}
	if((Condition & Glib::IO_PRI) != 0)
	{
//~ 		std::cout << "IO_PRI" << std::endl;
	}
	if((Condition & Glib::IO_ERR) != 0)
	{
//~ 		std::cout << "IO_ERR" << std::endl;
	}
	if((Condition & Glib::IO_HUP) != 0)
	{
//~ 		std::cout << "IO_HUP" << std::endl;
	}
	if((Condition & Glib::IO_NVAL) != 0)
	{
//~ 		std::cout << "IO_NVAL" << std::endl;
		
		return false;
	}
	
	return true;
}

void Network::Socket::vOnIn(void)
{
}

void Network::Socket::vOnOut(void)
{
	// if we get here it probably is a logical programming failure
	// somebody requested a OnOut event but didn't override vOnOut function
	std::cout << "[Network/Socket]: vOnOut in " << __FILE__ << ':' << __LINE__ << " called. Forgot to override vOnOut?" << std::endl;
	//  => to avoid a busy wait state we disconnect the OnOut event again
	vIgnoreOnOut();
}

void Network::Socket::vGetError(void)
{
	m_iError = errno;
}

void Network::Socket::vSetSocket(int iSocket)
{
	if(bIsOpen() == true)
	{
		return;
	}
	m_iSocket = iSocket;
	vMonitor();
}

Network::address_t Network::Socket::GetAddress(void)
{
	sockaddr SocketAddress;
	sockaddr_in & SocketInformation = reinterpret_cast< sockaddr_in & >(SocketAddress);
	socklen_t Length = sizeof(sockaddr);
	
	if(::getpeername(m_iSocket, &SocketAddress, &Length) == 0)
	{
		return inet_ntoa(SocketInformation.sin_addr);
	}
	else
	{
		vGetError();
		std::cout << "Error with getpeername() " << sErrorCodeToString(m_iError) << std::endl;
	}
	
	return "";
}

Network::port_t Network::Socket::GetPort(void)
{
	sockaddr SocketAddress;
	sockaddr_in & SocketInformation = reinterpret_cast< sockaddr_in & >(SocketAddress);
	socklen_t Length = sizeof(sockaddr);
	
	if(::getpeername(m_iSocket, &SocketAddress, &Length) == 0)
	{
		return SocketInformation.sin_port;
	}
	else
	{
		vGetError();
		std::cout << "Error with getpeername() " << sErrorCodeToString(m_iError) << std::endl;
	}
	
	return 0;
}
