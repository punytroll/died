#ifndef MESSAGE_H
#define MESSAGE_H

#include "Stream.h"
#include "Value.h"

namespace Network
{
	class BasicMessage
	{
	public:
		BasicMessage(u_int32_t u32Type, bool bForSending);
		virtual ~BasicMessage(void);
		bool bIsForSending(void) const;
		virtual bool bIsReady(void) = 0;
		virtual void vReadFrom(Network::Stream & Stream) = 0;
		void vWriteTo(Network::Stream & Stream) const;
		sigc::signal< void > Ready;
		u_int32_t u32GetType(void);
	protected:
		virtual void vWriteToInternal(Network::Stream & Stream) const = 0;
		void vRegisterValue(Network::BasicValue & Value);
	private:
		void vNotifyValueReady(void);
		Value< u_int32_t > m_Type;
		bool m_bForSending;
	};
}

#endif
