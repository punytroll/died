#include "MessageFactory.h"

#include <iostream>

#include "Messages.h"

boost::shared_ptr< Network::BasicMessage > DiED::MessageFactory::GetMessage(u_int32_t u32Type)
{
	switch(u32Type)
	{
	case DiED::_HelloMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::HelloMessage());
			
			break;
		}
	case DiED::_NoMessage:
	default:
		{
			std::cerr << "Encountered undefined message '" << u32Type << "'." << std::endl;
			std::cerr << "  Trying to ignore." << std::endl;
			
			return boost::shared_ptr< Network::BasicMessage >(new DiED::NoMessage());
		}
	}
}
