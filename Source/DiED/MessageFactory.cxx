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
		}
	case DiED::_ConnectionAcceptMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::ConnectionAcceptMessage());
		}
	case DiED::_KnownClientsMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::KnownClientsMessage());
		}
	case DiED::_InputMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::InputMessage());
		}
	case DiED::_PingMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::PingMessage());
		}
	case DiED::_PongMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::PongMessage());
		}
	case DiED::_ConnectionEstablishedMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::ConnectionEstablishedMessage());
		}
	case DiED::_ConnectionLostMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::ConnectionLostMessage());
		}
	case DiED::_TestMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::TestMessage());
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
