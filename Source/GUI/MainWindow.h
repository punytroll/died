#ifndef GUI_MAINWINDOW_H
#define GUI_MAINWINDOW_H

#include <gtkmm/notebook.h>
#include <gtkmm/paned.h>
#include <gtkmm/textview.h>
#include <gtkmm/treeview.h>
#include <gtkmm/window.h>

#include <DiED/System.h>

#include "Client.h"

namespace GUI
{
	class MainWindow : public Gtk::Window, public DiED::ExternalEnvironment
	{
	public:
		MainWindow(DiED::System & System);
		~MainWindow(void);
	protected:
		void vInserted(const Gtk::TextBuffer::iterator & Iterator, const Glib::ustring & sString, int);
		bool bKeyPressed(GdkEventKey * pEvent);
		
		// implementation of DiED::ExternalEnvironment
		virtual void vNewClient(DiED::Client & Client);
		virtual void vInsertText(const Glib::ustring & sString, int iLine, int iCharacter);
		virtual int iGetNumberOfLines(void);
		virtual int iGetLengthOfLine(int iLine);
		
		// callbacks
		void vClientStatusChanged(const DiED::clientid_t & ClientID, const DiED::clientstatus_t & Status, boost::reference_wrapper< DiED::Client > Client);
		void vHoldFlowButtonClicked(Gtk::Button * pHoldFlowButton, Gtk::Button * pNextButton, boost::reference_wrapper< GUI::Client > Client);
		void vPingButtonClicked(boost::reference_wrapper< GUI::Client > Client);
		void vNextButtonClicked(boost::reference_wrapper< GUI::Client > Client);
		void vRowInsertedForClient(const Gtk::TreePath & Path, const Gtk::TreeIter & Iterator, Gtk::TreeView * pTreeView);
	private:
		DiED::System & m_System;
		Gtk::VPaned m_Pane;
		Gtk::Notebook m_Notebook;
		Gtk::TextView m_TextView;
		Glib::RefPtr< Gtk::TextBuffer > m_TextBuffer;
		sigc::connection m_KeyPressedConnection;
		sigc::connection m_InsertConnection;
		sigc::connection m_StatusChangedConnection;
	};
}

#endif
