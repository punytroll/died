#include "BasicMessage.h"

Network::BasicMessage::BasicMessage(u_int32_t u32Type, bool bForSending) :
	m_Type(u32Type),
	m_bForSending(bForSending)
{
}

Network::BasicMessage::~BasicMessage(void)
{
}

bool Network::BasicMessage::bIsForSending(void) const
{
	return m_bForSending;
}

Network::BasicMessage::type_t Network::BasicMessage::GetType(void) const
{
	return m_Type;
}

void Network::BasicMessage::vWriteTo(Network::Stream & Stream) const
{
	Stream << m_Type;
	vWriteToInternal(Stream);
}
