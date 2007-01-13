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

#ifndef DIED_EXTERNALENVIRONMENT_H
#define DIED_EXTERNALENVIRONMENT_H

#include "Client.h"

namespace DiED
{
	class ExternalEnvironment
	{
	public:
		virtual void vNewClient(DiED::Client & DiEDClient) = 0;
		virtual void vInsert(const Glib::ustring & sString, int iLine, int iCharacter) = 0;
		virtual void vDelete(int iFromLine, int iFromCharacter, int iToLine, int iToCharacter) = 0;
		virtual int iGetNumberOfLines(void) const = 0;
		virtual int iGetLengthOfLine(int iLine) const = 0;
		virtual Glib::ustring sGetDocument(void) const = 0;
		virtual void vSetDocument(const Glib::ustring & sDocument) = 0;
		virtual int iGetLine(void) const = 0;
		virtual int iGetCharacter(void) const = 0;
	};
}

#endif
