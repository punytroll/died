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

#include "EventAction.h"

#include "Messages.h"

DiED::EventAction::~EventAction(void)
{
}

DiED::InsertAction::InsertAction(const Glib::ustring & sText) :
	m_sText(sText)
{
}

DiED::InsertAction::~InsertAction(void)
{
}

void DiED::InsertAction::vExecute(DiED::ActionTarget & ActionTarget)
{
	ActionTarget.vHandleInsert(m_sText);
}

boost::shared_ptr< DiED::BasicMessage > DiED::InsertAction::GetMessage(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID)
{
	return boost::shared_ptr< DiED::BasicMessage >(new DiED::InsertEvent(CreatorID, EventID, LostClientID, m_sText));
}

DiED::DeleteAction::DeleteAction(int iLineRelative, int iCharacterRelative, int iLineAbsolute, int iCharacterAbsolute) :
	m_iLineRelative(iLineRelative),
	m_iCharacterRelative(iCharacterRelative),
	m_iLineAbsolute(iLineAbsolute),
	m_iCharacterAbsolute(iCharacterAbsolute)
{
}

DiED::DeleteAction::~DeleteAction(void)
{
}

void DiED::DeleteAction::vExecute(DiED::ActionTarget & ActionTarget)
{
	ActionTarget.vHandleDelete(m_iLineRelative, m_iCharacterRelative, m_iLineAbsolute, m_iCharacterAbsolute);
}

boost::shared_ptr< DiED::BasicMessage > DiED::DeleteAction::GetMessage(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID)
{
	return boost::shared_ptr< DiED::BasicMessage >(new DiED::DeleteEvent(CreatorID, EventID, LostClientID, m_iLineRelative, m_iCharacterRelative, m_iLineAbsolute, m_iCharacterAbsolute));
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///                                     PositionAction                                          ///
///////////////////////////////////////////////////////////////////////////////////////////////////

DiED::PositionAction::PositionAction(int iLineRelative, int iCharacterRelative, int iLineAbsolute, int iCharacterAbsolute) :
	m_iLineRelative(iLineRelative),
	m_iCharacterRelative(iCharacterRelative),
	m_iLineAbsolute(iLineAbsolute),
	m_iCharacterAbsolute(iCharacterAbsolute)
{
}

DiED::PositionAction::~PositionAction(void)
{
}

void DiED::PositionAction::vExecute(DiED::ActionTarget & ActionTarget)
{
	ActionTarget.vHandlePosition(m_iLineRelative, m_iCharacterRelative, m_iLineAbsolute, m_iCharacterAbsolute);
}

boost::shared_ptr< DiED::BasicMessage > DiED::PositionAction::GetMessage(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID)
{
	return boost::shared_ptr< DiED::BasicMessage >(new DiED::PositionEvent(CreatorID, EventID, LostClientID, m_iLineRelative, m_iCharacterRelative, m_iLineAbsolute, m_iCharacterAbsolute));
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///                                 LogOutNotificationAction                                    ///
///////////////////////////////////////////////////////////////////////////////////////////////////

DiED::LogOutNotificationAction::LogOutNotificationAction(void)
{
}

DiED::LogOutNotificationAction::~LogOutNotificationAction(void)
{
}

void DiED::LogOutNotificationAction::vExecute(DiED::ActionTarget & ActionTarget)
{
	ActionTarget.vHandleLogOutNotification();
}

boost::shared_ptr< DiED::BasicMessage > DiED::LogOutNotificationAction::GetMessage(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID)
{
	return boost::shared_ptr< DiED::BasicMessage >(new DiED::LogOutNotificationEvent(CreatorID, EventID, LostClientID));
}
