#ifndef MESSAGE_H
#define MESSAGE_H

#include "Stream.h"
#include "Value.h"

namespace Network
{
	class BasicMessage
	{
	public:
		BasicMessage(u_int32_t u32MessageID, bool bForSending);
		bool bIsForSending(void) const;
		virtual bool bIsReady(void) = 0;
		virtual void vReadFrom(Network::Stream & Stream) = 0;
		void vWriteTo(Network::Stream & Stream) const;
	protected:
		virtual void vWriteToInternal(Network::Stream & Stream) const = 0;
	private:
		Value< u_int32_t > m_MessageID;
		bool m_bForSending;
	};
}

#endif
