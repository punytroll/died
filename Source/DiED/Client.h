#ifndef DIED_CLIENT_H
#define DIED_CLIENT_H

#include <Network/MessageStream.h>

#include "InternalEnvironment.h"

namespace DiED
{
	class Client : public DiED::User
	{
	public:
		// constructor and destructor
		Client(DiED::InternalEnvironment & InternalEnvironment);
		virtual ~Client(void);
		
		// socket
		void vSetMessageStream(boost::shared_ptr< Network::MessageStream > MessageStream);
		
		void vInsertText(const Glib::ustring & sString);
		void vConnectionRequest(const DiED::clientid_t & ClientID, const Network::port_t & Port);
		void vConnectionAccept(const DiED::clientid_t & LocalClientID, const DiED::clientid_t & RemoteClientID);
		
		// messages
		DiED::Client & operator<<(const Network::BasicMessage & Message);
		
		u_int32_t m_u32KnownClientsMessageID;
		
		//signals
		sigc::signal< void > MessageStreamSet;
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
		DiED::InternalEnvironment & m_InternalEnvironment;
	protected:
		boost::shared_ptr< Network::MessageStream > m_MessageStream;
	private:
		Network::port_t m_Port;
	};
}

#endif
