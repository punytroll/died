#include "BasicMessage.h"

#include <iostream>

Network::BasicMessage::BasicMessage(u_int32_t u32Type) :
	m_Type(u32Type)
{
//~ 	std::cout << "[Network/BasicMessage]: Created BasicMessage [Type = " << u32Type << "]" << std::endl;
}

Network::BasicMessage::~BasicMessage(void)
{
//~ 	std::cout << "[Network/BasicMessage]: Deleted BasicMessage." << std::endl;
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
