#ifndef GUI_MAINWINDOW_H
#define GUI_MAINWINDOW_H

#include <gtkmm/textview.h>
#include <gtkmm/window.h>

#include <DiED/System.h>

namespace GUI
{
	class MainWindow : public Gtk::Window, public DiED::ExternalEnvironment
	{
	public:
		MainWindow(DiED::System & System);
	protected:
		void vInserted(const Gtk::TextBuffer::iterator & Iterator, const Glib::ustring & sString, int);
		virtual void vInsertText(const Glib::ustring & sString, int iLine, int iCharacter);
	private:
		DiED::System & m_System;
		Gtk::TextView m_TextView;
		Glib::RefPtr< Gtk::TextBuffer > m_TextBuffer;
		sigc::connection m_InsertConnection;
	};
}

#endif
