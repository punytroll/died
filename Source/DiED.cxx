#include <iostream>
#include <sstream>

#include "Common.h"
#include "DiED/Client.h"
#include "DiED/ClientFactory.h"
#include "DiED/MessageFactory.h"
#include "DiED/Messages.h"
#include "DiED/Server.h"

bool g_bDone = false;
std::vector< boost::shared_ptr< Network::Socket > > g_Clients;

void vAccepted(boost::shared_ptr< Network::Socket > Client)
{
	g_Clients.push_back(Client);
}

int main(int argc, char ** argv)
{
	u_int16_t u16ServerPort = 6666;
	u_int16_t u16ConnectPort = 6666;
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
	
	DiED::MessageFactory MessageFactory;
	DiED::ClientFactory ClientFactory(MessageFactory);
	DiED::Server Server(ClientFactory);
	DiED::Client Client(MessageFactory);
	
	Server.vOpen(u16ServerPort);
	if(Server.bIsOpen() == false)
	{
		std::cerr << "[Server]: Error setting up the server. [" << sErrorCodeToString(Server.iGetError()) << "]." << std::endl;
	}
	else
	{
		Server.Accepted.connect(sigc::ptr_fun(vAccepted));
	}
	if(sConnectAddress != "")
	{
		Client.vOpen(sConnectAddress, u16ConnectPort);
		if(Client.bIsOpen() == false)
		{
			std::cout << "[Client]: Connection failed." << std::endl;
		}
		else
		{
			std::cout << "[Client]: Connected to " << sConnectAddress << ':' << u16ConnectPort << std::endl;
			
			Client << DiED::HelloMessage();
		}
	}
	while(g_bDone == false)
	{
		Glib::MainContext::get_default()->iteration(true);
	}
}
