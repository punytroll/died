#ifndef NETWORK_MESSAGESTREAM_H
#define NETWORK_MESSAGESTREAM_H

#include <deque>

#include "Value.h"
#include "MessageFactory.h"
#include "NotifyValue.h"
#include "Stream.h"

namespace Network
{
	class MessageStream : public Network::Stream, public std::deque< boost::shared_ptr< Network::BasicMessage > >
	{
	public:
		typedef std::deque< boost::shared_ptr< Network::BasicMessage > > iterator;
		
		MessageStream(boost::shared_ptr< Network::MessageFactory > MessageFactory);
		MessageStream(int iSocket, boost::shared_ptr< Network::MessageFactory > MessageFactory);
		MessageStream & operator>>(boost::shared_ptr< Network::BasicMessage > Message);
		MessageStream & operator<<(const Network::BasicMessage & Message);
		sigc::signal< void > MessageBegin;
		sigc::signal< void > MessageReady;
	private:
		// no copy and assignment for streams
		MessageStream(const MessageStream & MessageStream);
		MessageStream & operator=(const MessageStream & MessageStream);
		
		void vMessageTypeReady(void);
		void vMessageReady(void);
		
		virtual void vBytesSent(size_t stSize);
		
		boost::shared_ptr< Network::MessageFactory > m_MessageFactory;
		Network::Value< u_int32_t > m_MessageType;
		Network::NotifyValue m_NotifyValue;
	};
}

#endif
