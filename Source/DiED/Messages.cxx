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


///////////////////////////////////////////////////////////////////////////////////////////////////
///                                   ConnectionAcceptMessage                                   ///
///////////////////////////////////////////////////////////////////////////////////////////////////

DiED::ConnectionAcceptMessage::ConnectionAcceptMessage(void) :
	DiED::BasicMessage(DiED::_ConnectionAcceptMessage)
{
	vRegisterValue(m_RemoteClientID);
	vRegisterValue(m_LocalClientID);
}

DiED::ConnectionAcceptMessage::ConnectionAcceptMessage(const DiED::clientid_t & RemoteClientID, const DiED::clientid_t & LocalClientID) :
	DiED::BasicMessage(DiED::_ConnectionAcceptMessage),
	m_RemoteClientID(RemoteClientID),
	m_LocalClientID(LocalClientID)
{
	vRegisterValue(m_RemoteClientID);
	vRegisterValue(m_LocalClientID);
}

void DiED::ConnectionAcceptMessage::vExecute(DiED::MessageTarget & MessageTarget)
{
//~ 	std::cout << "Executing a ConnectionAccept message with parameters:\n\tRemoteClientID = " << m_RemoteClientID << "\n\tLocalClientID = " << m_LocalClientID << std::endl;
	MessageTarget.vHandleConnectionAccept(m_RemoteClientID, m_LocalClientID);
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
	
	ssString << "ConnectionAccept [RemoteClientID = " << m_RemoteClientID << " ; LocalClientID = " << m_LocalClientID << "]";
	
	return ssString.str();
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///                                     KnownClientsMessage                                     ///
///////////////////////////////////////////////////////////////////////////////////////////////////

DiED::KnownClientsMessage::KnownClientsMessage(void) :
	DiED::BasicMessage(DiED::_KnownClientsMessage)
{
	vRegisterValue(m_MessageID);
	vRegisterValue(m_ConnectedClientIDs);
	vRegisterValue(m_DisconnectedClientIDs);
}

DiED::KnownClientsMessage::KnownClientsMessage(u_int32_t u32MessageID, std::vector< DiED::clientid_t > ConnectedClientIDs, std::vector< DiED::clientid_t > DisconnectedClientIDs) :
	DiED::BasicMessage(DiED::_KnownClientsMessage),
	m_MessageID(u32MessageID),
	m_ConnectedClientIDs(ConnectedClientIDs),
	m_DisconnectedClientIDs(DisconnectedClientIDs)
{
	vRegisterValue(m_MessageID);
	vRegisterValue(m_ConnectedClientIDs);
	vRegisterValue(m_DisconnectedClientIDs);
}

void DiED::KnownClientsMessage::vExecute(DiED::MessageTarget & MessageTarget)
{
//~ 	std::cout << "KnownClientsMessage [MessageID = " << m_MessageID << " ; #Connected = " << m_ConnectedClientIDs.size() << " ; #Disconnected = " << m_DisconnectedClientIDs.size() << "]" << std::endl;
	MessageTarget.vHandleKnownClients(m_MessageID, m_ConnectedClientIDs, m_DisconnectedClientIDs);
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

boost::shared_ptr< DiED::ConfirmationParameters > DiED::KnownClientsMessage::GetConfirmationParameters(void)
{
	boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters(new DiED::ConfirmationParameters());
	
	ConfirmationParameters->insert(std::make_pair("Type", boost::any(static_cast< Network::BasicMessage::type_t >(GetType()))));
	
	return ConfirmationParameters;
}

Glib::ustring DiED::KnownClientsMessage::sGetString(void)
{
	std::stringstream ssString;
	
	ssString << "KnownClients [MessageID = " << m_MessageID << " ; #Connected = " << m_ConnectedClientIDs.size() << " ; #Disconnected = " << m_DisconnectedClientIDs.size() << "]";
	
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
//~ 	std::cout << "ClientsRegisteredMessage [MessageID = " << m_MessageID << "]" << std::endl;
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
	vRegisterValue(m_ClientID);
	vRegisterValue(m_ClientAddress);
	vRegisterValue(m_ClientPort);
}

DiED::ConnectionEstablishedMessage::ConnectionEstablishedMessage(const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort) :
	DiED::BasicMessage(DiED::_ConnectionEstablishedMessage),
	m_ClientID(ClientID),
	m_ClientAddress(ClientAddress),
	m_ClientPort(ClientPort)
{
	vRegisterValue(m_ClientID);
	vRegisterValue(m_ClientAddress);
	vRegisterValue(m_ClientPort);
}

void DiED::ConnectionEstablishedMessage::vExecute(DiED::MessageTarget & MessageTarget)
{
//~ 	std::cout << "Executing a ConnectionEstablished message with parameters:\n\tClientID = " << m_ClientID << "\n\tClientAddress = " << m_ClientAddress << "\n\tClientPort = " << m_ClientPort << std::endl;
	MessageTarget.vHandleConnectionEstablished(m_ClientID, m_ClientAddress, m_ClientPort);
}

Glib::ustring DiED::ConnectionEstablishedMessage::sGetString(void)
{
	std::stringstream ssString;
	
	ssString << "ConnectionEstablished [ ClientID = " << m_ClientID << " ;  ClientAddress = " << m_ClientAddress << " ;  ClientPort = " << m_ClientPort << " ]";
	
	return ssString.str();
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///                                    ConnectionLostMessage                                    ///
///////////////////////////////////////////////////////////////////////////////////////////////////

DiED::ConnectionLostMessage::ConnectionLostMessage(void) :
	DiED::BasicMessage(DiED::_ConnectionLostMessage)
{
	vRegisterValue(m_ClientID);
	vRegisterValue(m_ClientAddress);
	vRegisterValue(m_ClientPort);
}

DiED::ConnectionLostMessage::ConnectionLostMessage(const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort) :
	DiED::BasicMessage(DiED::_ConnectionLostMessage),
	m_ClientID(ClientID),
	m_ClientAddress(ClientAddress),
	m_ClientPort(ClientPort)
{
	vRegisterValue(m_ClientID);
	vRegisterValue(m_ClientAddress);
	vRegisterValue(m_ClientPort);
}

void DiED::ConnectionLostMessage::vExecute(DiED::MessageTarget & MessageTarget)
{
	MessageTarget.vHandleConnectionLost(m_ClientID, m_ClientAddress, m_ClientPort);
}

Glib::ustring DiED::ConnectionLostMessage::sGetString(void)
{
	std::stringstream ssString;
	
	ssString << "ConnectionLost [ ClientID = " << m_ClientID << " ; ClientAddress = " << m_ClientAddress << " ; ClientPort = " << m_ClientPort << " ]";
	
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
	MessageTarget.vHandleEvent(m_CreatorID, m_EventID, m_LostClientID);
	vExecuteEvent(MessageTarget);
}

Glib::ustring DiED::EventMessage::sGetString(void)
{
	std::stringstream ssString;
	
	ssString << "Event [ CreatorID = " << m_CreatorID << " ; EventID = " << m_EventID << " ; LostClientIO = " << m_LostClientID << " ]";
	
	return ssString.str();
}

bool DiED::EventMessage::bRequiresConfirmation(void)
{
	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///                                        InputMessage                                         ///
///////////////////////////////////////////////////////////////////////////////////////////////////

DiED::InputMessage::InputMessage(void) :
	DiED::EventMessage(DiED::_InputMessage)
{
	vRegisterValue(m_Text);
}

DiED::InputMessage::InputMessage(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID, const Glib::ustring & sText) :
	DiED::EventMessage(DiED::_InputMessage, CreatorID, EventID, LostClientID),
	m_Text(sText)
{
	vRegisterValue(m_Text);
}

void DiED::InputMessage::vExecuteEvent(DiED::MessageTarget & MessageTarget)
{
	MessageTarget.vHandleInsertText(m_Text);
}

Glib::ustring DiED::InputMessage::sGetString(void)
{
	std::stringstream ssString;
	
	ssString << "Input [ Event = " << DiED::EventMessage::sGetString() << " ; Text = " << m_Text << " ]";
	
	return ssString.str();
}
