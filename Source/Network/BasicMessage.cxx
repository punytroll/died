#include "BasicMessage.h"

Network::BasicMessage::BasicMessage(bool bForSending) :
	m_bForSending(bForSending)
{
}

bool Network::BasicMessage::bIsForSending(void) const
{
	return m_bForSending;
}
