#ifndef DIED_USER_H
#define DIED_USER_H

namespace DiED
{
	class User
	{
	public:
		User(void);
		void vModifyCaretPosition(int iDeltaLine, int iDeltaCharacter);
		int iGetLine(void) const;
		int iGetCharacter(void) const;
	private:
		int m_iLine;
		int m_iCharacter;
	};
}

#endif
