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
	};
}

#endif
