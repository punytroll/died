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
		Stream(void);
		Stream(int iSocket);
		Stream & operator>>(Network::BasicValue & Value);
		Stream & operator<<(const Network::BasicValue & Value);
		void vOpen(const std::string & sConnectAddress, u_int16_t u16ConnectPort);
	protected:
		void vRead(Network::BasicValue & Value);
		virtual void vOnIn(void);
	private:
		u_int8_t * m_pu8Buffer;
		Network::CircularBuffer m_Buffer;
		std::deque< boost::reference_wrapper< Network::BasicValue > > m_Values;
	};
}

#endif
