#include "MessageStream.h"

Network::MessageStream::MessageStream(MessageFactory & MessageFactory) :
	m_Buffer(2048),
	m_MessageFactory(MessageFactory)
{
	*this >> m_MessageType;
}

Network::MessageStream::MessageStream(int iSocket, MessageFactory & MessageFactory) :
	Stream(iSocket),
	m_Buffer(2048),
	m_MessageFactory(MessageFactory)
{
	*this >> m_MessageType;
}

void Network::MessageStream::vOnIn(void)
{
	// read from socket into buffer
	
	std::deque< boost::reference_wrapper< Network::BasicValue > >::iterator iValue(m_Values.begin());
	
	while(iValue != m_Values.end())
	{
		Network::BasicValue & Value(*iValue);
		
		vRead(Value);
		if(Value.bIsReady() == false)
		{
			break;
		}
		m_Values.erase(iValue);
		iValue = m_Values.begin();
	}
	if(m_MessageType.bIsReady() == true)
	{
		*this >> m_MessageFactory.GetMessage(m_MessageType) >> m_MessageType;
	}
}

Network::MessageStream & Network::MessageStream::operator>>(Network::BasicValue & Value)
{
	m_Values.push_back(boost::ref(Value));
	
	return *this;
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
//~ 		Message.vWriteTo();
// TODO: write to output buffer (circular)
	}
	
	return *this;
}

void Network::MessageStream::vRead(Network::BasicValue & Value)
{
	Value.vReadFrom(m_Buffer.GetReader());
}
