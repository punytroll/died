#include "Socket.h"

#include <iostream>

const int Network::g_iInvalidSocket = -1;

Network::Socket::Socket(void) :
	m_iSocket(g_iInvalidSocket),
	m_iError(0)
{
}

Network::Socket::Socket(int iSocket) :
	m_iSocket(iSocket),
	m_iError(0)
{
	vMonitor();
}

Network::Socket::~Socket(void)
{
	if(bIsOpen() == true)
	{
		close(m_iSocket);
	}
}

bool Network::Socket::bIsOpen(void) const
{
	return m_iSocket != g_iInvalidSocket;
}

int Network::Socket::iGetError(void) const
{
	return m_iError;
}

void Network::Socket::vMonitor(void)
{
	Glib::RefPtr< Glib::IOSource > IOSource = Glib::IOSource::create(m_iSocket, Glib::IO_IN | Glib::IO_PRI | Glib::IO_ERR | Glib::IO_HUP | Glib::IO_NVAL);
		
	IOSource->connect(sigc::mem_fun(*this, &Network::Socket::bNotify));
	IOSource->attach(Glib::MainContext::get_default());
}

bool Network::Socket::bNotify(const Glib::IOCondition & Condition)
{
	if((Condition & Glib::IO_IN) != 0)
	{
		std::cout << "IO_IN" << std::endl;
		vOnIn();
	}
	if((Condition & Glib::IO_OUT) != 0)
	{
		std::cout << "IO_OUT" << std::endl;
	}
	if((Condition & Glib::IO_PRI) != 0)
	{
		std::cout << "IO_PRI" << std::endl;
	}
	if((Condition & Glib::IO_ERR) != 0)
	{
		std::cout << "IO_ERR" << std::endl;
	}
	if((Condition & Glib::IO_HUP) != 0)
	{
		std::cout << "IO_HUP" << std::endl;
	}
	if((Condition & Glib::IO_NVAL) != 0)
	{
		std::cout << "IO_NVAL" << std::endl;
		
		return false;
	}
	
	return true;
}

void Network::Socket::vOnIn(void)
{
}

void Network::Socket::vGetError(void)
{
	m_iError = errno;
}
