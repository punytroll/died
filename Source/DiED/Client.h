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
		void vConnectionRequest(const DiED::clientid_t & ClientID, const Network::port_t & Port);
		void vConnectionAccept(const DiED::clientid_t & LocalClientID, const DiED::clientid_t & RemoteClientID);
		
		u_int32_t m_u32KnownClientsMessageID;
	protected:
		// callbacks from template pattern
//~ 		virtual void vOnConnect(void);
//~ 		virtual void vOnDisconnect(void);
		virtual void vOnMessageReady(void);
		virtual void vOnMessageBegin(void);
		virtual void vOnMessageExecuted(void);
		// operations
		virtual void vExecuteTopMessage(void);
	private:
		Network::port_t m_Port;
		DiED::InternalEnvironment & m_InternalEnvironment;
	};
}

#endif
