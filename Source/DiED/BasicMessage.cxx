#include "BasicMessage.h"

DiED::BasicMessage::BasicMessage(const Network::BasicMessage::type_t & Type) :
	Network::BasicMessage(Type)
{
}

DiED::BasicMessage::~BasicMessage(void)
{
	if(m_TimeoutConnection)
	{
		m_TimeoutConnection.disconnect();
	}
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

bool DiED::BasicMessage::bOnTimeout(DiED::MessageTarget * pMessageTarget)
{
	return false;
}
