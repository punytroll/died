#include "BasicMessage.h"

DiED::BasicMessage::BasicMessage(const Network::BasicMessage::type_t & Type, bool bForSending) :
	Network::BasicMessage(Type, bForSending)
{
}
