#include "MessageStream.h"

#include <iostream>

void vEchoReady(boost::shared_ptr< Network::BasicMessage > Message)
{
	std::cout << "Retrieved Message with ID " << Message->u32GetMessageID() << std::endl;
}

Network::MessageStream::MessageStream(MessageFactory & MessageFactory) :
	m_MessageFactory(MessageFactory)
{
	Network::Stream & Stream(*this);
	
	Stream >> m_MessageType;
}

Network::MessageStream::MessageStream(int iSocket, MessageFactory & MessageFactory) :
	Stream(iSocket),
	m_MessageFactory(MessageFactory)
{
	Network::Stream & Stream(*this);
	
	Stream >> m_MessageType;
}

Network::MessageStream & Network::MessageStream::operator>>(boost::shared_ptr< Network::BasicMessage > Message)
{
	m_Messages.push_back(Message);
	Message->Ready.connect(sigc::bind(sigc::ptr_fun(vEchoReady), Message));
	Message->vReadFrom(*this);
	
	return *this;
}

Network::MessageStream & Network::MessageStream::operator<<(const Network::BasicMessage & Message)
{
	if(Message.bIsForSending() == true)
	{
		Message.vWriteTo(*this);
		vRequestOnOut();
	}
	
	return *this;
}

void Network::MessageStream::vOnIn(void)
{
	Stream::vOnIn();
	if(m_MessageType.bIsReady() == true)
	{
		*this >> m_MessageFactory.GetMessage(m_MessageType);
		Stream::operator>>(m_MessageType);
	}
}
