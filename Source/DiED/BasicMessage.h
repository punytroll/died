#ifndef DIED_BASICMASSAGE_H
#define DIED_BASICMASSAGE_H

#include <Network/BasicMessage.h>
#include <DiED/Client.h>

namespace DiED
{
	class BasicMessage : public Network::BasicMessage
	{
	public:
		BasicMessage(u_int32_t u32Type, bool bForSending);
		virtual void vExecute(DiED::Client & Client) = 0;
		virtual Glib::ustring sGetString(void) = 0;
	};
}

#endif
