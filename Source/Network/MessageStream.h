#ifndef MESSAGESTREAM_H
#define MESSAGESTREAM_H

#include <deque>

#include <boost/ref.hpp>
#include <boost/shared_ptr.hpp>

#include "Value.h"
#include "CircularBuffer.h"
#include "MessageFactory.h"
#include "Stream.h"

namespace Network
{
	class MessageStream : public Network::Stream
	{
	public:
		MessageStream(Network::MessageFactory & MessageFactory);
		MessageStream(int iSocket, Network::MessageFactory & MessageFactory);
		MessageStream & operator>>(Network::BasicValue & Value);
		MessageStream & operator>>(boost::shared_ptr< Network::BasicMessage > Message);
		MessageStream & operator<<(const Network::BasicMessage & Message);
	protected:
		void vRead(BasicValue & Value);
		virtual void vOnIn(void);
	private:
		// no copy and assignment for streams
		MessageStream(const MessageStream & MessageStream);
		MessageStream & operator=(const MessageStream & MessageStream);
		
		Network::CircularBuffer m_Buffer;
		Network::MessageFactory & m_MessageFactory;
		Value< u_int32_t > m_MessageType;
		std::deque< boost::reference_wrapper< Network::BasicValue > > m_Values;
		std::deque< boost::shared_ptr< Network::BasicMessage > > m_Messages;
	};
}

#endif
