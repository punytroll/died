#ifndef DIED_EXTERNALENVIRONMENT_H
#define DIED_EXTERNALENVIRONMENT_H

#include "Client.h"

namespace DiED
{
	class ExternalEnvironment
	{
	public:
		virtual void vNewClient(DiED::Client & DiEDClient) = 0;
		virtual void vInsert(const Glib::ustring & sString, int iLine, int iCharacter) = 0;
		virtual void vDelete(int iFromLine, int iFromCharacter, int iToLine, int iToCharacter) = 0;
		virtual int iGetNumberOfLines(void) = 0;
		virtual int iGetLengthOfLine(int iLine) = 0;
	};
}

#endif
