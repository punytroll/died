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

void Network::BasicMessage::vRegisterValue(Network::BasicValue & Value)
{
	m_Values.push_back(boost::ref(Value));
}

void Network::BasicMessage::vReadFrom(Network::Stream & Stream)
{
	std::vector< boost::reference_wrapper< Network::BasicValue > >::iterator iValue(m_Values.begin());
	
	while(iValue != m_Values.end())
	{
		Stream >> *iValue;
		++iValue;
	}
}

void Network::BasicMessage::vWriteTo(Network::Stream & Stream) const
{
	Stream << m_Type;
	
	std::vector< boost::reference_wrapper< Network::BasicValue > >::const_iterator iValue(m_Values.begin());
	
	while(iValue != m_Values.end())
	{
		Stream << *iValue;
		++iValue;
	}
}

size_t Network::BasicMessage::stGetSize(void) const
{
	std::vector< boost::reference_wrapper< Network::BasicValue > >::const_iterator iValue(m_Values.begin());
	size_t stSize = 0;
	
	while(iValue != m_Values.end())
	{
		stSize += iValue->get().stGetSize();
		++iValue;
	}
	
	return stSize;
}

Network::BasicMessage::type_t Network::BasicMessage::GetType(void) const
{
	return m_Type;
}
