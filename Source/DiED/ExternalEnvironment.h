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
		virtual int iGetNumberOfLines(void) const = 0;
		virtual int iGetLengthOfLine(int iLine) const = 0;
		virtual Glib::ustring sGetDocument(void) const = 0;
		virtual void vSetDocument(const Glib::ustring & sDocument) = 0;
		virtual int iGetLine(void) const = 0;
		virtual int iGetCharacter(void) const = 0;
	};
}

#endif
