#include "MainWindow.h"

#include <iostream>
#include <sstream>

#include <gtkmm/box.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/stock.h>

#include "Client.h"

GdkColor Connecting = { 0x0000, 0xE0E0, 0xE0E0, 0x0000 };
GdkColor Connected = { 0x0000, 0x0000, 0x8080, 0x0000 };
GdkColor Disconnected = { 0x0000, 0x8080, 0x0000, 0x0000 };
GdkColor Deleted = { 0x0000, 0x6060, 0x6060, 0x6060 };

GUI::MainWindow::MainWindow(DiED::System & System) :
	m_System(System),
	m_TextBuffer(m_TextView.get_buffer()),
	m_KeyPressedConnection(m_TextView.signal_key_press_event().connect(sigc::mem_fun(*this, &GUI::MainWindow::bKeyPressed), false)),
	m_InsertConnection(m_TextBuffer->signal_insert().connect(sigc::mem_fun(*this, &GUI::MainWindow::vInserted)))
{
	m_System.vSetExternalEnvironment(this);
	
	Gtk::ScrolledWindow * pScrolledWindow(manage(new Gtk::ScrolledWindow()));
	
	m_TextView.show();
	pScrolledWindow->add(m_TextView);
	m_Notebook.show();
	m_Pane.show();
	m_Pane.pack1(*pScrolledWindow, true, true);
	m_Pane.pack2(m_Notebook, true, true);
	add(m_Pane);
	set_default_size(500, 300);
	show_all();
}

GUI::MainWindow::~MainWindow(void)
{
	m_StatusChangedConnection.disconnect();
}

void GUI::MainWindow::vInserted(const Gtk::TextBuffer::iterator & Iterator, const Glib::ustring & sString, int)
{
//~ 	std::cout << "Inserted text: \"" << sString << "\" [" << sString.length() << ']' << std::endl;
	// TODO: handle input at positions other than "insert" mark
	m_System.vInput(sString);
}

bool GUI::MainWindow::bKeyPressed(GdkEventKey * pEvent)
{
	return false;
}

void GUI::MainWindow::vNewClient(DiED::Client & DiEDClient)
{
	if(DiEDClient.GetID() == m_System.GetLocalClientID())
	{
		std::stringstream ssName;
		
		ssName << DiEDClient.GetID();
		set_title(std::string("DiED: ") + ssName.str());
		m_StatusChangedConnection = DiEDClient.StatusChanged.connect(sigc::bind(sigc::mem_fun(*this, &GUI::MainWindow::vClientStatusChanged), boost::ref(DiEDClient)));
		
		return;
	}
	
	GUI::Client & Client(dynamic_cast< GUI::Client & >(DiEDClient));
	Gtk::VBox * pBox(manage(new Gtk::VBox()));
	Gtk::ScrolledWindow * pScrolledWindow(manage(new Gtk::ScrolledWindow()));
	Gtk::TreeView * pClientView(manage(new Gtk::TreeView(Client.GetMessageListStore())));
	Gtk::HButtonBox * pButtonBox(manage(new Gtk::HButtonBox()));
	Gtk::Button * pHoldFlowButton(manage(new Gtk::Button("Hold/Flow")));
	Gtk::Button * pNextButton(manage(new Gtk::Button("Next")));
	std::stringstream ssName;
	
	ssName << Client.GetID();
	pClientView->append_column("Name", Client.GetMessageListStore()->Columns.Name);
	pClientView->append_column("Status", Client.GetMessageListStore()->Columns.Status);
	
	Gtk::CellRendererText * pCellRenderer = 0;
	
	pCellRenderer = dynamic_cast< Gtk::CellRendererText * >(pClientView->get_column_cell_renderer(0));
	pClientView->get_column(0)->add_attribute(pCellRenderer->property_foreground_gdk(), Client.GetMessageListStore()->Columns.Color);
	pCellRenderer = dynamic_cast< Gtk::CellRendererText * >(pClientView->get_column_cell_renderer(1));
	pClientView->get_column(1)->add_attribute(pCellRenderer->property_foreground_gdk(), Client.GetMessageListStore()->Columns.Color);
	pClientView->show();
	Client.GetMessageListStore()->signal_row_inserted().connect(sigc::bind(sigc::mem_fun(*this, &GUI::MainWindow::vRowInsertedForClient), pClientView));
	pScrolledWindow->add(*pClientView);
	pScrolledWindow->show();
	pHoldFlowButton->set_label(((Client.bIsHoldingMessagesBack() == true) ? ("Flow") : ("Hold")));
	pHoldFlowButton->signal_clicked().connect(sigc::bind(sigc::bind(sigc::mem_fun(*this, &GUI::MainWindow::vHoldFlowButtonClicked), boost::ref(Client)), pHoldFlowButton, pNextButton));
	pHoldFlowButton->show();
	pNextButton->set_sensitive(Client.bIsHoldingMessagesBack());
	pNextButton->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &GUI::MainWindow::vNextButtonClicked), boost::ref(Client)));
	pNextButton->show();
	pButtonBox->pack_start(*pHoldFlowButton);
	pButtonBox->pack_start(*pNextButton);
	pButtonBox->show();
	pBox->pack_start(*pScrolledWindow, true, true);
	pBox->pack_start(*pButtonBox, false, false);
	pBox->show();
	m_Notebook.append_page(*pBox, ssName.str());
	Client.vSetWidget(pBox);
	vClientStatusChanged(DiEDClient.GetID(), DiEDClient.GetStatus(m_System.GetLocalClientID()), boost::ref(DiEDClient));
}

void GUI::MainWindow::vInsertText(const Glib::ustring & sString, int iLine, int iCharacter)
{
	m_InsertConnection.block();
	
	Glib::RefPtr< Gtk::TextMark > Marker(m_TextBuffer->create_mark("insert-save", m_TextBuffer->get_insert()->get_iter(), true));
	
	m_TextBuffer->insert(m_TextBuffer->get_iter_at_line_offset(iLine, iCharacter), sString);
	m_TextBuffer->place_cursor(Marker->get_iter());
	m_TextBuffer->delete_mark(Marker);
	m_InsertConnection.unblock();
}

void GUI::MainWindow::vClientStatusChanged(const DiED::clientid_t & ClientID, const DiED::User::Status & Status, boost::reference_wrapper< DiED::Client > Client)
{
	DiED::Client * pDiEDClient(m_System.pGetClient(ClientID));
	
	if(pDiEDClient == 0)
	{
		// this can happen quite often during client registration.
		//  => The local client is set to Disconnected to the new client during registration but the client is not yet in the list.
		return;
	}
	
	GUI::Client * pClient(dynamic_cast< GUI::Client * >(pDiEDClient));
	
	if(pClient == 0)
	{
		std::cout << "Schade: " << __FILE__ << ':' << __LINE__ << std::endl;
		
		return;
	}
	
	Gtk::Widget * pClientWidget(pClient->pGetWidget());
	
	if(pClientWidget == 0)
	{
		std::cout << "Schade: " << __FILE__ << ':' << __LINE__ << std::endl;
		
		return;
	}
	
	Gtk::Widget * pLabel = m_Notebook.get_tab_label(*pClientWidget);
	
	switch(Status)
	{
	case DiED::User::Connected:
		{
			pLabel->modify_fg(Gtk::STATE_NORMAL, Gdk::Color(&Connected));
			pLabel->modify_fg(Gtk::STATE_ACTIVE, Gdk::Color(&Connected));
			
			break;
		}
	case DiED::User::Disconnected:
		{
			pLabel->modify_fg(Gtk::STATE_NORMAL, Gdk::Color(&Disconnected));
			pLabel->modify_fg(Gtk::STATE_ACTIVE, Gdk::Color(&Disconnected));
			
			break;
		}
	case DiED::User::Deleted:
		{
			pLabel->modify_fg(Gtk::STATE_NORMAL, Gdk::Color(&Deleted));
			pLabel->modify_fg(Gtk::STATE_ACTIVE, Gdk::Color(&Deleted));
			
			break;
		}
	case DiED::User::Connecting:
		{
			pLabel->modify_fg(Gtk::STATE_NORMAL, Gdk::Color(&Connecting));
			pLabel->modify_fg(Gtk::STATE_ACTIVE, Gdk::Color(&Connecting));
			
			break;
		}
	}
}

void GUI::MainWindow::vHoldFlowButtonClicked(Gtk::Button * pHoldFlowButton, Gtk::Button * pNextButton, boost::reference_wrapper< GUI::Client > Client)
{
	Client.get().vHoldMessagesBack(!Client.get().bIsHoldingMessagesBack());
	pHoldFlowButton->set_label(((Client.get().bIsHoldingMessagesBack() == true) ? ("Flow") : ("Hold")));
	pNextButton->set_sensitive(Client.get().bIsHoldingMessagesBack());
}

void GUI::MainWindow::vNextButtonClicked(boost::reference_wrapper< GUI::Client > Client)
{
	Client.get().vExecuteTopMessage();
}

void GUI::MainWindow::vRowInsertedForClient(const Gtk::TreePath & Path, const Gtk::TreeIter & Iterator, Gtk::TreeView * pTreeView)
{
	pTreeView->scroll_to_row(Path);
}
