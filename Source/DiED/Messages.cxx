#include "Messages.h"

DiED::NoMessage::NoMessage(void) :
	Network::BasicMessage(DiED::_NoMessage, false)
{
}

bool DiED::NoMessage::bIsReady(void)
{
	return true;
}

void DiED::NoMessage::vReadFrom(Network::Stream & Stream)
{
}

void DiED::NoMessage::vWriteToInternal(Network::Stream & Stream) const
{
}

DiED::HelloMessage::HelloMessage(void) :
	Network::BasicMessage(DiED::_HelloMessage, true)
{
}

bool DiED::HelloMessage::bIsReady(void)
{
	return true;
}

void DiED::HelloMessage::vReadFrom(Network::Stream & Stream)
{
}

void DiED::HelloMessage::vWriteToInternal(Network::Stream & Stream) const
{
}
