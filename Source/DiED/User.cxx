#include "User.h"

#include <iostream>

DiED::User::User(void) :
	m_iLine(0),
	m_iCharacter(0),
	m_ClientID(0)
{
}

DiED::User::~User(void)
{
}

void DiED::User::vModifyCaretPosition(int iDeltaLine, int iDeltaCharacter)
{
	// if n lines where inserted before our position we have to go down n lines
	m_iLine += iDeltaLine;
	// 
	if(iDeltaLine == 0)
	{
		m_iCharacter += iDeltaCharacter;
	}
	else
	{
		m_iCharacter = iDeltaCharacter;
	}
}

int DiED::User::iGetLine(void) const
{
	return m_iLine;
}

int DiED::User::iGetCharacter(void) const
{
	return m_iCharacter;
}

void DiED::User::vSetClientID(DiED::clientid_t ClientID)
{
	m_ClientID = ClientID;
	ClientIDChanged();
}

DiED::clientid_t DiED::User::GetClientID(void)
{
	return m_ClientID;
}

void DiED::User::vSetStatus(const DiED::clientid_t & ClientID, DiED::User::Status Status)
{
	if(Status == Delete)
	{
		std::map< DiED::clientid_t, DiED::User::Status >::iterator iClient(m_Status.find(ClientID));
		
		if(iClient != m_Status.end())
		{
			m_Status.erase(iClient);
		}
	}
	else
	{
		m_Status[ClientID] = Status;
	}
}

DiED::User::Status DiED::User::GetStatus(const DiED::clientid_t & ClientID)
{
	std::map< DiED::clientid_t, DiED::User::Status >::iterator iClient(m_Status.find(ClientID));
	
	if(iClient != m_Status.end())
	{
		return iClient->second;
	}
	
	return Delete;
}

std::vector< DiED::clientid_t > DiED::User::GetConnectedClientIDs(void)
{
	std::vector< DiED::clientid_t > ClientIDs;
	std::map< DiED::clientid_t, DiED::User::Status >::iterator iClient(m_Status.begin());
	
	while(iClient != m_Status.end())
	{
//~ 		std::cout << "[DiED/User]: Status of " << iClient->first << ": " << iClient->second << std::endl;
		if(iClient->second == DiED::User::Connected)
		{
			ClientIDs.push_back(iClient->first);
		}
		++iClient;
	}
	
	return ClientIDs;
}

std::vector< DiED::clientid_t > DiED::User::GetDisconnectedClientIDs(void)
{
	std::vector< DiED::clientid_t > ClientIDs;
	std::map< DiED::clientid_t, DiED::User::Status >::iterator iClient(m_Status.begin());
	
	while(iClient != m_Status.end())
	{
//~ 		std::cout << "[DiED/User]: Status of " << iClient->first << ": " << iClient->second << std::endl;
		if(iClient->second == DiED::User::Disconnected)
		{
			ClientIDs.push_back(iClient->first);
		}
		++iClient;
	}
	
	return ClientIDs;
}
