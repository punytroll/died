#ifndef DIED_INTERNALENVIRONMENT_H
#define DIED_INTERNALENVIRONMENT_H

#include <Network/MessageFactory.h>

#include "User.h"

namespace DiED
{
	class InternalEnvironment
	{
	public:
		virtual boost::shared_ptr< Network::MessageFactory > GetMessageFactory(void) = 0;
		virtual void vInsertText(DiED::User & User, const Glib::ustring & sString) = 0;
		virtual void vConnectionRequest(DiED::User & User, const DiED::clientid_t & ClientID) = 0;
		virtual void vConnectionAccept(DiED::User & User, const DiED::clientid_t & LocalClientID, const DiED::clientid_t & RemoteClientID) = 0;
	};
}

#endif
