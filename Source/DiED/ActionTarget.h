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

#ifndef DIED_ACTIONTARGET_H
#define DIED_ACTIONTARGET_H

namespace DiED
{
	class ActionTarget
	{
	public:
		virtual void vHandleInsert(const Glib::ustring & sString) = 0;
		virtual void vHandleDelete(int iLineRelative, int iCharacterRelative, int iLineAbsolute, int iCharacterAbsolute) = 0;
		virtual void vHandlePosition(int iLineRelative, int iCharacterRelative, int iLineAbsolute, int iCharacterAbsolute) = 0;
		virtual void vHandleLogOutNotification(void) = 0;
	};
}

#endif
