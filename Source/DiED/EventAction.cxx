#include "EventAction.h"

#include "Messages.h"

DiED::EventAction::~EventAction(void)
{
}

DiED::InsertTextAction::~InsertTextAction(void)
{
}

DiED::InsertTextAction::InsertTextAction(const Glib::ustring & sText) :
	m_sText(sText)
{
}

void DiED::InsertTextAction::vExecute(DiED::ActionTarget & ActionTarget)
{
	ActionTarget.vHandleInsertText(m_sText);
}

boost::shared_ptr< DiED::BasicMessage > DiED::InsertTextAction::GetMessage(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID)
{
	return boost::shared_ptr< DiED::BasicMessage >(new DiED::InsertTextEvent(CreatorID, EventID, LostClientID, m_sText));
}
