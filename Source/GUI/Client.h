#ifndef GUI_CLIENT_H
#define GUI_CLIENT_H

#include <DiED/Client.h>

namespace GUI
{
	class Client : public DiED::Client
	{
	public:
		Client(boost::shared_ptr< Network::MessageFactory > MessageFactory, DiED::InternalEnvironment & InternalEnvironment);
		Client(int iSocket, boost::shared_ptr< Network::MessageFactory > MessageFactory, DiED::InternalEnvironment & InternalEnvironment);
	protected:
		virtual void vMessageReady(void);
	private:
		bool m_bHoldMessagesBack;
	};
}

#endif
