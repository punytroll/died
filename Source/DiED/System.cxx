#include "System.h"

#include <iostream>

#include "../Common.h"
#include "Client.h"
#include "Messages.h"

DiED::System::System(void) :
	m_MessageFactory(new DiED::MessageFactory()),
	m_Client(new DiED::Client(m_MessageFactory, *this))
{
	m_Clients.push_back(m_Client);
}

DiED::System::~System(void)
{
}

void DiED::System::vSetExternalEnvironment(DiED::ExternalEnvironment * pExternalEnvironment)
{
	m_pExternalEnvironment = pExternalEnvironment;
}

boost::shared_ptr< Network::MessageFactory > DiED::System::GetMessageFactory(void)
{
	return m_MessageFactory;
}

DiED::Server & DiED::System::GetServer(void)
{
	return m_Server;
}

bool DiED::System::bListen(const Network::port_t & ServicePort)
{
	m_Server.vOpen(ServicePort);
	if(m_Server.bIsOpen() == false)
	{
		std::cerr << "[Server]: Error setting up the server. [" << sErrorCodeToString(m_Server.iGetError()) << "]." << std::endl;
		
		return false;
	}
	else
	{
		m_Server.Accepted.connect(sigc::mem_fun(*this, &DiED::System::vAccepted));
	}
	
	return true;
}

bool DiED::System::bConnectTo(const Network::address_t & ConnectAddress, const Network::port_t & ConnectPort)
{
	boost::shared_ptr< DiED::Client > Client(new DiED::Client(m_MessageFactory, *this));
	
	Client->vOpen(ConnectAddress, ConnectPort);
	if(Client->bIsOpen() == false)
	{
		std::cout << "[Client]: Connection failed." << std::endl;
	}
	else
	{
		std::cout << "[Client]: Connected to " << ConnectAddress << ':' << ConnectPort << std::endl;
	}
	m_Clients.push_back(Client);
}

void DiED::System::vInput(const Glib::ustring & sString)
{
	DiED::InputMessage Message(sString);
	
	vSendMessage(Message);
	vInsertText(*m_Client, sString, false);
}

void DiED::System::vInsertText(DiED::User & User, const Glib::ustring & sString)
{
	vInsertText(User, sString, true);
}

void DiED::System::vInsertText(DiED::User & User, const Glib::ustring & sString, bool bWriteToEnvironment)
{
	int iLine(User.iGetLine());
	int iCharacter(User.iGetCharacter());
	
	if((m_pExternalEnvironment != 0) && (bWriteToEnvironment == true))
	{
		m_pExternalEnvironment->vInsertText(sString, iLine, iCharacter);
	}
	
	int iDeltaLine = 0;
	int iDeltaCharacter = 0;
	
	for(Glib::ustring::size_type stI = 0; stI < sString.length(); ++stI)
	{
		if(sString[stI] == '\n')
		{
			++iDeltaLine;
			iDeltaCharacter = 0;
		}
		else
		{
			++iDeltaCharacter;
		}
	}
	
	std::vector< boost::shared_ptr< Network::Socket > >::iterator iClient = m_Clients.begin();
	
	while(iClient != m_Clients.end())
	{
		DiED::User & OtherUser = dynamic_cast< DiED::User & >(**iClient);
		
		if((&User == &OtherUser) || (iLine < OtherUser.iGetLine()) || ((iLine == OtherUser.iGetLine()) && (iCharacter < OtherUser.iGetCharacter())))
		{
			if(iDeltaLine == 0)
			{
				OtherUser.vModifyCaretPosition(iDeltaLine, iDeltaCharacter);
			}
			else
			{
				OtherUser.vModifyCaretPosition(iDeltaLine, OtherUser.iGetCharacter() - iCharacter + iDeltaCharacter);
			}
		}
		++iClient;
	}
}

void DiED::System::vSendMessage(Network::BasicMessage & Message)
{
	std::vector< boost::shared_ptr< Network::Socket > >::iterator iClient = m_Clients.begin();
	
	while(iClient != m_Clients.end())
	{
		Network::MessageStream & Stream(dynamic_cast< Network::MessageStream & >(**iClient));
		
		Stream << Message;
		++iClient;
	}
}

void DiED::System::vAccepted(boost::shared_ptr< Network::Socket > Socket)
{
	m_Clients.push_back(Socket);
	
	DiED::Client & Client(dynamic_cast< DiED::Client & >(*Socket));
	
	Client << ConnectionEstablishedMessage(12345, "blubblub.org", 9876);
}
