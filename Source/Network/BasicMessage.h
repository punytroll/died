#ifndef NETWORK_MESSAGE_H
#define NETWORK_MESSAGE_H

#include "Stream.h"
#include "Value.h"

namespace Network
{
	class BasicMessage
	{
	public:
		typedef u_int32_t type_t;
		
		// constructor and destructor
		BasicMessage(Network::BasicMessage::type_t Type);
		virtual ~BasicMessage(void);
		
		// query functions
		Network::BasicMessage::type_t GetType(void) const;
		size_t stGetSize(void) const;
		
		// stream operations
		void vReadFrom(Network::Stream & Stream);
		void vWriteTo(Network::Stream & Stream) const;
	protected:
		void vRegisterValue(Network::BasicValue & BasicValue);
	private:
		Network::Value< Network::BasicMessage::type_t > m_Type;
		std::vector< boost::reference_wrapper< Network::BasicValue > > m_Values;
	};
}

#endif
