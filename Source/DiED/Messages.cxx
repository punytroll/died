#include "Messages.h"

#include <iostream>
#include <sstream>


///////////////////////////////////////////////////////////////////////////////////////////////////
///                                          NoMessage                                          ///
///////////////////////////////////////////////////////////////////////////////////////////////////

DiED::NoMessage::NoMessage(void) :
	DiED::BasicMessage(DiED::_NoMessage)
{
}

void DiED::NoMessage::vExecute(DiED::MessageTarget & MessageTarget)
{
}

Glib::ustring DiED::NoMessage::sGetString(void)
{
	return "NoMessage []";
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///                                  ConnectionRequestMessage                                   ///
///////////////////////////////////////////////////////////////////////////////////////////////////

DiED::ConnectionRequestMessage::ConnectionRequestMessage(void) :
	DiED::BasicMessage(DiED::_ConnectionRequestMessage)
{
	vRegisterValue(m_ClientID);
	vRegisterValue(m_Port);
}

DiED::ConnectionRequestMessage::ConnectionRequestMessage(const DiED::clientid_t & ClientID, const Network::port_t & Port) :
	DiED::BasicMessage(DiED::_ConnectionRequestMessage),
	m_ClientID(ClientID),
	m_Port(Port)
{
	vRegisterValue(m_ClientID);
	vRegisterValue(m_Port);
}

void DiED::ConnectionRequestMessage::vExecute(DiED::MessageTarget & MessageTarget)
{
	MessageTarget.vHandleConnectionRequest(m_ClientID, m_Port);
}

Glib::ustring DiED::ConnectionRequestMessage::sGetString(void)
{
	std::stringstream ssString;
	
	ssString << "ConnectionRequest [ClientID = " << m_ClientID << " ;  Port = " << m_Port << "]";
	
	return ssString.str();
}

bool DiED::ConnectionRequestMessage::bRequiresConfirmation(void)
{
	return true;
}

bool DiED::ConnectionRequestMessage::bIsConfirmedBy(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters)
{
	DiED::ConfirmationParameters::iterator iParameter(ConfirmationParameters->find("Type"));
	
	if((iParameter == ConfirmationParameters->end()) || (boost::any_cast< Network::BasicMessage::type_t >(iParameter->second) != DiED::_ConnectionAcceptMessage))
	{
		return false;
	}
	
	return true;
}

bool DiED::ConnectionRequestMessage::bOnTimeout(DiED::MessageTarget * pMessageTarget)
{
	if(pMessageTarget != 0)
	{
		boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters(new DiED::ConfirmationParameters());
		
		ConfirmationParameters->insert(std::make_pair("Type", boost::any(static_cast< Network::BasicMessage::type_t >(DiED::_ConnectionAcceptMessage))));
		
		pMessageTarget->vHandleConnectionRequestConfirmationTimeout(ConfirmationParameters);
	}
	
	return false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///                                   ConnectionAcceptMessage                                   ///
///////////////////////////////////////////////////////////////////////////////////////////////////

DiED::ConnectionAcceptMessage::ConnectionAcceptMessage(void) :
	DiED::BasicMessage(DiED::_ConnectionAcceptMessage)
{
	vRegisterValue(m_AccepterClientID);
	vRegisterValue(m_RequesterClientID);
}

DiED::ConnectionAcceptMessage::ConnectionAcceptMessage(const DiED::clientid_t & AccepterClientID, const DiED::clientid_t & RequesterClientID) :
	DiED::BasicMessage(DiED::_ConnectionAcceptMessage),
	m_AccepterClientID(AccepterClientID),
	m_RequesterClientID(RequesterClientID)
{
	vRegisterValue(m_AccepterClientID);
	vRegisterValue(m_RequesterClientID);
}

void DiED::ConnectionAcceptMessage::vExecute(DiED::MessageTarget & MessageTarget)
{
	MessageTarget.vHandleConnectionAccept(m_AccepterClientID, m_RequesterClientID);
}

boost::shared_ptr< DiED::ConfirmationParameters > DiED::ConnectionAcceptMessage::GetConfirmationParameters(void)
{
	boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters(new DiED::ConfirmationParameters());
	
	ConfirmationParameters->insert(std::make_pair("Type", boost::any(static_cast< Network::BasicMessage::type_t >(DiED::_ConnectionAcceptMessage))));
	
	return ConfirmationParameters;
}

Glib::ustring DiED::ConnectionAcceptMessage::sGetString(void)
{
	std::stringstream ssString;
	
	ssString << "ConnectionAccept [AccepterClientID = " << m_AccepterClientID << " ; RequesterClientID = " << m_RequesterClientID << "]";
	
	return ssString.str();
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///                                     KnownClientsMessage                                     ///
///////////////////////////////////////////////////////////////////////////////////////////////////

DiED::KnownClientsMessage::KnownClientsMessage(void) :
	DiED::BasicMessage(DiED::_KnownClientsMessage)
{
	vRegisterValue(m_MessageID);
	vRegisterValue(m_ClientInfos);
}

DiED::KnownClientsMessage::KnownClientsMessage(DiED::messageid_t MessageID, std::vector< DiED::ClientInfo > ClientInfos) :
	DiED::BasicMessage(DiED::_KnownClientsMessage),
	m_MessageID(MessageID),
	m_ClientInfos(ClientInfos)
{
	vRegisterValue(m_MessageID);
	vRegisterValue(m_ClientInfos);
}

void DiED::KnownClientsMessage::vExecute(DiED::MessageTarget & MessageTarget)
{
	MessageTarget.vHandleKnownClients(m_MessageID, m_ClientInfos);
}

bool DiED::KnownClientsMessage::bRequiresConfirmation(void)
{
	return true;
}

bool DiED::KnownClientsMessage::bIsConfirmedBy(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters)
{
	if(m_MessageID == 0)
	{
		if(ConfirmationParameters->size() != 1)
		{
			return false;
		}
		
		DiED::ConfirmationParameters::iterator iParameter(ConfirmationParameters->find("Type"));
		
		if((iParameter == ConfirmationParameters->end()) || (boost::any_cast< Network::BasicMessage::type_t >(iParameter->second) != DiED::_KnownClientsMessage))
		{
			return false;
		}
	}
	else
	{
		if(ConfirmationParameters->size() != 2)
		{
			return false;
		}
		
		DiED::ConfirmationParameters::iterator iParameter(ConfirmationParameters->find("Type"));
		
		if((iParameter == ConfirmationParameters->end()) || (boost::any_cast< Network::BasicMessage::type_t >(iParameter->second) != DiED::_ClientsRegisteredMessage))
		{
			return false;
		}
		iParameter = ConfirmationParameters->find("MessageID");
		if((iParameter == ConfirmationParameters->end()) || (boost::any_cast< DiED::messageid_t >(iParameter->second) != m_MessageID))
		{
			return false;
		}
	}
	
	return true;
}

bool DiED::KnownClientsMessage::bOnTimeout(DiED::MessageTarget * pMessageTarget)
{
	if(pMessageTarget != 0)
	{
		boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters(new DiED::ConfirmationParameters());
		
		if(m_MessageID == 0)
		{
			ConfirmationParameters->insert(std::make_pair("Type", boost::any(static_cast< Network::BasicMessage::type_t >(DiED::_KnownClientsMessage))));
		}
		else
		{
			ConfirmationParameters->insert(std::make_pair("Type", boost::any(static_cast< Network::BasicMessage::type_t >(DiED::_ClientsRegisteredMessage))));
			ConfirmationParameters->insert(std::make_pair("MassageID", boost::any(static_cast< DiED::messageid_t >(m_MessageID))));
		}
		pMessageTarget->vHandleKnownClientsConfirmationTimeout(ConfirmationParameters);
	}
	
	return false;
}

boost::shared_ptr< DiED::ConfirmationParameters > DiED::KnownClientsMessage::GetConfirmationParameters(void)
{
	boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters(new DiED::ConfirmationParameters());
	
	ConfirmationParameters->insert(std::make_pair("Type", boost::any(static_cast< Network::BasicMessage::type_t >(DiED::_KnownClientsMessage))));
	
	return ConfirmationParameters;
}

Glib::ustring DiED::KnownClientsMessage::sGetString(void)
{
	std::stringstream ssString;
	
	ssString << "KnownClients [MessageID = " << m_MessageID << " ; #Clients = " << m_ClientInfos.size() << "]";
	
	return ssString.str();
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///                                  ClientsRegisteredMessage                                   ///
///////////////////////////////////////////////////////////////////////////////////////////////////

DiED::ClientsRegisteredMessage::ClientsRegisteredMessage(void) :
	DiED::BasicMessage(DiED::_ClientsRegisteredMessage)
{
	vRegisterValue(m_MessageID);
}

DiED::ClientsRegisteredMessage::ClientsRegisteredMessage(DiED::messageid_t MessageID) :
	DiED::BasicMessage(DiED::_ClientsRegisteredMessage),
	m_MessageID(MessageID)
{
	vRegisterValue(m_MessageID);
}

void DiED::ClientsRegisteredMessage::vExecute(DiED::MessageTarget & MessageTarget)
{
	LOG(Verbose, "DiED/Messages", "ClientsRegisteredMessage [MessageID = " << m_MessageID << "]");
	MessageTarget.vHandleClientsRegistered(m_MessageID);
}

boost::shared_ptr< DiED::ConfirmationParameters > DiED::ClientsRegisteredMessage::GetConfirmationParameters(void)
{
	boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters(new DiED::ConfirmationParameters());
	
	ConfirmationParameters->insert(std::make_pair("Type", boost::any(static_cast< Network::BasicMessage::type_t >(GetType()))));
	ConfirmationParameters->insert(std::make_pair("MessageID", boost::any(static_cast< DiED::messageid_t >(m_MessageID))));
	
	return ConfirmationParameters;
}

Glib::ustring DiED::ClientsRegisteredMessage::sGetString(void)
{
	std::stringstream ssString;
	
	ssString << "ClientsRegistered [MessageID = " << m_MessageID << "]";
	
	return ssString.str();
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///                                ConnectionEstablishedMessage                                 ///
///////////////////////////////////////////////////////////////////////////////////////////////////

DiED::ConnectionEstablishedMessage::ConnectionEstablishedMessage(void) :
	DiED::BasicMessage(DiED::_ConnectionEstablishedMessage)
{
	vRegisterValue(m_MessageID);
	vRegisterValue(m_ClientID);
	vRegisterValue(m_ClientAddress);
	vRegisterValue(m_ClientPort);
}

DiED::ConnectionEstablishedMessage::ConnectionEstablishedMessage(const DiED::messageid_t & MessageID, const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort) :
	DiED::BasicMessage(DiED::_ConnectionEstablishedMessage),
	m_MessageID(MessageID),
	m_ClientID(ClientID),
	m_ClientAddress(ClientAddress),
	m_ClientPort(ClientPort)
{
	vRegisterValue(m_MessageID);
	vRegisterValue(m_ClientID);
	vRegisterValue(m_ClientAddress);
	vRegisterValue(m_ClientPort);
}

void DiED::ConnectionEstablishedMessage::vExecute(DiED::MessageTarget & MessageTarget)
{
//~ 	LOG(Verbose, "DiED/Messages", "Executing a ConnectionEstablished message with parameters:\n\tClientID = " << m_ClientID << "\n\tClientAddress = " << m_ClientAddress << "\n\tClientPort = " << m_ClientPort);
	MessageTarget.vHandleConnectionEstablished(m_MessageID, m_ClientID, m_ClientAddress, m_ClientPort);
}

bool DiED::ConnectionEstablishedMessage::bRequiresConfirmation(void)
{
	return true;
}

bool DiED::ConnectionEstablishedMessage::bIsConfirmedBy(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters)
{
	if(ConfirmationParameters->size() != 2)
	{
		return false;
	}
	
	DiED::ConfirmationParameters::iterator iParameter(ConfirmationParameters->find("Type"));
	
	if((iParameter == ConfirmationParameters->end()) || (boost::any_cast< Network::BasicMessage::type_t >(iParameter->second) != DiED::_StatusConfirmMessage))
	{
		return false;
	}
	iParameter = ConfirmationParameters->find("MessageID");
	if((iParameter == ConfirmationParameters->end()) || (boost::any_cast< DiED::messageid_t >(iParameter->second) != m_MessageID))
	{
		return false;
	}
	
	return true;
}

Glib::ustring DiED::ConnectionEstablishedMessage::sGetString(void)
{
	std::stringstream ssString;
	
	ssString << "ConnectionEstablished [MessageID = " << m_MessageID << " ; ClientID = " << m_ClientID << " ; ClientAddress = " << m_ClientAddress.Get() << " ;  ClientPort = " << m_ClientPort << "]";
	
	return ssString.str();
}

bool DiED::ConnectionEstablishedMessage::bOnTimeout(DiED::MessageTarget * pMessageTarget)
{
	if(pMessageTarget != 0)
	{
		boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters(new DiED::ConfirmationParameters());
		
		ConfirmationParameters->insert(std::make_pair("Type", boost::any(static_cast< Network::BasicMessage::type_t >(DiED::_StatusConfirmMessage))));
		ConfirmationParameters->insert(std::make_pair("MessageID", boost::any(static_cast< DiED::messageid_t >(m_MessageID))));
		
		pMessageTarget->vHandleConnectionEstablishedConfirmationTimeout(ConfirmationParameters);
	}
	
	return false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///                                    ConnectionLostMessage                                    ///
///////////////////////////////////////////////////////////////////////////////////////////////////

DiED::ConnectionLostMessage::ConnectionLostMessage(void) :
	DiED::BasicMessage(DiED::_ConnectionLostMessage)
{
	vRegisterValue(m_MessageID);
	vRegisterValue(m_ClientID);
	vRegisterValue(m_ClientAddress);
	vRegisterValue(m_ClientPort);
}

DiED::ConnectionLostMessage::ConnectionLostMessage(const DiED::messageid_t & MessageID, const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort) :
	DiED::BasicMessage(DiED::_ConnectionLostMessage),
	m_MessageID(MessageID),
	m_ClientID(ClientID),
	m_ClientAddress(ClientAddress),
	m_ClientPort(ClientPort)
{
	vRegisterValue(m_MessageID);
	vRegisterValue(m_ClientID);
	vRegisterValue(m_ClientAddress);
	vRegisterValue(m_ClientPort);
}

void DiED::ConnectionLostMessage::vExecute(DiED::MessageTarget & MessageTarget)
{
	MessageTarget.vHandleConnectionLost(m_MessageID, m_ClientID, m_ClientAddress, m_ClientPort);
}

bool DiED::ConnectionLostMessage::bRequiresConfirmation(void)
{
	return true;
}

bool DiED::ConnectionLostMessage::bIsConfirmedBy(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters)
{
	if(ConfirmationParameters->size() != 2)
	{
		return false;
	}
	
	DiED::ConfirmationParameters::iterator iParameter(ConfirmationParameters->find("Type"));
	
	if((iParameter == ConfirmationParameters->end()) || (boost::any_cast< Network::BasicMessage::type_t >(iParameter->second) != DiED::_StatusConfirmMessage))
	{
		return false;
	}
	iParameter = ConfirmationParameters->find("MessageID");
	if((iParameter == ConfirmationParameters->end()) || (boost::any_cast< DiED::messageid_t >(iParameter->second) != m_MessageID))
	{
		return false;
	}
	
	return true;
}

Glib::ustring DiED::ConnectionLostMessage::sGetString(void)
{
	std::stringstream ssString;
	
	ssString << "ConnectionLost [MessageID = " << m_MessageID << " ; ClientID = " << m_ClientID << " ; ClientAddress = " << m_ClientAddress.Get() << " ; ClientPort = " << m_ClientPort << "]";
	
	return ssString.str();
}

bool DiED::ConnectionLostMessage::bOnTimeout(DiED::MessageTarget * pMessageTarget)
{
	if(pMessageTarget != 0)
	{
		boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters(new DiED::ConfirmationParameters());
		
		ConfirmationParameters->insert(std::make_pair("Type", boost::any(static_cast< Network::BasicMessage::type_t >(DiED::_StatusConfirmMessage))));
		ConfirmationParameters->insert(std::make_pair("MessageID", boost::any(static_cast< DiED::messageid_t >(m_MessageID))));
		
		pMessageTarget->vHandleConnectionLostConfirmationTimeout(ConfirmationParameters);
	}
	
	return false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///                                     StatusConfirmMessage                                    ///
///////////////////////////////////////////////////////////////////////////////////////////////////

DiED::StatusConfirmMessage::StatusConfirmMessage(void) :
	DiED::BasicMessage(DiED::_StatusConfirmMessage)
{
	vRegisterValue(m_MessageID);
}

DiED::StatusConfirmMessage::StatusConfirmMessage(const DiED::messageid_t & MessageID) :
	DiED::BasicMessage(DiED::_StatusConfirmMessage),
	m_MessageID(MessageID)
{
	vRegisterValue(m_MessageID);
}

void DiED::StatusConfirmMessage::vExecute(DiED::MessageTarget & MessageTarget)
{
	MessageTarget.vHandleStatusConfirm(m_MessageID);
}

boost::shared_ptr< DiED::ConfirmationParameters > DiED::StatusConfirmMessage::GetConfirmationParameters(void)
{
	boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters(new DiED::ConfirmationParameters());
	
	ConfirmationParameters->insert(std::make_pair("Type", boost::any(static_cast< Network::BasicMessage::type_t >(GetType()))));
	ConfirmationParameters->insert(std::make_pair("MessageID", boost::any(static_cast< DiED::messageid_t >(m_MessageID))));
	
	return ConfirmationParameters;
}

Glib::ustring DiED::StatusConfirmMessage::sGetString(void)
{
	std::stringstream ssString;
	
	ssString << "StatusConfirm [MessageID = " << m_MessageID << "]";
	
	return ssString.str();
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///                                         PingMessage                                         ///
///////////////////////////////////////////////////////////////////////////////////////////////////

DiED::PingMessage::PingMessage(void) :
	DiED::BasicMessage(DiED::_PingMessage)
{
	vRegisterValue(m_PingID);
}

DiED::PingMessage::PingMessage(const DiED::messageid_t & PingID) :
	DiED::BasicMessage(DiED::_PingMessage),
	m_PingID(PingID)
{
	vRegisterValue(m_PingID);
}

void DiED::PingMessage::vExecute(DiED::MessageTarget & MessageTarget)
{
	MessageTarget.vHandlePing(m_PingID);
}

Glib::ustring DiED::PingMessage::sGetString(void)
{
	std::stringstream ssString;
	
	ssString << "Ping [ PingID = " << m_PingID << " ]";
	
	return ssString.str();
}

bool DiED::PingMessage::bRequiresConfirmation(void)
{
	return true;
}

bool DiED::PingMessage::bIsConfirmedBy(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters)
{
	DiED::ConfirmationParameters::iterator iParameter(ConfirmationParameters->find("PingID"));
	
	if((iParameter == ConfirmationParameters->end()) || (boost::any_cast< DiED::messageid_t >(iParameter->second) != m_PingID))
	{
		return false;
	}
	
	return true;
}

bool DiED::PingMessage::bOnTimeout(DiED::MessageTarget * pMessageTarget)
{
	if(pMessageTarget != 0)
	{
		boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters(new DiED::ConfirmationParameters());
		
		ConfirmationParameters->insert(std::make_pair("PingID", boost::any(static_cast< DiED::messageid_t >(m_PingID))));
		pMessageTarget->vHandlePingConfirmationTimeout(ConfirmationParameters);
	}
	
	return false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///                                         PongMessage                                         ///
///////////////////////////////////////////////////////////////////////////////////////////////////

DiED::PongMessage::PongMessage(void) :
	DiED::BasicMessage(DiED::_PongMessage)
{
	vRegisterValue(m_PingID);
}

DiED::PongMessage::PongMessage(const DiED::messageid_t & PingID) :
	DiED::BasicMessage(DiED::_PongMessage),
	m_PingID(PingID)
{
	vRegisterValue(m_PingID);
}

void DiED::PongMessage::vExecute(DiED::MessageTarget & MessageTarget)
{
	MessageTarget.vHandlePong(m_PingID);
}

Glib::ustring DiED::PongMessage::sGetString(void)
{
	std::stringstream ssString;
	
	ssString << "Pong [ PingID = " << m_PingID << " ]";
	
	return ssString.str();
}

boost::shared_ptr< DiED::ConfirmationParameters > DiED::PongMessage::GetConfirmationParameters(void)
{
	boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters(new DiED::ConfirmationParameters());
	
	ConfirmationParameters->insert(std::make_pair("PingID", boost::any(static_cast< DiED::messageid_t >(m_PingID))));
	
	return ConfirmationParameters;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///                                    EventReceivedMessage                                     ///
///////////////////////////////////////////////////////////////////////////////////////////////////

DiED::EventReceivedMessage::EventReceivedMessage(void) :
	DiED::BasicMessage(DiED::_EventReceivedMessage)
{
	vRegisterValue(m_CreatorID);
	vRegisterValue(m_EventID);
}

DiED::EventReceivedMessage::EventReceivedMessage(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID) :
	DiED::BasicMessage(DiED::_EventReceivedMessage),
	m_CreatorID(CreatorID),
	m_EventID(EventID)
{
	vRegisterValue(m_CreatorID);
	vRegisterValue(m_EventID);
}

boost::shared_ptr< DiED::ConfirmationParameters > DiED::EventReceivedMessage::GetConfirmationParameters(void)
{
	boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters(new DiED::ConfirmationParameters());
	
	ConfirmationParameters->insert(std::make_pair("CreatorID", boost::any(static_cast< DiED::clientid_t >(m_CreatorID))));
	ConfirmationParameters->insert(std::make_pair("EventID", boost::any(static_cast< DiED::messageid_t >(m_EventID))));
	
	return ConfirmationParameters;
}

void DiED::EventReceivedMessage::vExecute(DiED::MessageTarget & MessageTarget)
{
	MessageTarget.vHandleEventReceived(m_CreatorID, m_EventID);
}

Glib::ustring DiED::EventReceivedMessage::sGetString(void)
{
	std::stringstream ssString;
	
	ssString << "EventReceived [ CreatorID = " << m_CreatorID << " ; EventID = " << m_EventID << " ]";
	
	return ssString.str();
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///                                        EventMessage                                         ///
///////////////////////////////////////////////////////////////////////////////////////////////////

DiED::EventMessage::EventMessage(const Network::BasicMessage::type_t & Type) :
	DiED::BasicMessage(Type)
{
	vRegisterValue(m_CreatorID);
	vRegisterValue(m_EventID);
	vRegisterValue(m_LostClientID);
}

DiED::EventMessage::EventMessage(const Network::BasicMessage::type_t & Type, const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID) :
	DiED::BasicMessage(Type),
	m_CreatorID(CreatorID),
	m_EventID(EventID),
	m_LostClientID(LostClientID)
{
	vRegisterValue(m_CreatorID);
	vRegisterValue(m_EventID);
	vRegisterValue(m_LostClientID);
}

void DiED::EventMessage::vSetLostClientID(const DiED::clientid_t & ClientID)
{
	m_LostClientID = ClientID;
}

bool DiED::EventMessage::bIsConfirmedBy(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters)
{
	DiED::ConfirmationParameters::iterator iParameter(ConfirmationParameters->find("CreatorID"));
	
	if((iParameter == ConfirmationParameters->end()) || (boost::any_cast< DiED::clientid_t >(iParameter->second) != m_CreatorID))
	{
		return false;
	}
	iParameter = ConfirmationParameters->find("EventID");
	if((iParameter == ConfirmationParameters->end()) || (boost::any_cast< DiED::messageid_t >(iParameter->second) != m_EventID))
	{
		return false;
	}
	
	return true;
}

bool DiED::EventMessage::bIsEventMessage(void)
{
	return true;
}

void DiED::EventMessage::vExecute(DiED::MessageTarget & MessageTarget)
{
	MessageTarget.vHandleEvent(m_CreatorID, m_EventID, m_LostClientID, GetAction());
}

Glib::ustring DiED::EventMessage::sGetString(void)
{
	std::stringstream ssString;
	
	ssString << "Event [ CreatorID = " << m_CreatorID << " ; EventID = " << m_EventID << " ; LostClientID = " << m_LostClientID << " ]";
	
	return ssString.str();
}

bool DiED::EventMessage::bRequiresConfirmation(void)
{
	return true;
}

bool DiED::EventMessage::bOnTimeout(DiED::MessageTarget * pMessageTarget)
{
	if(pMessageTarget != 0)
	{
		boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters(new DiED::ConfirmationParameters());
		
		ConfirmationParameters->insert(std::make_pair("CreatorID", boost::any(static_cast< DiED::clientid_t >(m_CreatorID))));
		ConfirmationParameters->insert(std::make_pair("EventID", boost::any(static_cast< DiED::messageid_t >(m_EventID))));
		
		pMessageTarget->vHandleEventConfirmationTimeout(ConfirmationParameters);
	}
	
	return false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///                                       InsertEvent                                           ///
///////////////////////////////////////////////////////////////////////////////////////////////////

DiED::InsertEvent::InsertEvent(void) :
	DiED::EventMessage(DiED::_InsertEvent)
{
	vRegisterValue(m_Text);
}

DiED::InsertEvent::InsertEvent(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID, const Glib::ustring & sText) :
	DiED::EventMessage(DiED::_InsertEvent, CreatorID, EventID, LostClientID),
	m_Text(sText)
{
	vRegisterValue(m_Text);
}

Glib::ustring DiED::InsertEvent::sGetString(void)
{
	std::stringstream ssString;
	
	ssString << "Insert [ " << DiED::EventMessage::sGetString() << " ; Text = " << m_Text.Get() << " ]";
	
	return ssString.str();
}

boost::shared_ptr< DiED::EventAction > DiED::InsertEvent::GetAction(void)
{
	return boost::shared_ptr< DiED::EventAction >(new DiED::InsertAction(m_Text));
}

boost::shared_ptr< DiED::BasicMessage > DiED::InsertEvent::Clone(void)
{
	return boost::shared_ptr< DiED::BasicMessage >(new DiED::InsertEvent(m_CreatorID, m_EventID, m_LostClientID, m_Text));
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///                                         DeleteEvent                                         ///
///////////////////////////////////////////////////////////////////////////////////////////////////

DiED::DeleteEvent::DeleteEvent(void) :
	DiED::EventMessage(DiED::_DeleteEvent)
{
	vRegisterValue(m_LineRelative);
	vRegisterValue(m_LineAbsolute);
	vRegisterValue(m_CharacterRelative);
	vRegisterValue(m_CharacterAbsolute);
}

DiED::DeleteEvent::DeleteEvent(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID, int iLineRelative, int iCharacterRelative, int iLineAbsolute, int iCharacterAbsolute) :
	DiED::EventMessage(DiED::_DeleteEvent, CreatorID, EventID, LostClientID),
	m_LineRelative(iLineRelative),
	m_CharacterRelative(iCharacterRelative),
	m_LineAbsolute(iLineAbsolute),
	m_CharacterAbsolute(iCharacterAbsolute)
{
	vRegisterValue(m_LineRelative);
	vRegisterValue(m_LineAbsolute);
	vRegisterValue(m_CharacterRelative);
	vRegisterValue(m_CharacterAbsolute);
}

Glib::ustring DiED::DeleteEvent::sGetString(void)
{
	std::stringstream ssString;
	
	ssString << "Delete [ " << DiED::EventMessage::sGetString() << " ; LineRelative = " << m_LineRelative << " ; CharacterRelative = " << m_CharacterRelative << " ; LineAbsolute = " << m_LineAbsolute << " ; CharacterAbsolute = " << m_CharacterAbsolute << " ]";
	
	return ssString.str();
}

boost::shared_ptr< DiED::BasicMessage > DiED::DeleteEvent::Clone(void)
{
	return boost::shared_ptr< DiED::BasicMessage >(new DiED::DeleteEvent(m_CreatorID, m_EventID, m_LostClientID, m_LineRelative, m_CharacterRelative, m_LineAbsolute, m_CharacterAbsolute));
}

boost::shared_ptr< DiED::EventAction > DiED::DeleteEvent::GetAction(void)
{
	return boost::shared_ptr< DiED::EventAction >(new DiED::DeleteAction(m_LineRelative, m_CharacterRelative, m_LineAbsolute, m_CharacterAbsolute));
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///                                        PositionEvent                                        ///
///////////////////////////////////////////////////////////////////////////////////////////////////

DiED::PositionEvent::PositionEvent(void) :
	DiED::EventMessage(DiED::_PositionEvent)
{
	vRegisterValue(m_LineRelative);
	vRegisterValue(m_LineAbsolute);
	vRegisterValue(m_CharacterRelative);
	vRegisterValue(m_CharacterAbsolute);
}

DiED::PositionEvent::PositionEvent(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID, int iLineRelative, int iCharacterRelative, int iLineAbsolute, int iCharacterAbsolute) :
	DiED::EventMessage(DiED::_PositionEvent, CreatorID, EventID, LostClientID),
	m_LineRelative(iLineRelative),
	m_CharacterRelative(iCharacterRelative),
	m_LineAbsolute(iLineAbsolute),
	m_CharacterAbsolute(iCharacterAbsolute)
{
	vRegisterValue(m_LineRelative);
	vRegisterValue(m_LineAbsolute);
	vRegisterValue(m_CharacterRelative);
	vRegisterValue(m_CharacterAbsolute);
}

Glib::ustring DiED::PositionEvent::sGetString(void)
{
	std::stringstream ssString;
	
	ssString << "Position [ " << DiED::EventMessage::sGetString() << " ; LineRelative = " << m_LineRelative << " ; CharacterRelative = " << m_CharacterRelative << " ; LineAbsolute = " << m_LineAbsolute << " ; CharacterAbsolute = " << m_CharacterAbsolute << " ]";
	
	return ssString.str();
}

boost::shared_ptr< DiED::BasicMessage > DiED::PositionEvent::Clone(void)
{
	return boost::shared_ptr< DiED::BasicMessage >(new DiED::PositionEvent(m_CreatorID, m_EventID, m_LostClientID, m_LineRelative, m_CharacterRelative, m_LineAbsolute, m_CharacterAbsolute));
}

boost::shared_ptr< DiED::EventAction > DiED::PositionEvent::GetAction(void)
{
	return boost::shared_ptr< DiED::EventAction >(new DiED::PositionAction(m_LineRelative, m_CharacterRelative, m_LineAbsolute, m_CharacterAbsolute));
}
