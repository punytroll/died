/* DiED - A distributed Editor.
 * Copyright (C) 2005 Hagen M�bius & Aram Altschudjian
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

#ifndef GUI_MESSAGELISTSTORE_H
#define GUI_MESSAGELISTSTORE_H

#include <gtkmm/liststore.h>

#include <DiED/User.h>

namespace GUI
{
	class MessageListStore : public Gtk::ListStore
	{
	private:
		class MessageListColumns : public Gtk::TreeModelColumnRecord
		{
		public:
			Gtk::TreeModelColumn< DiED::clientid_t > ClientID;
			Gtk::TreeModelColumn< Glib::ustring > Name;
			Gtk::TreeModelColumn< Glib::ustring > Status;
			Gtk::TreeModelColumn< Gdk::Color > Color;
		
			MessageListColumns(void)
			{
				add(ClientID);
				add(Name);
				add(Status);
				add(Color);
			}
		};
	public:
		static Glib::RefPtr< GUI::MessageListStore > create(void);
		MessageListColumns & Columns;
	private:
		MessageListStore(void);
		static MessageListColumns * m_pColumns;
	};
}

#endif
