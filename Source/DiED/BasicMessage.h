#ifndef DIED_BASICMASSAGE_H
#define DIED_BASICMASSAGE_H

#include <Network/BasicMessage.h>

#include "Common.h"
#include "MessageTarget.h"

namespace DiED
{
	class BasicMessage : public sigc::trackable, public Network::BasicMessage
	{
	public:
		BasicMessage(const Network::BasicMessage::type_t & Type);
		virtual ~BasicMessage(void);
		virtual boost::shared_ptr< DiED::ConfirmationParameters > GetConfirmationParameters(void);
		virtual bool bIsConfirmedBy(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters);
		virtual void vExecute(DiED::MessageTarget & MessageTarget) = 0;
		virtual Glib::ustring sGetString(void) = 0;
		virtual bool bIsEvent(void);
		virtual bool bRequiresConfirmation(void);
		virtual boost::shared_ptr< DiED::BasicMessage > Clone(void);
	
		void vInitiateTimeout(DiED::MessageTarget * pMessageTarget, unsigned int uiMilliSeconds);
		void vDisconnectTimeout(void);
	private:
		sigc::connection m_TimeoutConnection;
		virtual bool bOnTimeout(DiED::MessageTarget * pMessageTarget);
	};
}

#endif
