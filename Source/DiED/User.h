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
			Delete
		};
		
		User(void);
		virtual ~User(void);
		void vModifyCaretPosition(int iDeltaLine, int iDeltaCharacter);
		int iGetLine(void) const;
		int iGetCharacter(void) const;
		
		// ClientID stuff
		void vSetClientID(DiED::clientid_t ClientID);
		DiED::clientid_t GetClientID(void);
		sigc::signal< void > ClientIDChanged;
		
		// Status stuff
		void vSetStatus(const DiED::clientid_t & ClientID, DiED::User::Status Status);
		Status GetStatus(const DiED::clientid_t & ClientID);
		std::vector< DiED::clientid_t > GetConnectedClientIDs(void);
		std::vector< DiED::clientid_t > GetDisconnectedClientIDs(void);
	private:
		int m_iLine;
		int m_iCharacter;
		DiED::clientid_t m_ClientID;
		std::map< DiED::clientid_t, DiED::User::Status > m_Status;
	};
}

#endif
