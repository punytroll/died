#ifndef DIED_USER_H
#define DIED_USER_H

#include <sys/types.h>

#include <map>
#include <vector>

#include <sigc++/sigc++.h>

namespace DiED
{
	typedef u_int32_t clientid_t;
	
	class User
	{
	public:
		enum Status
		{
			Connected,
			Disconnected,
			Deleted
		};
		
		User(void);
		virtual ~User(void);
		
		// ClientID stuff
		void vSetID(DiED::clientid_t ID);
		DiED::clientid_t GetID(void);
		
		// Status stuff
		void vSetStatus(const DiED::clientid_t & ClientID, DiED::User::Status Status);
		Status GetStatus(const DiED::clientid_t & ClientID);
		std::vector< DiED::clientid_t > GetConnectedClientIDs(void);
		std::vector< DiED::clientid_t > GetDisconnectedClientIDs(void);
		sigc::signal< void, DiED::clientid_t, DiED::User::Status > StatusChanged;
		
		// caret position stuff
		void vModifyCaretPosition(int iDeltaLine, int iDeltaCharacter);
		int iGetLine(void) const;
		int iGetCharacter(void) const;
		sigc::signal< void > CaretPositionChanged;
	private:
		int m_iLine;
		int m_iCharacter;
		DiED::clientid_t m_ID;
		std::map< DiED::clientid_t, DiED::User::Status > m_Status;
	};
}

#endif
