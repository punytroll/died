#include "MessageListStore.h"

GUI::MessageListStore::MessageListColumns * GUI::MessageListStore::m_pColumns = 0;

GUI::MessageListStore::MessageListStore(void) :
	Gtk::ListStore(*m_pColumns),
	Columns(*m_pColumns)
{
}

Glib::RefPtr< GUI::MessageListStore > GUI::MessageListStore::create(void)
{
	if(GUI::MessageListStore::m_pColumns == 0)
	{
		m_pColumns = new(GUI::MessageListStore::MessageListColumns);
	}
	
	return Glib::RefPtr< GUI::MessageListStore >(new GUI::MessageListStore());
}
