#include "User.h"

#include <iostream>

#include <Common.h>

DiED::User::User(void) :
	m_iLine(0),
	m_iCharacter(0),
	m_ID(0)
{
	m_Status.insert(std::make_pair(m_ID, DiED::Local));
}

DiED::User::~User(void)
{
}

void DiED::User::vModifyCaretPosition(int iDeltaLine, int iDeltaCharacter)
{
	// avoid signal if we can
	if((iDeltaLine == 0) && (iDeltaCharacter == 0))
	{
		return;
	}
	m_iLine += iDeltaLine;
	m_iCharacter += iDeltaCharacter;
	CaretPositionChanged();
}

int DiED::User::iGetLine(void) const
{
	return m_iLine;
}

int DiED::User::iGetCharacter(void) const
{
	return m_iCharacter;
}

void DiED::User::vSetLine(int iLine)
{
	if(iLine == m_iLine)
	{
		return;
	}
	m_iLine = iLine;
	CaretPositionChanged();
}

void DiED::User::vSetCharacter(int iCharacter)
{
	if(iCharacter == m_iCharacter)
	{
		return;
	}
	m_iCharacter = iCharacter;
	CaretPositionChanged();
}

void DiED::User::vSetID(const DiED::clientid_t & ID)
{
	m_Status.erase(m_Status.find(m_ID));
	m_ID = ID;
	m_Status.insert(std::make_pair(m_ID, DiED::Local));
}

DiED::clientid_t DiED::User::GetID(void) const
{
	return m_ID;
}

void DiED::User::vSetStatus(const DiED::clientid_t & ClientID, const DiED::clientstatus_t & Status)
{
	LOG(Verbose, "DiED/User", "Client " << GetID() << " is setting client " << ClientID << " to " << sStatusToString(Status));
	
	std::map< DiED::clientid_t, DiED::clientstatus_t >::iterator iClient(m_Status.find(ClientID));
	
	if(Status == DiED::Undefined)
	{
		if(iClient != m_Status.end())
		{
			m_Status.erase(iClient);
		}
		else
		{
			return;
		}
	}
	else
	{
		if(iClient == m_Status.end())
		{
			m_Status.insert(std::make_pair(ClientID, Status));
			if(Status == Disconnected)
			{
				return;
			}
		}
		else
		{
			if(iClient->second != Status)
			{
				iClient->second = Status;
			}
			else
			{
				return;
			}
		}
	}
	StatusChanged(ClientID, Status);
}

DiED::clientstatus_t DiED::User::GetStatus(const DiED::clientid_t & ClientID)
{
	std::map< DiED::clientid_t, DiED::clientstatus_t >::iterator iClient(m_Status.find(ClientID));
	
	if(iClient != m_Status.end())
	{
		return iClient->second;
	}
	
	return DiED::Undefined;
}

std::set< DiED::clientid_t > DiED::User::GetConnectedClientIDs(void)
{
	std::set< DiED::clientid_t > ClientIDs;
	std::map< DiED::clientid_t, DiED::clientstatus_t >::iterator iClient(m_Status.begin());
	
	while(iClient != m_Status.end())
	{
		LOG(Verbose, "DiED/User", "Status of " << iClient->first << ": " << sStatusToString(iClient->second));
		if(iClient->second == DiED::Connected)
		{
			ClientIDs.insert(iClient->first);
		}
		++iClient;
	}
	
	return ClientIDs;
}

std::set< DiED::clientid_t > DiED::User::GetDisconnectedClientIDs(void)
{
	std::set< DiED::clientid_t > ClientIDs;
	std::map< DiED::clientid_t, DiED::clientstatus_t >::iterator iClient(m_Status.begin());
	
	while(iClient != m_Status.end())
	{
		LOG(Verbose, "DiED/User", "Status of " << iClient->first << ": " << iClient->second);
		if(iClient->second == DiED::Disconnected)
		{
			ClientIDs.insert(iClient->first);
		}
		++iClient;
	}
	
	return ClientIDs;
}
