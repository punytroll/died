#include "Client.h"

#include <sstream>

#include <DiED/BasicMessage.h>

GUI::Client::Client(DiED::InternalEnvironment & InternalEnvironment) :
	DiED::Client(InternalEnvironment),
	m_bHoldMessagesBack(false),
	m_MessageListStore(GUI::MessageListStore::create())
{
	std::cout << "[GUI/Client]: Created new Client." << std::endl;
}

GUI::Client::Client(int iSocket, DiED::InternalEnvironment & InternalEnvironment) :
	DiED::Client(iSocket, InternalEnvironment),
	m_bHoldMessagesBack(false),
	m_MessageListStore(GUI::MessageListStore::create())
{
	std::cout << "[GUI/Client]: Created new Client from socket." << std::endl;
}

GUI::Client::~Client(void)
{
	std::cout << "[GUI/Client]: Deleted Client." << std::endl;
}

Glib::RefPtr< GUI::MessageListStore > GUI::Client::GetMessageListStore(void)
{
	return m_MessageListStore;
}

void GUI::Client::vOnMessageReady(void)
{
	std::cout << "Message ready:\n\t" << boost::dynamic_pointer_cast< DiED::BasicMessage >(back())->sGetString() << std::endl;
	
	Gtk::TreeRow Row(*(m_MessageListStore->children().begin()));
	
	Row[m_MessageListStore->Columns.Name] = boost::dynamic_pointer_cast< DiED::BasicMessage >(back())->sGetString();
	if(m_bHoldMessagesBack == false)
	{
		DiED::Client::vOnMessageReady();
	}
}

void GUI::Client::vOnMessageBegin(void)
{
	Gtk::TreeRow Row(*m_MessageListStore->prepend());
	std::stringstream ssName;
	
	ssName << "Type = " << back()->GetType();
	Row[m_MessageListStore->Columns.Name] = ssName.str();
	Row[m_MessageListStore->Columns.ClientID] = GetClientID();
	std::cout << "Message begin: " << ssName.str() << std::endl;
	DiED::Client::vOnMessageBegin();
}

void GUI::Client::vOnMessageExecuted(void)
{
	std::cout << "Message executed:\n\t" << boost::dynamic_pointer_cast< DiED::BasicMessage >(back())->sGetString() << std::endl;
}
