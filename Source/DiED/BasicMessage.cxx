#include "BasicMessage.h"

#include <iostream>

DiED::BasicMessage::BasicMessage(const Network::BasicMessage::type_t & Type) :
	Network::BasicMessage(Type)
{
}

DiED::BasicMessage::~BasicMessage(void)
{
	vDisconnectTimeout();
}

boost::shared_ptr< DiED::ConfirmationParameters > DiED::BasicMessage::GetConfirmationParameters(void)
{
	return boost::shared_ptr< DiED::ConfirmationParameters >();
}

bool DiED::BasicMessage::bIsConfirmedBy(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters)
{
	return false;
}

bool DiED::BasicMessage::bIsEventMessage(void)
{
	return false;
}

bool DiED::BasicMessage::bRequiresConfirmation(void)
{
	return false;
}

void DiED::BasicMessage::vTriggerTimeout(DiED::MessageTarget * pMessageTarget, unsigned int uiMilliSeconds)
{
	m_TimeoutConnection = Glib::signal_timeout().connect(sigc::bind(sigc::mem_fun(*this, &DiED::BasicMessage::bOnTimeout), pMessageTarget), uiMilliSeconds);
}

void DiED::BasicMessage::vDisconnectTimeout(void)
{
	if(m_TimeoutConnection)
	{
		m_TimeoutConnection.disconnect();
	}
}

bool DiED::BasicMessage::bOnTimeout(DiED::MessageTarget * pMessageTarget)
{
	return false;
}

boost::shared_ptr< DiED::BasicMessage > DiED::BasicMessage::Clone(void)
{
	std::cout << "VERY BAD: " << __FILE__ << ':' << __LINE__ << " no Clone function for MessageType " << GetType() << std::endl;
	
	return boost::shared_ptr< DiED::BasicMessage >();
}
