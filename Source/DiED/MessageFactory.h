#ifndef DIED_MESSAGEFACTORY_H
#define DIED_MESSAGEFACTORY_H

#include <Network/MessageFactory.h>

namespace DiED
{
	class MessageFactory : public Network::MessageFactory
	{
	public:
		virtual boost::shared_ptr< Network::BasicMessage > GetMessage(u_int32_t u32Type);
	};
}

#endif
