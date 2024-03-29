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
		MainWindow(DiED::System & System, bool bMessageList = false);
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
		void vNextButtonClicked(boost::reference_wrapper< GUI::Client > Client);
		void vConnectButtonClicked(boost::reference_wrapper< GUI::Client > Client);
		void vPingButtonClicked(boost::reference_wrapper< GUI::Client > Client);
		void vRowInsertedForClient(const Gtk::TreePath & Path, const Gtk::TreeIter & Iterator, Gtk::TreeView * pTreeView);
	private:
		virtual bool on_delete_event(GdkEventAny * pEvent);
		DiED::System & m_System;
#ifndef NODEBUG
		Gtk::VPaned m_Pane;
		Gtk::Notebook m_Notebook;
#endif
		Gtk::TextView m_TextView;
		Glib::RefPtr< Gtk::TextBuffer > m_TextBuffer;
		sigc::connection m_KeyPressedConnection;
//~ 		sigc::connection m_InsertConnection;
//~ 		sigc::connection m_EraseConnection;
		sigc::connection m_StatusChangedConnection;
		gulong m_ulInsertTextHandlerID;
		gulong m_ulDeleteRangeHandlerID;
		bool m_bShowMessageList;
	};
}

#endif
