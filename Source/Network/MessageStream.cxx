#include "MessageStream.h"

#include <iostream>

Network::MessageStream::MessageStream(boost::shared_ptr< Network::MessageFactory > MessageFactory) :
	m_MessageFactory(MessageFactory)
{
	Network::Stream & Stream(*this);
	
	Stream >> m_MessageType;
	m_MessageType.Ready.connect(sigc::mem_fun(*this, &Network::MessageStream::vMessageTypeReady));
	m_NotifyValue.Ready.connect(sigc::mem_fun(*this, &Network::MessageStream::vMessageReady));
}

Network::MessageStream::MessageStream(int iSocket, boost::shared_ptr< Network::MessageFactory > MessageFactory) :
	Stream(iSocket),
	m_MessageFactory(MessageFactory)
{
	Network::Stream & Stream(*this);
	
	Stream >> m_MessageType;
	m_MessageType.Ready.connect(sigc::mem_fun(*this, &Network::MessageStream::vMessageTypeReady));
	m_NotifyValue.Ready.connect(sigc::mem_fun(*this, &Network::MessageStream::vMessageReady));
}

Network::MessageStream & Network::MessageStream::operator>>(boost::shared_ptr< Network::BasicMessage > Message)
{
	push_back(Message);
	MessageBegin();
	Message->vReadFrom(*this);
	
	return *this;
}

Network::MessageStream & Network::MessageStream::operator<<(const Network::BasicMessage & Message)
{
	if(Message.bIsForSending() == true)
	{
		Message.vWriteTo(*this);
//~ 		std::cout << "[Network/MessageStream]: Requesting OnOut." << std::endl;
		vRequestOnOut();
	}
	else
	{
		std::cout << "[Network/MessageStream]: Trying to send Message [" << Message.GetType() << "] which is not for sending." << std::endl;
	}
	
	return *this;
}

void Network::MessageStream::vMessageTypeReady(void)
{
	*this >> m_MessageFactory->GetMessage(m_MessageType);
	Stream::operator>>(m_NotifyValue);
	Stream::operator>>(m_MessageType);
}

void Network::MessageStream::vMessageReady(void)
{
	if(size() > 0)
	{
		MessageReady();
	}
}
