#ifndef NETWORK_NOTIFYVALUE_H
#define NETWORK_NOTIFYVALUE_H

#include <iostream>

#include "BasicValue.h"

namespace Network
{
	class NotifyValue : public Network::BasicValue
	{
	public:
		NotifyValue(void)
		{
		}
		
		virtual ~NotifyValue(void)
		{
		}
		
		virtual void vReadFrom(boost::shared_ptr< Network::BasicReader > Reader)
		{
			vSetReady(true);
		}
		
		virtual void vWriteTo(boost::shared_ptr< Network::BasicWriter > Writer) const
		{
		}
	};
}

#endif
