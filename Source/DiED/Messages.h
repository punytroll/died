#ifndef MESSAGES_H
#define MESSAGES_H

#include <Network/BasicMessage.h>

namespace DiED
{
	enum
	{
		_NoMessage,
		_HelloMessage = 0xB0A0A0A0
	};
	
	class NoMessage : public Network::BasicMessage
	{
	public:
		NoMessage(void);
		virtual bool bIsReady(void);
		virtual void vReadFrom(Network::Stream & Stream);
		virtual void vWriteToInternal(Network::Stream & Stream) const;
	};
	
	class HelloMessage : public Network::BasicMessage
	{
	public:
		HelloMessage(void);
		virtual bool bIsReady(void);
		virtual void vReadFrom(Network::Stream & Stream);
		virtual void vWriteToInternal(Network::Stream & Stream) const;
	};
}

#endif
