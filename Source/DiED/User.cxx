#include "User.h"

#include <iostream>

#include <Common.h>

DiED::User::User(void) :
	m_iLine(0),
	m_iCharacter(0),
	m_ID(0)
{
}

DiED::User::~User(void)
{
}

void DiED::User::vModifyCaretPosition(int iDeltaLine, int iDeltaCharacter)
{
	// avoid all the fuss if we can
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

void DiED::User::vSetID(DiED::clientid_t ID)
{
	m_ID = ID;
}

DiED::clientid_t DiED::User::GetID(void)
{
	return m_ID;
}

void DiED::User::vSetStatus(const DiED::clientid_t & ClientID, const DiED::clientstatus_t & Status)
{
//~ 	std::cout << "[DiED/User]: Client " << GetID() << " is setting client " << ClientID << " to " << sStatusToString(Status) << std::endl;
	std::map< DiED::clientid_t, DiED::clientstatus_t >::iterator iClient(m_Status.find(ClientID));
	
	if(Status == DiED::Deleted)
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
	
	return DiED::Deleted;
}

std::set< DiED::clientid_t > DiED::User::GetConnectedClientIDs(void)
{
	std::set< DiED::clientid_t > ClientIDs;
	std::map< DiED::clientid_t, DiED::clientstatus_t >::iterator iClient(m_Status.begin());
	
	while(iClient != m_Status.end())
	{
//~ 		std::cout << "[DiED/User]: Status of " << iClient->first << ": " << sStatusToString(iClient->second) << std::endl;
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
//~ 		std::cout << "[DiED/User]: Status of " << iClient->first << ": " << iClient->second << std::endl;
		if(iClient->second == DiED::Disconnected)
		{
			ClientIDs.insert(iClient->first);
		}
		++iClient;
	}
	
	return ClientIDs;
}
