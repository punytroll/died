#ifndef DIED_EXTERNALENVIRONMENT_H
#define DIED_EXTERNALENVIRONMENT_H

#include "Client.h"

namespace DiED
{
	class ExternalEnvironment
	{
	public:
		virtual void vClientConnected(DiED::Client & DiEDClient) = 0;
		virtual void vInsertText(const Glib::ustring & sString, int iLine, int iCharacter) = 0;
	};
}

#endif
