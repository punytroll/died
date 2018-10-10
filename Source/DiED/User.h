/* DiED - A distributed Editor.
 * Copyright (C) 2005 Hagen Möbius & Aram Altschudjian
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef DIED_USER_H
#define DIED_USER_H

#include <sys/types.h>

#include <map>
#include <set>

#include <sigc++/sigc++.h>

#include "Common.h"

namespace DiED
{
	struct ClientInfo
	{
		DiED::clientid_t ClientID;
		DiED::clientstatus_t Status;
		DiED::messageid_t EventCounter;
		int iLine;
		int iCharacter;
	};
	
	class User
	{
	public:
		User(void);
		virtual ~User(void);
		
		// ClientID stuff
		void vSetID(const DiED::clientid_t & ID);
		DiED::clientid_t GetID(void) const;
		
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
		void vSetLine(int iLine);
		void vSetCharacter(int iCharacter);
		sigc::signal< void > CaretPositionChanged;
	private:
		int m_iLine;
		int m_iCharacter;
		DiED::clientid_t m_ID;
		std::map< DiED::clientid_t, DiED::clientstatus_t > m_Status;
	};
}

#endif
