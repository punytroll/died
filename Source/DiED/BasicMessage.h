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

#ifndef DIED_BASICMASSAGE_H
#define DIED_BASICMASSAGE_H

#include <Network/BasicMessage.h>

#include "Common.h"
#include "MessageTarget.h"

namespace DiED
{
	class BasicMessage : public sigc::trackable, public Network::BasicMessage
	{
	public:
		BasicMessage(const Network::BasicMessage::type_t & Type);
		virtual ~BasicMessage(void);
		virtual boost::shared_ptr< DiED::ConfirmationParameters > GetConfirmationParameters(void);
		virtual bool bIsConfirmedBy(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters);
		virtual void vExecute(DiED::MessageTarget & MessageTarget) = 0;
		virtual Glib::ustring sGetString(void) = 0;
		virtual bool bIsEvent(void);
		virtual bool bRequiresConfirmation(void);
		virtual boost::shared_ptr< DiED::BasicMessage > Clone(void);
	
		void vInitiateTimeout(DiED::MessageTarget * pMessageTarget, unsigned int uiMilliSeconds);
		void vDisconnectTimeout(void);
	private:
		sigc::connection m_TimeoutConnection;
		virtual bool bOnTimeout(DiED::MessageTarget * pMessageTarget);
	};
}

#endif
