#ifndef DIED_USER_H
#define DIED_USER_H

#include <sys/types.h>

#include <sigc++/sigc++.h>

namespace DiED
{
	typedef u_int32_t clientid_t;
	
	class User
	{
	public:
		User(void);
		virtual ~User(void);
		void vModifyCaretPosition(int iDeltaLine, int iDeltaCharacter);
		int iGetLine(void) const;
		int iGetCharacter(void) const;
		
		// ClientID stuff
		void vSetClientID(DiED::clientid_t ClientID);
		DiED::clientid_t GetClientID(void);
		sigc::signal< void > ClientIDChanged;
	private:
		int m_iLine;
		int m_iCharacter;
		DiED::clientid_t m_ClientID;
	};
}

#endif
