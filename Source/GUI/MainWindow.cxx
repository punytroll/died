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

#include "MainWindow.h"

#include <iostream>
#include <sstream>

#include <gtk/gtktextbuffer.h>

#include <gtkmm/box.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/stock.h>

#include "Client.h"

GdkColor Connecting = { 0x0000, 0xE0E0, 0xE0E0, 0x0000 };
GdkColor Connected = { 0x0000, 0x0000, 0xC0C0, 0x0000 };
GdkColor Disconnected = { 0x0000, 0xC0C0, 0x0000, 0x0000 };
GdkColor Undefined = { 0x0000, 0x6060, 0x6060, 0x6060 };

void GUI::vDeleteRange(GtkTextBuffer * pTextBuffer, GtkTextIter * pBeginIterator, GtkTextIter * pEndIterator, GUI::MainWindow * pMainWindow)
{
	LOG(Verbose, "GUI/MainWindow", "Erase signal: from iterator @ " << gtk_text_iter_get_line(pBeginIterator) << "; " << gtk_text_iter_get_line_offset(pBeginIterator));
	LOG(Verbose, "GUI/MainWindow", "Erase signal:   to iterator @ " << gtk_text_iter_get_line(pEndIterator) << "; " << gtk_text_iter_get_line_offset(pEndIterator));
	GtkTextIter InsertIterator;
	
	gtk_text_buffer_get_iter_at_mark(pTextBuffer, &InsertIterator, gtk_text_buffer_get_insert(pTextBuffer));
	if(gtk_text_iter_equal(pEndIterator, &InsertIterator) == true)
	{
		pMainWindow->m_System.vDelete(gtk_text_iter_get_line(pBeginIterator), gtk_text_iter_get_line_offset(pBeginIterator));
	}
	else
	{
		if(gtk_text_iter_equal(pBeginIterator, &InsertIterator) == true)
		{
			pMainWindow->m_System.vDelete(gtk_text_iter_get_line(pEndIterator), gtk_text_iter_get_line_offset(pEndIterator));
		}
		else
		{
			pMainWindow->m_System.vPosition(gtk_text_iter_get_line(pBeginIterator), gtk_text_iter_get_line_offset(pBeginIterator));
			pMainWindow->m_System.vDelete(gtk_text_iter_get_line(pEndIterator), gtk_text_iter_get_line_offset(pEndIterator));
			pMainWindow->m_System.vPosition(gtk_text_iter_get_line(&InsertIterator), gtk_text_iter_get_line_offset(&InsertIterator));
		}
	}
}

void GUI::vInsertText(GtkTextBuffer * pTextBuffer, GtkTextIter * pIterator, char * pcData, int, GUI::MainWindow * pMainWindow)
{
	GtkTextIter InsertIterator;
	
	gtk_text_buffer_get_iter_at_mark(pTextBuffer, &InsertIterator, gtk_text_buffer_get_insert(pTextBuffer));
	LOG(Verbose, "GUI/MainWindow", "Inserted signal: \"insert\" mark @ " << gtk_text_iter_get_line(&InsertIterator) << "; " << gtk_text_iter_get_line_offset(&InsertIterator));
	LOG(Verbose, "GUI/MainWindow", "Inserted signal: iterator      @ " << gtk_text_iter_get_line(pIterator) << "; " << gtk_text_iter_get_line_offset(pIterator));
	if(gtk_text_iter_equal(pIterator, &InsertIterator) == false)
	{
		pMainWindow->m_System.vPosition(gtk_text_iter_get_line(pIterator),  gtk_text_iter_get_line_offset(pIterator));
		pMainWindow->m_System.vInsert(pcData);
		// the correct position will be set by MarkSet.
	}
	else
	{
		pMainWindow->m_System.vInsert(pcData);
	}
}

GUI::MainWindow::MainWindow(DiED::System & System, bool bShowMessageList) :
	m_System(System),
	m_TextBuffer(m_TextView.get_buffer()),
	m_KeyPressedConnection(m_TextView.signal_key_press_event().connect(sigc::mem_fun(*this, &GUI::MainWindow::bKeyPressed), false)),
	m_ulInsertTextHandlerID(g_signal_connect(m_TextBuffer->gobj(), "insert-text", GCallback(GUI::vInsertText), this)),
	m_ulDeleteRangeHandlerID(g_signal_connect(m_TextBuffer->gobj(), "delete-range", GCallback(GUI::vDeleteRange), this)),
	m_bShowMessageList(bShowMessageList)
{
	m_System.vSetExternalEnvironment(this);
	
	Gtk::ScrolledWindow * pScrolledWindow(manage(new Gtk::ScrolledWindow()));
	
	m_TextView.show();
	pScrolledWindow->add(m_TextView);
#ifndef NODEBUG
	m_Notebook.show();
	m_Pane.show();
	m_Pane.pack1(*pScrolledWindow, true, true);
	m_Pane.pack2(m_Notebook, true, true);
	add(m_Pane);
#else
	add(*pScrolledWindow);
#endif
	set_default_size(500, 300);
	show_all();
	m_TextBuffer->signal_mark_set().connect(sigc::mem_fun(*this, &GUI::MainWindow::vMarkSet));
	m_TextBuffer->signal_changed().connect(sigc::mem_fun(*this, &GUI::MainWindow::vChanged));
}

GUI::MainWindow::~MainWindow(void)
{
	m_StatusChangedConnection.disconnect();
}

bool GUI::MainWindow::bKeyPressed(GdkEventKey * pEvent)
{
	return false;
}

void GUI::MainWindow::vMarkSet(const Gtk::TextIter & Iterator, const Glib::RefPtr< Gtk::TextMark > & Mark)
{
	if(Mark->get_name() == "insert")
	{
		LOG(Verbose, "GUI/MainWindow", "MarkSet signal: \"insert\" mark @ " << Mark->get_iter().get_line() << "; " << Mark->get_iter().get_line_offset());
		m_System.vPosition(Mark->get_iter().get_line(), Mark->get_iter().get_line_offset());
	}
}

void GUI::MainWindow::vChanged(void)
{
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

#ifndef NODEBUG
	GUI::Client & Client(dynamic_cast< GUI::Client & >(DiEDClient));
	std::stringstream ssName;
	
	ssName << Client.GetID();
	
	Gtk::Widget * pClientWidget(0);
	Gtk::HButtonBox * pButtonBox(manage(new Gtk::HButtonBox()));
	Gtk::Button * pConnectButton(manage(new Gtk::Button("Connect")));
	Gtk::Button * pPingButton(manage(new Gtk::Button("Ping")));
	
	if(m_bShowMessageList == true)
	{
		Gtk::VBox * pBox(manage(new Gtk::VBox()));
		Gtk::ScrolledWindow * pScrolledWindow(manage(new Gtk::ScrolledWindow()));
		Gtk::TreeView * pClientView(manage(new Gtk::TreeView(Client.GetMessageListStore())));
		Gtk::Button * pHoldFlowButton(manage(new Gtk::Button("Hold/Flow")));
		Gtk::Button * pNextButton(manage(new Gtk::Button("Next")));
		Gtk::CellRendererText * pCellRenderer = 0;
		
		pClientWidget = pBox;
		pClientView->append_column("Name", Client.GetMessageListStore()->Columns.Name);
		pClientView->append_column("Status", Client.GetMessageListStore()->Columns.Status);
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
		pBox->pack_start(*pScrolledWindow, true, true);
		pBox->pack_start(*pButtonBox, false, false);
		pButtonBox->pack_start(*pHoldFlowButton);
		pButtonBox->pack_start(*pNextButton);
		pBox->show();
	}
	else
	{
		pClientWidget = pButtonBox;
	}
	pConnectButton->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &GUI::MainWindow::vConnectButtonClicked), boost::ref(Client)));
	pConnectButton->show();
	pPingButton->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &GUI::MainWindow::vPingButtonClicked), boost::ref(Client)));
	pPingButton->show();
	pButtonBox->pack_start(*pConnectButton);
	pButtonBox->pack_start(*pPingButton);
	pButtonBox->show();
	Client.vSetWidget(pClientWidget);
	m_Notebook.append_page(*pClientWidget, ssName.str());
	vClientStatusChanged(DiEDClient.GetID(), DiEDClient.GetStatus(m_System.GetLocalClientID()), boost::ref(DiEDClient));
#endif
}

void GUI::MainWindow::vInsert(const Glib::ustring & sString, int iLine, int iCharacter)
{
	LOG(DebugGUI, "GUI/MainWindow", "Inserting " << sString << " @ Line = " << iLine << " ; Character = " << iCharacter);
	
	// HACK!!!
	if(iLine < 0)
	{
		iLine = 0;
	}
	if(iCharacter < 0)
	{
		iCharacter = 0;
	}
	if(iLine > m_TextBuffer->get_line_count())
	{
		iLine = m_TextBuffer->get_line_count();
	}
	
	Gtk::TextIter Iterator(m_TextBuffer->get_iter_at_line(iLine));
	
	if((Iterator.forward_to_line_end() == true) || (Iterator.is_end() == true))
	{
		if(iCharacter > Iterator.get_line_offset())
		{
			iCharacter = Iterator.get_line_offset();
		}
	}
	else
	{
		throw;
	}
//~ 	m_InsertConnection.block();
	g_signal_handler_block(m_TextBuffer->gobj(), m_ulInsertTextHandlerID);
	
	// construct a left-gravity save for the cursor
	Glib::RefPtr< Gtk::TextMark > Marker(m_TextBuffer->create_mark("insert-save", m_TextBuffer->get_insert()->get_iter(), true));
	
	m_TextBuffer->insert(m_TextBuffer->get_iter_at_line_offset(iLine, iCharacter), sString);
	m_TextBuffer->place_cursor(Marker->get_iter());
	m_TextBuffer->delete_mark(Marker);
	g_signal_handler_unblock(m_TextBuffer->gobj(), m_ulInsertTextHandlerID);
//~ 	m_InsertConnection.unblock();
	LOG(DebugGUI, "GUI/MainWindow", "         ... done inserting.");
}

void GUI::MainWindow::vDelete(int iFromLine, int iFromCharacter, int iToLine, int iToCharacter)
{
//~ 	m_EraseConnection.block();
	g_signal_handler_block(m_TextBuffer->gobj(), m_ulDeleteRangeHandlerID);
	
	// construct a left-gravity save for the cursor
	Glib::RefPtr< Gtk::TextMark > Marker(m_TextBuffer->create_mark("insert-save", m_TextBuffer->get_insert()->get_iter(), true));
	Gtk::TextIter FromIterator(m_TextBuffer->get_iter_at_line_offset(iFromLine, iFromCharacter));
	Gtk::TextIter ToIterator(m_TextBuffer->get_iter_at_line_offset(iToLine, iToCharacter));
	
	m_TextBuffer->erase(FromIterator, ToIterator);
	m_TextBuffer->place_cursor(Marker->get_iter());
	m_TextBuffer->delete_mark(Marker);
	g_signal_handler_unblock(m_TextBuffer->gobj(), m_ulDeleteRangeHandlerID);
//~ 	m_EraseConnection.unblock();
}

int GUI::MainWindow::iGetNumberOfLines(void) const
{
	return m_TextBuffer->get_line_count();
}

int GUI::MainWindow::iGetLengthOfLine(int iLine) const
{
	Gtk::TextIter Iterator(m_TextBuffer->get_iter_at_line(iLine));
	
	return Iterator.get_chars_in_line();
}

Glib::ustring GUI::MainWindow::sGetDocument(void) const
{
	return m_TextBuffer->get_text();
}

void GUI::MainWindow::vSetDocument(const Glib::ustring & sDocument)
{
//~ 	m_InsertConnection.block();
	g_signal_handler_block(m_TextBuffer->gobj(), m_ulInsertTextHandlerID);
	m_TextBuffer->set_text(sDocument);
	m_TextBuffer->place_cursor(m_TextBuffer->begin());
	g_signal_handler_unblock(m_TextBuffer->gobj(), m_ulInsertTextHandlerID);
}

int GUI::MainWindow::iGetLine(void) const
{
	return m_TextBuffer->get_iter_at_mark(m_TextBuffer->get_mark("insert")).get_line();
}

int GUI::MainWindow::iGetCharacter(void) const
{
	return m_TextBuffer->get_iter_at_mark(m_TextBuffer->get_mark("insert")).get_line_offset();
}

void GUI::MainWindow::vClientStatusChanged(const DiED::clientid_t & ClientID, const DiED::clientstatus_t & Status, boost::reference_wrapper< DiED::Client > Client)
{
#ifndef NODEBUG
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
	case DiED::Undefined:
		{
			pLabel->modify_fg(Gtk::STATE_NORMAL, Gdk::Color(&Undefined));
			pLabel->modify_fg(Gtk::STATE_ACTIVE, Gdk::Color(&Undefined));
			
			break;
		}
	case DiED::Connecting:
		{
			pLabel->modify_fg(Gtk::STATE_NORMAL, Gdk::Color(&Connecting));
			pLabel->modify_fg(Gtk::STATE_ACTIVE, Gdk::Color(&Connecting));
			
			break;
		}
	case DiED::Connected:
		{
			pLabel->modify_fg(Gtk::STATE_NORMAL, Gdk::Color(&Connected));
			pLabel->modify_fg(Gtk::STATE_ACTIVE, Gdk::Color(&Connected));
			
			break;
		}
	case DiED::Disconnected:
		{
			pLabel->modify_fg(Gtk::STATE_NORMAL, Gdk::Color(&Disconnected));
			pLabel->modify_fg(Gtk::STATE_ACTIVE, Gdk::Color(&Disconnected));
			
			break;
		}
	case DiED::Local:
		{
			LOG(Error, "GUI/MainWindow", "This should not happen: " << __FILE__ << ':' << __LINE__);
			
			break;
		}
	}
#endif
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

void GUI::MainWindow::vConnectButtonClicked(boost::reference_wrapper< GUI::Client > Client)
{
	m_System.vTryReconnect(Client.get().GetID());
}

void GUI::MainWindow::vPingButtonClicked(boost::reference_wrapper< GUI::Client > Client)
{
	Client.get().vPing();
}

void GUI::MainWindow::vRowInsertedForClient(const Gtk::TreePath & Path, const Gtk::TreeIter & Iterator, Gtk::TreeView * pTreeView)
{
	pTreeView->scroll_to_row(Path);
}

bool GUI::MainWindow::on_delete_event(GdkEventAny * pEvent)
{
	m_System.vLogOut();
	
	return false;
}
