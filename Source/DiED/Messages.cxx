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
//~ 	std::cout << "Executing a ConnectionRequest message with parameters:\n\tClientID = " << m_ClientID << "\n\tPort = " << m_Port << std::endl;
	MessageTarget.vConnectionRequest(m_ClientID, m_Port);
}

Glib::ustring DiED::ConnectionRequestMessage::sGetString(void)
{
	std::stringstream ssString;
	
	ssString << "ConnectionRequestMessage [ClientID = " << m_ClientID << " ;  Port = " << m_Port << "]";
	
	return ssString.str();
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
	MessageTarget.vConnectionAccept(m_RemoteClientID, m_LocalClientID);
}

Glib::ustring DiED::ConnectionAcceptMessage::sGetString(void)
{
	std::stringstream ssString;
	
	ssString << "ConnectionAcceptMessage [RemoteClientID = " << m_RemoteClientID << " ; LocalClientID = " << m_LocalClientID << "]";
	
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
	MessageTarget.vKnownClients(m_MessageID, m_ConnectedClientIDs, m_DisconnectedClientIDs);
}

Glib::ustring DiED::KnownClientsMessage::sGetString(void)
{
	std::stringstream ssString;
	
	ssString << "KnownClientsMessage [MessageID = " << m_MessageID << " ; #Connected = " << m_ConnectedClientIDs.size() << " ; #Disconnected = " << m_DisconnectedClientIDs.size() << "]";
	
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
	MessageTarget.vClientsRegistered(m_MessageID);
}

Glib::ustring DiED::ClientsRegisteredMessage::sGetString(void)
{
	std::stringstream ssString;
	
	ssString << "ClientsRegisteredMessage [MessageID = " << m_MessageID << "]";
	
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
	MessageTarget.vConnectionEstablished(m_ClientID, m_ClientAddress, m_ClientPort);
}

Glib::ustring DiED::ConnectionEstablishedMessage::sGetString(void)
{
	std::stringstream ssString;
	
	ssString << "ConnectionEstablished [ClientID = " << m_ClientID << " ;  ClientAddress = " << m_ClientAddress << " ;  ClientPort = " << m_ClientPort << "]";
	
	return ssString.str();
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///                                    ConnectionLostMessage                                    ///
///////////////////////////////////////////////////////////////////////////////////////////////////

DiED::ConnectionLostMessage::ConnectionLostMessage(void) :
	DiED::BasicMessage(DiED::_ConnectionLostMessage)
{
	vRegisterValue(m_ClientID);
}

DiED::ConnectionLostMessage::ConnectionLostMessage(const DiED::clientid_t & ClientID) :
	DiED::BasicMessage(DiED::_ConnectionLostMessage),
	m_ClientID(ClientID)
{
	vRegisterValue(m_ClientID);
}

void DiED::ConnectionLostMessage::vExecute(DiED::MessageTarget & MessageTarget)
{
//~ 	std::cout << "Executing a ConnectionLost message with parameters:\n\tClientID = " << m_ClientID << std::endl;
}

Glib::ustring DiED::ConnectionLostMessage::sGetString(void)
{
	std::stringstream ssString;
	
	ssString << "ConnectionLostMessage [ClientID = " << m_ClientID << "]";
	
	return ssString.str();
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///                                         PingMessage                                         ///
///////////////////////////////////////////////////////////////////////////////////////////////////

DiED::PingMessage::PingMessage(void) :
	DiED::BasicMessage(DiED::_PingMessage)
{
}

void DiED::PingMessage::vExecute(DiED::MessageTarget & MessageTarget)
{
//~ 	MessageTarget << boost::shared_ptr< Network::BasicMessage >(new DiED::PongMessage());
}

Glib::ustring DiED::PingMessage::sGetString(void)
{
	std::stringstream ssString;
	
	ssString << "PingMessage []";
	
	return ssString.str();
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///                                         PongMessage                                         ///
///////////////////////////////////////////////////////////////////////////////////////////////////

DiED::PongMessage::PongMessage(void) :
	DiED::BasicMessage(DiED::_PongMessage)
{
}

void DiED::PongMessage::vExecute(DiED::MessageTarget & MessageTarget)
{
}

Glib::ustring DiED::PongMessage::sGetString(void)
{
	std::stringstream ssString;
	
	ssString << "PongMessage []";
	
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

bool DiED::EventMessage::bIsEventMessage(void)
{
	return true;
}

void DiED::EventMessage::vExecute(DiED::MessageTarget & MessageTarget)
{
	vExecuteEvent(MessageTarget);
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
	MessageTarget.vInsertText(m_Text);
}

Glib::ustring DiED::InputMessage::sGetString(void)
{
	std::stringstream ssString;
	
	ssString << "InputMessage [Text = " << m_Text << "]";
	
	return ssString.str();
}
