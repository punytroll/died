#include "User.h"

DiED::User::User(void) :
	m_iLine(0),
	m_iCharacter(0)
{
}

void DiED::User::vModifyCaretPosition(int iDeltaLine, int iDeltaCharacter)
{
	m_iLine += iDeltaLine;
	if(iDeltaLine == 0)
	{
		m_iCharacter += iDeltaCharacter;
	}
	else
	{
		m_iCharacter = iDeltaCharacter;
	}
}

int DiED::User::iGetLine(void) const
{
	return m_iLine;
}

int DiED::User::iGetCharacter(void) const
{
	return m_iCharacter;
}
