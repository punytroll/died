#ifndef MESSAGE_H
#define MESSAGE_H

#include "Stream.h"

namespace Network
{
	class BasicMessage
	{
	public:
		BasicMessage(bool bForSending);
		bool bIsForSending(void) const;
		virtual bool bIsReady(void) = 0;
		virtual void vReadFrom(Network::Stream & Stream) = 0;
	private:
		bool m_bForSending;
	};
}

#endif
