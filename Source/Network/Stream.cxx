#include "Stream.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <sys/socket.h>

#include <iostream>

#include "../Common.h"

const size_t g_stInitialBufferSize = 1024;

Network::Stream::Stream(void) :
	m_pu8Buffer(new u_int8_t[2048]),
	m_IBuffer(g_stInitialBufferSize),
	m_OBuffer(g_stInitialBufferSize),
	m_bConnectingInProgress(false)
{
	m_bOnDisconnected = true;
}

Network::Stream::Stream(int iSocket) :
	Socket(iSocket),
	m_pu8Buffer(new u_int8_t[2048]),
	m_IBuffer(g_stInitialBufferSize),
	m_OBuffer(g_stInitialBufferSize),
	m_bConnectingInProgress(false)
{
	m_bOnDisconnected = true;
}

void Network::Stream::vOpen(const Network::address_t & ConnectAddress, const Network::port_t & ConnectPort)
{
	if(bIsOpen() == true)
	{
		return;
	}
	m_bOnDisconnected = false;
	// -1 instead of g_iInvalidSocket because the two things are not connected to each other
	if((m_iSocket = ::socket(PF_INET, SOCK_STREAM, 0)) == -1)
	{
		m_iSocket = g_iInvalidSocket;
		vGetError();
		
		return;
	}
	
	sockaddr SocketAddress;
	sockaddr_in & SocketInformation = reinterpret_cast< sockaddr_in & >(SocketAddress);
	hostent * pHostEntity = ::gethostbyname(ConnectAddress.c_str());
	
	SocketInformation.sin_port = htons(ConnectPort);
	if(pHostEntity != 0)
	{
		SocketInformation.sin_family = pHostEntity->h_addrtype;
		::memcpy(&(SocketInformation.sin_addr), pHostEntity->h_addr_list[0], pHostEntity->h_length);
	}
	else
	{
		if(::inet_aton(ConnectAddress.c_str(), &(SocketInformation.sin_addr)) == 0)
		{
			vClose();
			vGetError();
			
			return;
		}
    }
	LOG(Info, "Network/Stream", "Beginning ::connect. This may block for some time.");
	if(::connect(m_iSocket, &SocketAddress, sizeof(sockaddr_in)) == -1)
	{
		vGetError();
		if(m_iError != EINPROGRESS)
		{
			vClose();
			LOG(Debug, "Network/Socket", "Ended ::connect.");
			
			return;
		}
		else
		{
			m_bConnectingInProgress = true;
			vRequestOnOut();
		}
	}
	LOG(Info, "Network/Socket", "Ended ::connect.");
	if(::fcntl(m_iSocket, F_SETFL, O_NONBLOCK) == -1)
	{
		vClose();
		vGetError();
		
		return;
	}
	OnConnected();
	m_bOnDisconnected = true;
	vMonitor();
}

Network::Stream & Network::Stream::operator>>(Network::BasicValue & Value)
{
	m_Values.push_back(boost::ref(Value));
	
	return *this;
}

Network::Stream & Network::Stream::operator<<(const Network::BasicValue & Value)
{
	Value.vWriteTo(m_OBuffer.GetWriter());
	
	return *this;
}

void Network::Stream::vOnIn(void)
{
//~ 	std::cout << m_Values.size() << " Values in queue for reading." << std::endl;
	
	ssize_t stSize = recv(m_iSocket, m_pu8Buffer, 2048, 0);
	
	if(stSize == -1)
	{
		vGetError();
		LOG(Error, "Network/Stream", "'recv' failed with " << sErrorCodeToString(m_iError));
		
		return;
	}
	if(stSize == 0)
	{
		LOG(Info, "Network/Stream", "Remote disconnected. Address = " << GetAddress() << ':' << GetPort());
		vClose();
		
		return;
	}
//~ 	std::cout << "Read " << stSize << " bytes from the socket." << std::endl;
	
//~ 	std::cout << std::hex;
//~ 	for(ssize_t stI = 0; stI < stSize; ++stI)
//~ 	{
//~ 		std::cout << static_cast< u_int32_t >(m_pu8Buffer[stI]) << ' ';
//~ 	}
//~ 	std::cout << std::dec << std::endl;
	
//~ 	std::cout << "Buffer size is " << m_IBuffer.stGetSize() << "." << std::endl;
	m_IBuffer.vWrite(m_pu8Buffer, stSize);
//~ 	std::cout << "Buffer size is " << m_IBuffer.stGetSize() << "." << std::endl;
	
	
	std::deque< boost::reference_wrapper< Network::BasicValue > >::iterator iValue(m_Values.begin());
	
	while(iValue != m_Values.end())
	{
		Network::BasicValue & Value(*iValue);
		
		vRead(Value);
		if(Value.bIsReady() == false)
		{
			break;
		}
		m_Values.erase(m_Values.begin());
		iValue = m_Values.begin();
	}
}

void Network::Stream::vOnOut(void)
{
	if(m_bConnectingInProgress == true)
	{
		int iError = 0;
		socklen_t slSize = sizeof(int);
		
		if(::getsockopt(m_iSocket, SOL_SOCKET, SO_ERROR, &iError, &slSize) != 0)
		{
			vClose();
			vGetError();
			LOG(Error, "Network/Stream", "VERY BAD: " << __FILE__ << ':' << __LINE__ << "  " << sErrorCodeToString(m_iError));
			
			throw;
		}
		if(iError != 0)
		{
			vClose();
			LOG(Error, "Network/Stream", "VERY BAD: " << __FILE__ << ':' << __LINE__ << "  " << sErrorCodeToString(iError));
			
			throw;
		}
		m_bConnectingInProgress = false;
	}
	
	u_int8_t * pu8Temporary = new u_int8_t[m_OBuffer.stGetSize() + 1];
	ssize_t stSize = m_OBuffer.stRead(pu8Temporary, Network::BasicBuffer::npos);
	
//~ 	std::cout << "[Network/Stream]: " << m_OBuffer.stGetSize() << " bytes to send." << " (" << stSize << " bytes from buffer)" << std::endl << std::hex;
//~ 	for(ssize_t stI = 0; stI < stSize; ++stI)
//~ 	{
//~ 		std::cout << static_cast< u_int32_t >(pu8Temporary[stI]) << ' ';
//~ 	}
//~ 	std::cout << std::endl;
	
	ssize_t stSentSize = send(m_iSocket, pu8Temporary, stSize, 0);
	
	if(stSentSize == -1)
	{
		vGetError();
		if(m_iError == EBADF)
		{
			vClose();
		}
		LOG(Error, "Network/Stream", __FILE__ << ':' << __LINE__ << ": " << sErrorCodeToString(m_iError));
		
		return;
	}
	if(stSize != stSentSize)
	{
		LOG(Error, "Network/Stream", "VERY BAD: " << __FILE__ << ':' << __LINE__ << ": Sent only " << stSentSize << " from " << stSize << " bytes.");
		LOG(TODO, "Network/Stream", __FILE__ << ':' << __LINE__ << ": Write overhang back.");
		
		throw;
	}
	else
	{
		vIgnoreOnOut();
	}
//~ 	std::cout << "                  " << std::dec << stSize << " bytes sent." << std::endl;
	delete[] pu8Temporary;
	BytesSent(stSentSize);
}

void Network::Stream::vRead(Network::BasicValue & Value)
{
	Value.vReadFrom(m_IBuffer.GetReader());
}
