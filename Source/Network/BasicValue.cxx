#include "BasicValue.h"

std::ostream & Network::BasicValue::operator<<(std::ostream & OStream)
{
	return OStream << "<Unknown Type>";
}

void Network::BasicValue::vSetReady(bool bReady)
{
	m_bReady = bReady;
	if(bReady == true)
	{
		Ready();
	}
}
