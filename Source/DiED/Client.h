#ifndef DIED_CLIENT_H
#define DIED_CLIENT_H

#include <Network/MessageStream.h>

#include "InternalEnvironment.h"

namespace DiED
{
	typedef u_int32_t clientid_t;
	
	class Client : public Network::MessageStream, public DiED::User
	{
	public:
		Client(boost::shared_ptr< Network::MessageFactory > MessageFactory, DiED::InternalEnvironment & InternalEnvironment);
		Client(int iSocket, boost::shared_ptr< Network::MessageFactory > MessageFactory, DiED::InternalEnvironment & InternalEnvironment);
		virtual ~Client(void);
		void vInsertText(const Glib::ustring & sString);
	protected:
		virtual void vMessageReady(void);
		virtual void vExecuteTopMessage(void);
	private:
		DiED::InternalEnvironment & m_InternalEnvironment;
	};
}

#endif
