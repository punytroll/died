#ifndef NETWORK_MESSAGESTREAM_H
#define NETWORK_MESSAGESTREAM_H

#include <deque>

#include "Value.h"
#include "MessageFactory.h"
#include "NotifyValue.h"
#include "Stream.h"

namespace Network
{
	class MessageStream : public Network::Stream
	{
	public:
		MessageStream(boost::shared_ptr< Network::MessageFactory > MessageFactory);
		MessageStream(int iSocket, boost::shared_ptr< Network::MessageFactory > MessageFactory);
		MessageStream & operator>>(boost::shared_ptr< Network::BasicMessage > Message);
		MessageStream & operator<<(const Network::BasicMessage & Message);
		
		boost::shared_ptr< Network::BasicMessage > rbegin(void);
		boost::shared_ptr< Network::BasicMessage > PopMessage(void);
	protected:
		virtual void vMessageReady(void) = 0;
	private:
		// no copy and assignment for streams
		MessageStream(const MessageStream & MessageStream);
		MessageStream & operator=(const MessageStream & MessageStream);
		
		void vMessageTypeReady(void);
		
		boost::shared_ptr< Network::MessageFactory > m_MessageFactory;
		Value< u_int32_t > m_MessageType;
		Network::NotifyValue m_NotifyValue;
		std::deque< boost::shared_ptr< Network::BasicMessage > > m_Messages;
	};
}

#endif
