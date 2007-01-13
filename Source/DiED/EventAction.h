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

#ifndef DIED_EVENTACTION_H
#define DIED_EVENTACTION_H

#include <glibmm/ustring.h>

#include "ActionTarget.h"
#include "BasicMessage.h"

namespace DiED
{
	class EventAction
	{
	public:
		virtual ~EventAction(void);
		virtual void vExecute(DiED::ActionTarget & ActionTarget) = 0;
		virtual boost::shared_ptr< DiED::BasicMessage > GetMessage(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID) = 0;
	};
	
	class InsertAction : public DiED::EventAction
	{
	public:
		InsertAction(const Glib::ustring & sText);
		virtual ~InsertAction(void);
		virtual void vExecute(DiED::ActionTarget & ActionTarget);
		virtual boost::shared_ptr< DiED::BasicMessage > GetMessage(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID);
	private:
		Glib::ustring m_sText;
	};
	
	class DeleteAction : public DiED::EventAction
	{
	public:
		DeleteAction(int iLineRelative, int iCharacterRelative, int iLineAbsolute, int iCharacterAbsolute);
		virtual ~DeleteAction(void);
		virtual void vExecute(DiED::ActionTarget & ActionTarget);
		virtual boost::shared_ptr< DiED::BasicMessage > GetMessage(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID);
	private:
		int m_iLineRelative;
		int m_iCharacterRelative;
		int m_iLineAbsolute;
		int m_iCharacterAbsolute;
	};
	
	class PositionAction : public DiED::EventAction
	{
	public:
		PositionAction(int iLineRelative, int iCharacterRelative, int iLineAbsolute, int iCharacterAbsolute);
		virtual ~PositionAction(void);
		virtual void vExecute(DiED::ActionTarget & ActionTarget);
		virtual boost::shared_ptr< DiED::BasicMessage > GetMessage(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID);
	private:
		int m_iLineRelative;
		int m_iCharacterRelative;
		int m_iLineAbsolute;
		int m_iCharacterAbsolute;
	};
	
	class LogOutNotificationAction : public DiED::EventAction
	{
	public:
		LogOutNotificationAction(void);
		virtual ~LogOutNotificationAction(void);
		virtual void vExecute(DiED::ActionTarget & ActionTarget);
		virtual boost::shared_ptr< DiED::BasicMessage > GetMessage(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID);
	};
}

#endif
