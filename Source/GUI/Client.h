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
	private:
		bool m_bHoldMessagesBack;
		Glib::RefPtr< GUI::MessageListStore > m_MessageListStore;
		Gtk::Widget * m_pWidget;
	};
}

#endif
