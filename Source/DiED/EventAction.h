#ifndef DIED_EVENTACTION_H
#define DIED_EVENTACTION_H

#include <glibmm/ustring.h>

#include "ActionTarget.h"
#include "BasicMessage.h"

namespace DiED
{
	class EventAction
	{
	public:
		virtual ~EventAction(void);
		virtual void vExecute(DiED::ActionTarget & ActionTarget) = 0;
		virtual boost::shared_ptr< DiED::BasicMessage > GetMessage(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID) = 0;
	};
	
	class InsertTextAction : public DiED::EventAction
	{
	public:
		InsertTextAction(const Glib::ustring & sText);
		virtual ~InsertTextAction(void);
		virtual void vExecute(DiED::ActionTarget & ActionTarget);
		virtual boost::shared_ptr< DiED::BasicMessage > GetMessage(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID);
	private:
		Glib::ustring m_sText;
	};
}

#endif
