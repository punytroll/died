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
		
			MessageListColumns(void)
			{
				add(ClientID);
				add(Name);
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
