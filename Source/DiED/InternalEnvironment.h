#ifndef DIED_INTERNALENVIRONMENT_H
#define DIED_INTERNALENVIRONMENT_H

namespace DiED
{
	class InternalEnvironment
	{
	public:
		virtual void vInsertText(const Glib::ustring & sString, int iLine, int iCharacter) = 0;
	};
}

#endif
