#include "BasicMessage.h"

Network::BasicMessage::BasicMessage(u_int32_t u32MessageID, bool bForSending) :
	m_MessageID(u32MessageID),
	m_bForSending(bForSending)
{
}

bool Network::BasicMessage::bIsForSending(void) const
{
	return m_bForSending;
}

void Network::BasicMessage::vWriteTo(Network::Stream & Stream) const
{
	Stream << m_MessageID;
	vWriteToInternal(Stream);
}
