#include "MainWindow.h"

#include <iostream>

GUI::MainWindow::MainWindow(DiED::System & System) :
	m_System(System),
	m_TextBuffer(m_TextView.get_buffer()),
	m_InsertConnection(m_TextBuffer->signal_insert().connect(sigc::mem_fun(*this, &GUI::MainWindow::vInserted)))
{
	m_System.vSetExternalEnvironment(this);
	m_TextView.show();
	add(m_TextView);
	set_default_size(400, 300);
}

void GUI::MainWindow::vInserted(const Gtk::TextBuffer::iterator & Iterator, const Glib::ustring & sString, int)
{
	std::cout << "Inserted text: " << sString << std::endl;
	m_System.vInput(sString);
}

void GUI::MainWindow::vInsertText(const Glib::ustring & sString, int iLine, int iCharacter)
{
	m_InsertConnection.block();
	m_TextBuffer->insert(m_TextBuffer->get_iter_at_line_offset(iLine, iCharacter), sString);
	m_InsertConnection.unblock();
}
