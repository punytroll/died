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
	case DiED::_ClientsRegisteredMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::ClientsRegisteredMessage());
		}
	case DiED::_ConnectionEstablishedMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::ConnectionEstablishedMessage());
		}
	case DiED::_ConnectionLostMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::ConnectionLostMessage());
		}
	case DiED::_PingMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::PingMessage());
		}
	case DiED::_PongMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::PongMessage());
		}
	case DiED::_EventReceivedMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::EventReceivedMessage());
		}
	case DiED::_InputMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::InputMessage());
		}
	case DiED::_NoMessage:
	default:
		{
			std::cout << "Encountered undefined message '" << std::hex << u32Type << std::dec << "'." << std::endl;
			std::cout << "  Trying to ignore." << std::endl;
			
			return boost::shared_ptr< Network::BasicMessage >(new DiED::NoMessage());
		}
	}
}
