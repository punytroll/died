#include "BasicValue.h"

void Network::BasicValue::vSetReady(bool bReady)
{
	m_bReady = bReady;
	Ready();
}
