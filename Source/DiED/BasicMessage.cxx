#include "BasicMessage.h"

DiED::BasicMessage::BasicMessage(const Network::BasicMessage::type_t & Type) :
	Network::BasicMessage(Type)
{
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

bool DiED::BasicMessage::bOnTimeout(DiED::MessageTarget * pMessageTarget)
{
	return false;
}
