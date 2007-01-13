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

#include <iostream>
#include <sstream>

#include <gtkmm/main.h>

#include "Common.h"
#include "DiED/System.h"
#include "GUI/ClientFactory.h"
#include "GUI/MainWindow.h"

bool g_bDone = false;

int main(int argc, char ** argv)
{
#ifndef NODEBUG
	g_bLogLevels[DebugCurrent] = true;
	g_bLogLevels[Info] = true;
	g_bLogLevels[Error] = true;
	g_bLogLevels[TODO] = true;
	bool bDebugWindow = false;
#endif
	srand(time(0));
	
	Network::port_t ServerPort = 5867;
	Network::port_t ConnectPort = 5867;
	Network::address_t ConnectAddress = "";
	int iI = 1;
	
	while(iI < argc)
	{
		if(std::string(argv[iI]) == "--serve")
		{
			++iI;
			
			std::stringstream ssPort(argv[iI]);
			
			ssPort >> ServerPort;
		}
		else if(std::string(argv[iI]) == "--connect")
		{
			++iI;
			ConnectAddress = argv[iI];
			
			std::string::size_type stPort = ConnectAddress.rfind(':');
			
			if(stPort != std::string::npos)
			{
				std::stringstream ssPort(ConnectAddress.substr(stPort + 1));
				
				ssPort >> ConnectPort;
				ConnectAddress = ConnectAddress.substr(0, stPort);
			}
		}
		else if(std::string(argv[iI]) == "--debug")
		{
			bDebugWindow = true;
		}
		++iI;
	}
	
	Gtk::Main Main(argc, argv);
	DiED::System DiEDSystem;
	GUI::MainWindow MainWindow(DiEDSystem, bDebugWindow);
	boost::shared_ptr< DiED::ClientFactory > DummyFactoryFactory(new DiED::ClientFactory(DiEDSystem));
	boost::shared_ptr< GUI::ClientFactory > ClientFactory(new GUI::ClientFactory(DiEDSystem));
	
	DiEDSystem.vSetClientFactory(ClientFactory);
	if(ServerPort != 0)
	{
		DiEDSystem.bListen(ServerPort);
	}
	if(ConnectAddress != "")
	{
		DiEDSystem.bConnectTo(ConnectAddress, ConnectPort);
	}
	Main.run(MainWindow);
}
