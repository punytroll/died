#include "BasicMessage.h"

DiED::BasicMessage::BasicMessage(const Network::BasicMessage::type_t & Type) :
	Network::BasicMessage(Type)
{
}

bool DiED::BasicMessage::bIsEventMessage(void)
{
	return false;
}

bool DiED::BasicMessage::bRequiresConfirmation(void)
{
	return false;
}
