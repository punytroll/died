#ifndef DIED_CLIENT_H
#define DIED_CLIENT_H

#include <Network/MessageStream.h>

#include "InternalEnvironment.h"

namespace DiED
{
	class Client : public Network::MessageStream, public DiED::User
	{
	public:
		Client(DiED::InternalEnvironment & InternalEnvironment);
		Client(int iSocket, DiED::InternalEnvironment & InternalEnvironment);
		virtual ~Client(void);
		void vInsertText(const Glib::ustring & sString);
	protected:
		// callbacks from template pattern
		virtual void vOnMessageReady(void);
		virtual void vOnMessageBegin(void);
		virtual void vOnMessageExecuted(void);
		// operations
		virtual void vExecuteTopMessage(void);
	private:
		DiED::InternalEnvironment & m_InternalEnvironment;
	};
}

#endif
