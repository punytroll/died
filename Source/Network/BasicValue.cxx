#include "BasicValue.h"

void Network::BasicValue::vSetReady(bool bReady)
{
	m_bReady = bReady;
	if(bReady == true)
	{
		Ready();
	}
}
