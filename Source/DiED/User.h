#ifndef DIED_USER_H
#define DIED_USER_H

#include <sys/types.h>

#include <map>
#include <set>

#include <sigc++/sigc++.h>

#include <Common.h>

namespace DiED
{
	class User
	{
	public:
		User(void);
		virtual ~User(void);
		
		// ClientID stuff
		void vSetID(DiED::clientid_t ID);
		DiED::clientid_t GetID(void);
		
		// Status stuff
		virtual void vSetStatus(const DiED::clientid_t & ClientID, const DiED::clientstatus_t & Status);
		DiED::clientstatus_t GetStatus(const DiED::clientid_t & ClientID);
		std::set< DiED::clientid_t > GetConnectedClientIDs(void);
		std::set< DiED::clientid_t > GetDisconnectedClientIDs(void);
		sigc::signal< void, DiED::clientid_t, DiED::clientstatus_t > StatusChanged;
		
		// caret position stuff
		void vModifyCaretPosition(int iDeltaLine, int iDeltaCharacter);
		int iGetLine(void) const;
		int iGetCharacter(void) const;
		sigc::signal< void > CaretPositionChanged;
	private:
		int m_iLine;
		int m_iCharacter;
		DiED::clientid_t m_ID;
		std::map< DiED::clientid_t, DiED::clientstatus_t > m_Status;
	};
}

#endif
