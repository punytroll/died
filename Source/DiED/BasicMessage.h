#ifndef DIED_BASICMASSAGE_H
#define DIED_BASICMASSAGE_H

#include <Network/BasicMessage.h>
#include <DiED/Client.h>

namespace DiED
{
	class BasicMessage : public Network::BasicMessage
	{
	public:
		BasicMessage(const Network::BasicMessage::type_t & Type, bool bForSending);
		virtual void vExecute(DiED::Client & Client) = 0;
		virtual Glib::ustring sGetString(void) = 0;
	};
}

#endif
