#ifndef DIED_INTERNALENVIRONMENT_H
#define DIED_INTERNALENVIRONMENT_H

#include "Client.h"

namespace DiED
{
	class Client;
	
	class InternalEnvironment
	{
	public:
		virtual void vInsertText(DiED::Client & Client, const Glib::ustring & sString) = 0;
	};
}

#endif
