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

DiED::PositionAction::PositionAction(int iLineRelative, int iCharacterRelative, int iLineAbsolute, int iCharacterAbsolute) :
	m_iLineRelative(iLineRelative),
	m_iCharacterRelative(iCharacterRelative),
	m_iLineAbsolute(iLineAbsolute),
	m_iCharacterAbsolute(iCharacterAbsolute)
{
}

DiED::PositionAction::~PositionAction(void)
{
}

void DiED::PositionAction::vExecute(DiED::ActionTarget & ActionTarget)
{
	ActionTarget.vHandlePosition(m_iLineRelative, m_iCharacterRelative, m_iLineAbsolute, m_iCharacterAbsolute);
}

boost::shared_ptr< DiED::BasicMessage > DiED::PositionAction::GetMessage(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID)
{
	return boost::shared_ptr< DiED::BasicMessage >(new DiED::PositionEvent(CreatorID, EventID, LostClientID, m_iLineRelative, m_iCharacterRelative, m_iLineAbsolute, m_iCharacterAbsolute));
}
