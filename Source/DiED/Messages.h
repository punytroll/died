#ifndef MESSAGES_H
#define MESSAGES_H

#include <Network/BasicMessage.h>

namespace DiED
{
	enum
	{
		_NoMessage,
		_HelloMessage
	};
	
	class NoMessage : public Network::BasicMessage
	{
	public:
		NoMessage(void);
		virtual bool bIsReady(void);
		virtual void vReadFrom(Network::Stream & Stream);
	};
	
	class HelloMessage : public Network::BasicMessage
	{
	public:
		HelloMessage(void);
		virtual bool bIsReady(void);
		virtual void vReadFrom(Network::Stream & Stream);
	};
}

#endif
