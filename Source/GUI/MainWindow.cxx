#include "MainWindow.h"

#include <iostream>
#include <sstream>

#include <gtkmm/box.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/stock.h>
#include <gtkmm/treeview.h>

#include "Client.h"

GUI::MainWindow::MainWindow(DiED::System & System) :
	m_System(System),
	m_TextBuffer(m_TextView.get_buffer()),
	m_KeyPressedConnection(m_TextView.signal_key_press_event().connect(sigc::mem_fun(*this, &GUI::MainWindow::bKeyPressed), false)),
	m_InsertConnection(m_TextBuffer->signal_insert().connect(sigc::mem_fun(*this, &GUI::MainWindow::vInserted)))
{
	m_System.vSetExternalEnvironment(this);
	m_TextView.show();
	m_Notebook.show();
	m_Pane.show();
	m_Pane.pack1(m_TextView, true, true);
	m_Pane.pack2(m_Notebook, true, true);
	add(m_Pane);
	set_default_size(500, 300);
	show_all();
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
	GUI::Client & Client(dynamic_cast< GUI::Client & >(DiEDClient));
	Gtk::VBox * pBox(manage(new Gtk::VBox()));
	Gtk::ScrolledWindow * pScrolledWindow(manage(new Gtk::ScrolledWindow()));
	Gtk::TreeView * pClientView(manage(new Gtk::TreeView(Client.GetMessageListStore())));
	Gtk::HButtonBox * pButtonBox(manage(new Gtk::HButtonBox()));
	Gtk::Button * pHoldFlowButton(manage(new Gtk::Button(Gtk::Stock::STOP)));
	Gtk::Button * pNextButton(manage(new Gtk::Button(Gtk::Stock::EXECUTE)));
	std::stringstream ssName;
	
	ssName << Client.GetClientID();
	pClientView->append_column("Name", Client.GetMessageListStore()->Columns.Name);
	pClientView->append_column("Status", Client.GetMessageListStore()->Columns.Status);
	pClientView->show();
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
	DiEDClient.ClientIDChanged.connect(sigc::bind(sigc::mem_fun(*this, &GUI::MainWindow::vClientIDChanged), boost::ref(DiEDClient), pBox));
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

void GUI::MainWindow::vClientIDChanged(boost::reference_wrapper< DiED::Client > Client, Gtk::Widget * pClientWidget)
{
	std::stringstream ssName;
	
	ssName << Client.get().GetClientID();
	m_Notebook.set_tab_label_text(*pClientWidget, ssName.str());
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
