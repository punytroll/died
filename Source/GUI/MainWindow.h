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
	class MainWindow;
	
	void vDeleteRange(GtkTextBuffer * pTextBuffer, GtkTextIter * pBeginIterator, GtkTextIter * pEndIterator, GUI::MainWindow * pMainWindow);
	void vInsertText(GtkTextBuffer * pTextBuffer, GtkTextIter * pIterator, char * pcData, int, GUI::MainWindow * pMainWindow);
	
	class MainWindow : public Gtk::Window, public DiED::ExternalEnvironment
	{
		friend void vDeleteRange(GtkTextBuffer * pTextBuffer, GtkTextIter * pBeginIterator, GtkTextIter * pEndIterator, GUI::MainWindow * pMainWindow);
		friend void vInsertText(GtkTextBuffer * pTextBuffer, GtkTextIter * pIterator, char * pcData, int, GUI::MainWindow * pMainWindow);
	public:
		MainWindow(DiED::System & System);
		~MainWindow(void);
	protected:
		void vInserted(const Gtk::TextBuffer::iterator & Iterator, const Glib::ustring & sString, int);
		void vErase(const Gtk::TextBuffer::iterator & BeginIterator, const Gtk::TextBuffer::iterator & EndIterator);
		bool bKeyPressed(GdkEventKey * pEvent);
		void vMarkSet(const Gtk::TextIter & Iterator, const Glib::RefPtr< Gtk::TextMark > & Mark);
		void vChanged(void);
		
		// implementation of DiED::ExternalEnvironment
		virtual void vNewClient(DiED::Client & Client);
		virtual void vInsert(const Glib::ustring & sString, int iLine, int iCharacter);
		virtual void vDelete(int iFromLine, int iFromCharacter, int iToLine, int iToCharacter);
		virtual int iGetNumberOfLines(void) const;
		virtual int iGetLengthOfLine(int iLine) const;
		virtual Glib::ustring sGetDocument(void) const;
		virtual void vSetDocument(const Glib::ustring & sDocument);
		virtual int iGetLine(void) const;
		virtual int iGetCharacter(void) const;
		
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
//~ 		sigc::connection m_InsertConnection;
//~ 		sigc::connection m_EraseConnection;
		sigc::connection m_StatusChangedConnection;
		gulong m_ulInsertTextHandlerID;
		gulong m_ulDeleteRangeHandlerID;
	};
}

#endif
