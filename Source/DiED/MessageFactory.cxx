#include "MessageFactory.h"

#include <iostream>

#include "Messages.h"

boost::shared_ptr< Network::BasicMessage > DiED::MessageFactory::GetMessage(u_int32_t u32Type)
{
	switch(u32Type)
	{
	case DiED::_ConnectionRequestMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::ConnectionRequestMessage());
			
			break;
		}
	case DiED::_ConnectionAcceptMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::ConnectionAcceptMessage());
			
			break;
		}
	case DiED::_InputMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::InputMessage());
			
			break;
		}
	case DiED::_PingMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::PingMessage());
			
			break;
		}
	case DiED::_PongMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::PongMessage());
			
			break;
		}
	case DiED::_ConnectionEstablishedMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::ConnectionEstablishedMessage());
			
			break;
		}
	case DiED::_ConnectionLostMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::ConnectionLostMessage());
			
			break;
		}
	case DiED::_TestMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::TestMessage());
			
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
