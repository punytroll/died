#ifndef CLIENT_H
#define CLIENT_H

#include "Socket.h"

namespace Network
{
	class Stream : public Network::Socket
	{
	public:
		Stream(void);
		Stream(int iSocket);
		void vOpen(const std::string & sConnectAddress, u_int16_t u16ConnectPort);
	};
}

#endif
