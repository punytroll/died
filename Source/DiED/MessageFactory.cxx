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
	case DiED::_StatusConfirmMessage:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::StatusConfirmMessage());
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
	case DiED::_InsertTextEvent:
		{
			return boost::shared_ptr< Network::BasicMessage >(new DiED::InsertTextEvent());
		}
	case DiED::_NoMessage:
	default:
		{
			LOG(Warning, "DiED/MessageFactory", "Encountered undefined message '0x" << std::setw(8) << std::setfill('0') << std::hex << u32Type << std::dec << "'.");
			LOG(Warning, "DiED/MessageFactory", "  Trying to ignore.");
			
			return boost::shared_ptr< Network::BasicMessage >(new DiED::NoMessage());
		}
	}
}
