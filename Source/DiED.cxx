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
	g_bLogLevels[TODO] = true;
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
		++iI;
	}
	
	Gtk::Main Main(argc, argv);
	DiED::System DiEDSystem;
	GUI::MainWindow MainWindow(DiEDSystem);
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
