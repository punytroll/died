#include "Client.h"

#include <iostream>

#include "BasicMessage.h"

DiED::Client::Client(boost::shared_ptr< Network::MessageFactory > MessageFactory, DiED::InternalEnvironment & InternalEnvironment) :
	Network::MessageStream(MessageFactory),
	m_InternalEnvironment(InternalEnvironment),
	m_iLine(0),
	m_iCharacter(0)
{
	std::cout << "[DiED/Client]: Created new Client." << std::endl;
}

DiED::Client::Client(int iSocket, boost::shared_ptr< Network::MessageFactory > MessageFactory, DiED::InternalEnvironment & InternalEnvironment) :
	Network::MessageStream(iSocket, MessageFactory),
	m_InternalEnvironment(InternalEnvironment),
	m_iLine(0),
	m_iCharacter(0)
{
	std::cout << "[DiED/Client]: Created new Client from socket." << std::endl;
}

void DiED::Client::vInsertText(const Glib::ustring & sString)
{
	m_InternalEnvironment.vInsertText(*this, sString);
}

void DiED::Client::vModifyCaretPosition(int iDeltaLine, int iDeltaCharacter)
{
	m_iLine += iDeltaLine;
	if(iDeltaLine == 0)
	{
		m_iCharacter += iDeltaCharacter;
	}
	else
	{
		m_iCharacter = iDeltaCharacter;
	}
}

void DiED::Client::vMessageReady(void)
{
	boost::shared_ptr< Network::BasicMessage > Message(rbegin());
	
	std::cout << "[GUI/Client]: Retrieved message with type " << Message->u32GetType() << '.' << std::endl;
	vExecuteTopMessage();
}

void DiED::Client::vExecuteTopMessage(void)
{
	boost::shared_ptr< Network::BasicMessage > Message(PopMessage());
	
	std::cout << "[GUI/Client]: Executing message with type " << Message->u32GetType() << '.' << std::endl;
	
	DiED::BasicMessage & DiEDMessage = dynamic_cast< DiED::BasicMessage & >(*Message);
	
	DiEDMessage.vExecute(*this);
}
