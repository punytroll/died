#include "Client.h"

#include <sstream>

#include <DiED/BasicMessage.h>

GUI::Client::Client(DiED::InternalEnvironment & InternalEnvironment) :
	DiED::Client(InternalEnvironment),
	m_bHoldMessagesBack(false),
	m_MessageListStore(GUI::MessageListStore::create())
{
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
	
	while(Iterator != m_MessageListStore->children().end())
	{
		if((*Iterator)[m_MessageListStore->Columns.Status] == "Ready")
		{
			Row = *Iterator;
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
