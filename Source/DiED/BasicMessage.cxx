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

#include "BasicMessage.h"

#include <iostream>

DiED::BasicMessage::BasicMessage(const Network::BasicMessage::type_t & Type) :
	Network::BasicMessage(Type)
{
}

DiED::BasicMessage::~BasicMessage(void)
{
	vDisconnectTimeout();
}

boost::shared_ptr< DiED::ConfirmationParameters > DiED::BasicMessage::GetConfirmationParameters(void)
{
	return boost::shared_ptr< DiED::ConfirmationParameters >();
}

bool DiED::BasicMessage::bIsConfirmedBy(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters)
{
	return false;
}

bool DiED::BasicMessage::bIsEvent(void)
{
	return false;
}

bool DiED::BasicMessage::bRequiresConfirmation(void)
{
	return false;
}

void DiED::BasicMessage::vInitiateTimeout(DiED::MessageTarget * pMessageTarget, unsigned int uiMilliSeconds)
{
	m_TimeoutConnection = Glib::signal_timeout().connect(sigc::bind(sigc::mem_fun(*this, &DiED::BasicMessage::bOnTimeout), pMessageTarget), uiMilliSeconds);
}

void DiED::BasicMessage::vDisconnectTimeout(void)
{
	if(m_TimeoutConnection)
	{
		m_TimeoutConnection.disconnect();
	}
}

bool DiED::BasicMessage::bOnTimeout(DiED::MessageTarget * pMessageTarget)
{
	return false;
}

boost::shared_ptr< DiED::BasicMessage > DiED::BasicMessage::Clone(void)
{
	LOG(Error, "DiED/BasicMessage", "VERY BAD: " << __FILE__ << ':' << __LINE__ << " no Clone function for MessageType " << GetType());
	
	return boost::shared_ptr< DiED::BasicMessage >();
}
