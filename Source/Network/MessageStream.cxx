#include "MessageStream.h"

#include <iostream>

Network::MessageStream::MessageStream(MessageFactory & MessageFactory) :
	m_MessageFactory(MessageFactory)
{
	Network::Stream & Stream(*this);
	
	Stream >> m_MessageType;
	m_MessageType.Ready.connect(sigc::mem_fun(*this, &Network::MessageStream::vMessageTypeReady));
	m_NotifyValue.Ready.connect(sigc::mem_fun(*this, &Network::MessageStream::vMessageReady));
}

Network::MessageStream::MessageStream(int iSocket, MessageFactory & MessageFactory) :
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
	m_Messages.push_back(Message);
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

void Network::MessageStream::vMessageTypeReady(void)
{
	*this >> m_MessageFactory.GetMessage(m_MessageType);
	Stream::operator>>(m_NotifyValue);
	Stream::operator>>(m_MessageType);
}

boost::shared_ptr< Network::BasicMessage > Network::MessageStream::PopMessage(void)
{
	std::deque< boost::shared_ptr< Network::BasicMessage > >::iterator iBegin(m_Messages.begin());
	boost::shared_ptr< Network::BasicMessage > Message(*iBegin);
	
	m_Messages.erase(iBegin);
	
	return Message;
}
