#include "EventAction.h"

#include "Messages.h"

DiED::EventAction::~EventAction(void)
{
}

DiED::InsertAction::~InsertAction(void)
{
}

DiED::InsertAction::InsertAction(const Glib::ustring & sText) :
	m_sText(sText)
{
}

void DiED::InsertAction::vExecute(DiED::ActionTarget & ActionTarget)
{
	ActionTarget.vHandleInsert(m_sText);
}

boost::shared_ptr< DiED::BasicMessage > DiED::InsertAction::GetMessage(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID)
{
	return boost::shared_ptr< DiED::BasicMessage >(new DiED::InsertEvent(CreatorID, EventID, LostClientID, m_sText));
}

DiED::DeleteAction::DeleteAction(int iLineRelative, int iCharacterRelative, int iLineAbsolute, int iCharacterAbsolute) :
	m_iLineRelative(iLineRelative),
	m_iCharacterRelative(iCharacterRelative),
	m_iLineAbsolute(iLineAbsolute),
	m_iCharacterAbsolute(iCharacterAbsolute)
{
}

DiED::DeleteAction::~DeleteAction(void)
{
}

void DiED::DeleteAction::vExecute(DiED::ActionTarget & ActionTarget)
{
	ActionTarget.vHandleDelete(m_iLineRelative, m_iCharacterRelative, m_iLineAbsolute, m_iCharacterAbsolute);
}

boost::shared_ptr< DiED::BasicMessage > DiED::DeleteAction::GetMessage(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID)
{
	return boost::shared_ptr< DiED::BasicMessage >(new DiED::DeleteEvent(CreatorID, EventID, LostClientID, m_iLineRelative, m_iCharacterRelative, m_iLineAbsolute, m_iCharacterAbsolute));
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
