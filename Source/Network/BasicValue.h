#ifndef BASICVALUE_H
#define BASICVALUE_H

#include "BasicReader.h"

namespace Network
{
	class BasicValue
	{
	public:
		BasicValue(void) :
			m_bReady(false)
		{
		}
		
		bool bIsReady(void)
		{
			return m_bReady;
		}
		
		virtual void vReadFrom(boost::shared_ptr< Network::BasicReader > Reader) = 0;
	protected:
		bool m_bReady;
	};
}

#endif
