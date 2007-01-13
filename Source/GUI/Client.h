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

#ifndef GUI_CLIENT_H
#define GUI_CLIENT_H

#include <gtkmm/widget.h>

#include <DiED/Client.h>

#include "MessageListStore.h"

namespace GUI
{
	class Client : public DiED::Client
	{
	public:
		Client(DiED::InternalEnvironment & InternalEnvironment);
		Client(int iSocket, DiED::InternalEnvironment & InternalEnvironment);
		virtual ~Client(void);
		
		Glib::RefPtr< GUI::MessageListStore > GUI::Client::GetMessageListStore(void);
		void vSetWidget(Gtk::Widget * pWidget);
		Gtk::Widget * pGetWidget(void);
		
		// messages
		bool bIsHoldingMessagesBack(void) const;
		void vHoldMessagesBack(bool bHoldMessagesBack);
	protected:
		virtual void vOnMessageReady(void);
		virtual void vOnMessageBegin(void);
		virtual void vOnMessageExecuted(void);
		void vOnMessageQueued(boost::weak_ptr< DiED::BasicMessage > Message);
		void vOnMessageSent(boost::weak_ptr< DiED::BasicMessage > Message);
	private:
		bool m_bHoldMessagesBack;
		Glib::RefPtr< GUI::MessageListStore > m_MessageListStore;
		Gtk::Widget * m_pWidget;
	};
}

#endif
