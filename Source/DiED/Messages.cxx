#include "Messages.h"

DiED::NoMessage::NoMessage(void) :
	Network::BasicMessage(false)
{
}

bool DiED::NoMessage::bIsReady(void)
{
	return true;
}

void DiED::NoMessage::vReadFrom(Network::Stream & Stream)
{
}

DiED::HelloMessage::HelloMessage(void) :
	Network::BasicMessage(true)
{
}

bool DiED::HelloMessage::bIsReady(void)
{
	return true;
}

void DiED::HelloMessage::vReadFrom(Network::Stream & Stream)
{
}
