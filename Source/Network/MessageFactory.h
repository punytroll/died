#ifndef MESSAGEFACTORY_H
#define MESSAGEFACTORY_H

#include <boost/shared_ptr.hpp>

#include "BasicMessage.h"

namespace Network
{
	class MessageFactory
	{
	public:
		virtual boost::shared_ptr< Network::BasicMessage > GetMessage(u_int32_t u32Type) = 0;
	};
}

#endif
