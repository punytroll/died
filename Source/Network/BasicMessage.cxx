#include "BasicMessage.h"

Network::BasicMessage::BasicMessage(u_int32_t u32MessageID, bool bForSending) :
	m_MessageID(u32MessageID),
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

void Network::BasicMessage::vWriteTo(Network::Stream & Stream) const
{
	Stream << m_MessageID;
	vWriteToInternal(Stream);
}

void Network::BasicMessage::vRegisterValue(Network::BasicValue & Value)
{
	Value.Ready.connect(sigc::mem_fun(*this, &Network::BasicMessage::vNotifyValueReady));
}

void Network::BasicMessage::vNotifyValueReady(void)
{
	if(bIsReady() == true)
	{
		Ready();
	}
}

u_int32_t Network::BasicMessage::u32GetMessageID(void)
{
	return m_MessageID;
}
