#ifndef DIED_BASICMASSAGE_H
#define DIED_BASICMASSAGE_H

#include <boost/any.hpp>

#include <Network/BasicMessage.h>

#include "MessageTarget.h"

namespace DiED
{
	typedef std::map< Glib::ustring, boost::any > ConfirmationParameters;
	
	class BasicMessage : public Network::BasicMessage
	{
	public:
		BasicMessage(const Network::BasicMessage::type_t & Type);
		virtual ~BasicMessage(void);
		virtual boost::shared_ptr< DiED::ConfirmationParameters > GetConfirmationParameters(void);
		virtual bool bIsConfirmedBy(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters);
		virtual void vExecute(DiED::MessageTarget & MessageTarget) = 0;
		virtual Glib::ustring sGetString(void) = 0;
		virtual bool bIsEventMessage(void);
		virtual bool bRequiresConfirmation(void);
		void vTriggerTimeout(DiED::MessageTarget * pMessageTarget, unsigned int uiMilliSeconds);
	private:
		sigc::connection m_TimeoutConnection;
		virtual bool bOnTimeout(DiED::MessageTarget * pMessageTarget);
	};
}

#endif
