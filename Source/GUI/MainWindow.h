#ifndef GUI_MAINWINDOW_H
#define GUI_MAINWINDOW_H

#include <gtkmm/notebook.h>
#include <gtkmm/paned.h>
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
		bool bKeyPressed(GdkEventKey * pEvent);
		
		// implementation of DiED::ExternalEnvironment
		virtual void vClientConnected(DiED::Client & Client);
		virtual void vInsertText(const Glib::ustring & sString, int iLine, int iCharacter);
	private:
		DiED::System & m_System;
		Gtk::VPaned m_Pane;
		Gtk::Notebook m_Notebook;
		Gtk::TextView m_TextView;
		Glib::RefPtr< Gtk::TextBuffer > m_TextBuffer;
		sigc::connection m_KeyPressedConnection;
		sigc::connection m_InsertConnection;
	};
}

#endif
