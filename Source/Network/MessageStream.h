#ifndef MESSAGESTREAM_H
#define MESSAGESTREAM_H

#include <deque>

#include "Value.h"
#include "MessageFactory.h"
#include "Stream.h"

namespace Network
{
	class MessageStream : public Network::Stream
	{
	public:
		MessageStream(Network::MessageFactory & MessageFactory);
		MessageStream(int iSocket, Network::MessageFactory & MessageFactory);
		MessageStream & operator>>(boost::shared_ptr< Network::BasicMessage > Message);
		MessageStream & operator<<(const Network::BasicMessage & Message);
	protected:
		virtual void vOnIn(void);
	private:
		// no copy and assignment for streams
		MessageStream(const MessageStream & MessageStream);
		MessageStream & operator=(const MessageStream & MessageStream);
		
		Network::MessageFactory & m_MessageFactory;
		Value< u_int32_t > m_MessageType;
		std::deque< boost::shared_ptr< Network::BasicMessage > > m_Messages;
	};
}

#endif
