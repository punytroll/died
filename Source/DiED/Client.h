#ifndef DIED_CLIENT_H
#define DIED_CLIENT_H

#include <Network/MessageStream.h>

#include "InternalEnvironment.h"

namespace DiED
{
	class InternalEnvironment;
	
	class Client : public Network::MessageStream
	{
	public:
		Client(boost::shared_ptr< Network::MessageFactory > MessageFactory, DiED::InternalEnvironment & InternalEnvironment);
		Client(int iSocket, boost::shared_ptr< Network::MessageFactory > MessageFactory, DiED::InternalEnvironment & InternalEnvironment);
		void vInsertText(const Glib::ustring & sString);
		void vModifyCaretPosition(int iDeltaLine, int iDeltaCharacter);
		int iGetLine(void) const
		{
			return m_iLine;
		}
		
		int iGetCharacter(void) const
		{
			return m_iCharacter;
		}
	protected:
		virtual void vMessageReady(void);
		virtual void vExecuteTopMessage(void);
	private:
		DiED::InternalEnvironment & m_InternalEnvironment;
		int m_iLine;
		int m_iCharacter;
	};
}

#endif
