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
	u_int16_t u16ServerPort = 5867;
	u_int16_t u16ConnectPort = 5867;
	std::string sConnectAddress = "";
	int iI = 1;
	
	while(iI < argc)
	{
		if(std::string(argv[iI]) == "serve")
		{
			++iI;
			
			std::stringstream ssPort(argv[iI]);
			
			ssPort >> u16ServerPort;
		}
		else if(std::string(argv[iI]) == "connect")
		{
			++iI;
			sConnectAddress = argv[iI];
			
			std::string::size_type stPort = sConnectAddress.rfind(':');
			
			if(stPort != std::string::npos)
			{
				std::stringstream ssPort(sConnectAddress.substr(stPort + 1));
				
				ssPort >> u16ConnectPort;
				sConnectAddress = sConnectAddress.substr(0, stPort);
			}
		}
		++iI;
	}
	
	Gtk::Main Main(argc, argv);
	DiED::System DiEDSystem;
	boost::shared_ptr< GUI::ClientFactory > ClientFactory(new GUI::ClientFactory(DiEDSystem));
	
	DiEDSystem.vSetClientFactory(ClientFactory);
	DiEDSystem.bListen(u16ServerPort);
	if(sConnectAddress != "")
	{
		DiEDSystem.bConnectTo(sConnectAddress, u16ConnectPort);
	}
	
	GUI::MainWindow MainWindow(DiEDSystem);
	
	Main.run(MainWindow);
}
