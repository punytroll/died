#ifndef MESSAGE_H
#define MESSAGE_H

#include "Stream.h"
#include "Value.h"

namespace Network
{
	class BasicMessage
	{
	public:
		typedef u_int32_t type_t;
		
		// constructor and destructor
		BasicMessage(Network::BasicMessage::type_t Type, bool bForSending);
		virtual ~BasicMessage(void);
		
		// query functions
		bool bIsForSending(void) const;
		virtual bool bIsReady(void) const = 0;
		Network::BasicMessage::type_t GetType(void) const;
		
		// stream operations
		virtual void vReadFrom(Network::Stream & Stream) = 0;
		void vWriteTo(Network::Stream & Stream) const;
	protected:
		// overwrite this function instead of vWriteTo
		virtual void vWriteToInternal(Network::Stream & Stream) const = 0;
	private:
		Network::Value< Network::BasicMessage::type_t > m_Type;
		bool m_bForSending;
	};
}

#endif
