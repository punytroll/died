#include "BasicValue.h"

Network::BasicValue::BasicValue(void) :
	m_bReady(false)
{
}

Network::BasicValue::~BasicValue(void)
{
}

bool Network::BasicValue::bIsReady(void) const
{
	return m_bReady;
}

size_t Network::BasicValue::stGetSize(void) const
{
	return 0;
}

void Network::BasicValue::vSetReady(bool bReady)
{
	m_bReady = bReady;
	if(bReady == true)
	{
		Ready();
	}
}
