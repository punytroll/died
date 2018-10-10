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

#include "Client.h"

#include <sstream>

#include <boost/weak_ptr.hpp>

#include <DiED/BasicMessage.h>

GdkColor OutMessage = { 0x0000, 0xA0A0, 0xA0A0, 0xA0A0 };

GUI::Client::Client(DiED::InternalEnvironment & InternalEnvironment) :
	DiED::Client(InternalEnvironment),
	m_bHoldMessagesBack(false),
	m_MessageListStore(GUI::MessageListStore::create())
{
	MessageQueued.connect(sigc::mem_fun(*this, &GUI::Client::vOnMessageQueued));
	MessageSent.connect(sigc::mem_fun(*this, &GUI::Client::vOnMessageSent));
//~ 	std::cout << "[GUI/Client]: Created new Client." << std::endl;
}

GUI::Client::~Client(void)
{
//~ 	std::cout << "[GUI/Client]: Deleted Client." << std::endl;
}

Glib::RefPtr< GUI::MessageListStore > GUI::Client::GetMessageListStore(void)
{
	return m_MessageListStore;
}

void GUI::Client::vOnMessageReady(void)
{
//~ 	std::cout << "[GUI/Client]: Message ready for Client " << GetClientID() << " with MessageStream " << m_MessageStream.get() << std::endl;
	Gtk::TreeIter Iterator(m_MessageListStore->children().begin());
	Gtk::TreeRow Row;
	
	while(Iterator != m_MessageListStore->children().end())
	{
		if((*Iterator)[m_MessageListStore->Columns.Status] == "Downloading")
		{
			Row = *Iterator;
		} else if((*Iterator)[m_MessageListStore->Columns.Status] == "Executed")
		{
			break;
		}
		++Iterator;
	}
	if(m_MessageStream->size() > 0)
	{
//~ 		std::cout << "[GUI/Client]: " << boost::dynamic_pointer_cast< DiED::BasicMessage >(m_MessageStream->back())->sGetString() << std::endl;
		Row[m_MessageListStore->Columns.Name] = boost::dynamic_pointer_cast< DiED::BasicMessage >(m_MessageStream->back())->sGetString();
		Row[m_MessageListStore->Columns.Status] = "Ready";
		if(m_bHoldMessagesBack == false)
		{
			DiED::Client::vOnMessageReady();
		}
	}
}

void GUI::Client::vOnMessageBegin(void)
{
	Gtk::TreeRow Row(*m_MessageListStore->prepend());
	std::stringstream ssName;
	
	ssName << "Type = " << m_MessageStream->back()->GetType();
	Row[m_MessageListStore->Columns.Name] = ssName.str();
	Row[m_MessageListStore->Columns.ClientID] = GetID();
	Row[m_MessageListStore->Columns.Status] = "Downloading";
//~ 	std::cout << "Message begin: " << ssName.str() << std::endl;
	DiED::Client::vOnMessageBegin();
}

void GUI::Client::vOnMessageExecuted(void)
{
//~ 	std::cout << "Message executed:\n\t" << boost::dynamic_pointer_cast< DiED::BasicMessage >(m_MessageStream->back())->sGetString() << std::endl;
	Gtk::TreeIter Iterator(m_MessageListStore->children().begin());
	Gtk::TreeRow Row;
	
	// go from the top of the list down
	while(Iterator != m_MessageListStore->children().end())
	{
		// if the Status of the Row is "Ready" set the Row
		if((*Iterator)[m_MessageListStore->Columns.Status] == "Ready")
		{
			Row = *Iterator;
		}
		// if we encounter the first "Executed" message, Row points to the last one with Status "Ready"
		else if((*Iterator)[m_MessageListStore->Columns.Status] == "Executed")
		{
			break;
		}
		++Iterator;
	}
	Row[m_MessageListStore->Columns.Status] = "Executed";
}

bool GUI::Client::bIsHoldingMessagesBack(void) const
{
	return m_bHoldMessagesBack;
}

void GUI::Client::vHoldMessagesBack(bool bHoldMessagesBack)
{
	m_bHoldMessagesBack = bHoldMessagesBack;
	if((m_bHoldMessagesBack == false) && (m_MessageStream.get() != 0))
	{
		while(m_MessageStream->size() > 0)
		{
			DiED::Client::vOnMessageReady();
		}
	}
}

void GUI::Client::vSetWidget(Gtk::Widget * pWidget)
{
	m_pWidget = pWidget;
}

Gtk::Widget * GUI::Client::pGetWidget(void)
{
	return m_pWidget;
}

void GUI::Client::vOnMessageQueued(boost::weak_ptr< DiED::BasicMessage > _Message)
{
	boost::shared_ptr< DiED::BasicMessage > Message(_Message.lock());
	Gtk::TreeRow Row(*m_MessageListStore->prepend());
	std::stringstream ssName;
	
	Row[m_MessageListStore->Columns.Name] = Message->sGetString();
	Row[m_MessageListStore->Columns.ClientID] = GetID();
	Row[m_MessageListStore->Columns.Status] = "Queued";
	Row[m_MessageListStore->Columns.Color] = Gdk::Color(&OutMessage);
}

void GUI::Client::vOnMessageSent(boost::weak_ptr< DiED::BasicMessage > Message)
{
//~ 	std::cout << "Message executed:\n\t" << boost::dynamic_pointer_cast< DiED::BasicMessage >(m_MessageStream->back())->sGetString() << std::endl;
	Gtk::TreeIter Iterator(m_MessageListStore->children().begin());
	Gtk::TreeRow Row;
	
	// go from the top of the list down
	while(Iterator != m_MessageListStore->children().end())
	{
		// if the Status of the Row is "Ready" set the Row
		if((*Iterator)[m_MessageListStore->Columns.Status] == "Queued")
		{
			Row = *Iterator;
		}
		// if we encounter the first "Executed" message, Row points to the last one with Status "Ready"
		else if((*Iterator)[m_MessageListStore->Columns.Status] == "Sent")
		{
			break;
		}
		++Iterator;
	}
	Row[m_MessageListStore->Columns.Status] = "Sent";
}
