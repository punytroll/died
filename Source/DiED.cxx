#include <iostream>
#include <sstream>

#include "Common.h"
#include "DiED/System.h"

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
	
	DiED::System DiEDSystem;
	
	DiEDSystem.bListen(u16ServerPort);
	if(sConnectAddress != "")
	{
		DiEDSystem.bConnectTo(sConnectAddress, u16ConnectPort);
	}
	while(g_bDone == false)
	{
		Glib::MainContext::get_default()->iteration(true);
	}
}
