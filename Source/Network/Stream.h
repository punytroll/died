#ifndef CLIENT_H
#define CLIENT_H

#include <deque>

#include <boost/ref.hpp>
#include <boost/shared_ptr.hpp>

#include "BasicValue.h"
#include "CircularBuffer.h"
#include "Socket.h"

namespace Network
{
	class Stream : public Network::Socket
	{
	public:
		// constructors and destructors
		Stream(void);
		Stream(int iSocket);
		
		// connection
		void vOpen(const Network::address_t & ConnectAddress, const Network::port_t & ConnectPort);
		
		// input/output streaming of Values
		Stream & operator>>(Network::BasicValue & Value);
		Stream & operator<<(const Network::BasicValue & Value);
		
		// signals
		sigc::signal< void, size_t > BytesSent;
	protected:
		void vRead(Network::BasicValue & Value);
		virtual void vOnIn(void);
		virtual void vOnOut(void);
	private:
		u_int8_t * m_pu8Buffer;
		Network::CircularBuffer m_IBuffer;
		Network::CircularBuffer m_OBuffer;
		std::deque< boost::reference_wrapper< Network::BasicValue > > m_Values;
	};
}

#endif
