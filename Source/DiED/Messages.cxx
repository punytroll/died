#include "Messages.h"

#include <iostream>
#include <sstream>

DiED::NoMessage::NoMessage(void) :
	DiED::BasicMessage(DiED::_NoMessage, false)
{
}

bool DiED::NoMessage::bIsReady(void) const
{
	return true;
}

void DiED::NoMessage::vReadFrom(Network::Stream & Stream)
{
}

void DiED::NoMessage::vExecute(DiED::MessageTarget & MessageTarget)
{
}

Glib::ustring DiED::NoMessage::sGetString(void)
{
	return "NoMessage []";
}

void DiED::NoMessage::vWriteToInternal(Network::Stream & Stream) const
{
}

DiED::ConnectionRequestMessage::ConnectionRequestMessage(void) :
	DiED::BasicMessage(DiED::_ConnectionRequestMessage, false)
{
}

DiED::ConnectionRequestMessage::ConnectionRequestMessage(const DiED::clientid_t & ClientID, const Network::port_t & Port) :
	DiED::BasicMessage(DiED::_ConnectionRequestMessage, true),
	m_ClientID(ClientID),
	m_Port(Port)
{
}

bool DiED::ConnectionRequestMessage::bIsReady(void) const
{
	return (m_ClientID.bIsReady() == true) && (m_Port.bIsReady() == true);
}

void DiED::ConnectionRequestMessage::vReadFrom(Network::Stream & Stream)
{
	Stream >> m_ClientID >> m_Port;
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

void DiED::ConnectionRequestMessage::vWriteToInternal(Network::Stream & Stream) const
{
	Stream << m_ClientID << m_Port;
}

DiED::ConnectionAcceptMessage::ConnectionAcceptMessage(void) :
	DiED::BasicMessage(DiED::_ConnectionAcceptMessage, false)
{
}

DiED::ConnectionAcceptMessage::ConnectionAcceptMessage(const DiED::clientid_t & RemoteClientID, const DiED::clientid_t & LocalClientID) :
	DiED::BasicMessage(DiED::_ConnectionAcceptMessage, true),
	m_RemoteClientID(RemoteClientID),
	m_LocalClientID(LocalClientID)
{
}

bool DiED::ConnectionAcceptMessage::bIsReady(void) const
{
	return (m_RemoteClientID.bIsReady() == true) && (m_LocalClientID.bIsReady() == true);
}

void DiED::ConnectionAcceptMessage::vReadFrom(Network::Stream & Stream)
{
	// different order than vWriteToInternal because the sides of the socket switched
	Stream >> m_LocalClientID >> m_RemoteClientID;
}

void DiED::ConnectionAcceptMessage::vExecute(DiED::MessageTarget & MessageTarget)
{
//~ 	std::cout << "Executing a ConnectionAccept message with parameters:\n\tRemoteClientID = " << m_RemoteClientID << "\n\tLocalClientID = " << m_LocalClientID << std::endl;
	MessageTarget.vConnectionAccept(m_LocalClientID, m_RemoteClientID);
}

Glib::ustring DiED::ConnectionAcceptMessage::sGetString(void)
{
	std::stringstream ssString;
	
	ssString << "ConnectionAcceptMessage [RemoteClientID = " << m_RemoteClientID << " ; LocalClientID = " << m_LocalClientID << "]";
	
	return ssString.str();
}

void DiED::ConnectionAcceptMessage::vWriteToInternal(Network::Stream & Stream) const
{
	Stream << m_RemoteClientID << m_LocalClientID;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

DiED::KnownClientsMessage::KnownClientsMessage(void) :
	DiED::BasicMessage(DiED::_KnownClientsMessage, false)
{
}

DiED::KnownClientsMessage::KnownClientsMessage(u_int32_t u32MessageID, std::vector< DiED::clientid_t > ConnectedClientIDs, std::vector< DiED::clientid_t > DisconnectedClientIDs) :
	DiED::BasicMessage(DiED::_KnownClientsMessage, true),
	m_MessageID(u32MessageID),
	m_ConnectedClientIDs(ConnectedClientIDs),
	m_DisconnectedClientIDs(DisconnectedClientIDs)
{
}

bool DiED::KnownClientsMessage::bIsReady(void) const
{
	return (m_MessageID.bIsReady() == true) && (m_ConnectedClientIDs.bIsReady() == true) && (m_DisconnectedClientIDs.bIsReady() == true);
}

void DiED::KnownClientsMessage::vReadFrom(Network::Stream & Stream)
{
	Stream >> m_MessageID >> m_ConnectedClientIDs >> m_DisconnectedClientIDs;
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

void DiED::KnownClientsMessage::vWriteToInternal(Network::Stream & Stream) const
{
	Stream << m_MessageID << m_ConnectedClientIDs << m_DisconnectedClientIDs;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

DiED::ClientsRegisteredMessage::ClientsRegisteredMessage(void) :
	DiED::BasicMessage(DiED::_ClientsRegisteredMessage, false)
{
}

DiED::ClientsRegisteredMessage::ClientsRegisteredMessage(DiED::messageid_t MessageID) :
	DiED::BasicMessage(DiED::_ClientsRegisteredMessage, true),
	m_MessageID(MessageID)
{
}

bool DiED::ClientsRegisteredMessage::bIsReady(void) const
{
	return (m_MessageID.bIsReady() == true);
}

void DiED::ClientsRegisteredMessage::vReadFrom(Network::Stream & Stream)
{
	Stream >> m_MessageID;
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

void DiED::ClientsRegisteredMessage::vWriteToInternal(Network::Stream & Stream) const
{
	Stream << m_MessageID;
}

DiED::PingMessage::PingMessage(void) :
	DiED::BasicMessage(DiED::_PingMessage, true)
{
}

bool DiED::PingMessage::bIsReady(void) const
{
	return true;
}

void DiED::PingMessage::vReadFrom(Network::Stream & Stream)
{
}

void DiED::PingMessage::vWriteToInternal(Network::Stream & Stream) const
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

DiED::PongMessage::PongMessage(void) :
	DiED::BasicMessage(DiED::_PongMessage, true)
{
}

bool DiED::PongMessage::bIsReady(void) const
{
	return true;
}

void DiED::PongMessage::vReadFrom(Network::Stream & Stream)
{
}

void DiED::PongMessage::vWriteToInternal(Network::Stream & Stream) const
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

DiED::ConnectionEstablishedMessage::ConnectionEstablishedMessage(void) :
	DiED::BasicMessage(DiED::_ConnectionEstablishedMessage, false)
{
}

DiED::ConnectionEstablishedMessage::ConnectionEstablishedMessage(const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort) :
	DiED::BasicMessage(DiED::_ConnectionEstablishedMessage, true),
	m_ClientID(ClientID),
	m_ClientAddress(ClientAddress),
	m_ClientPort(ClientPort)
{
}

bool DiED::ConnectionEstablishedMessage::bIsReady(void) const
{
	return (m_ClientID.bIsReady() == true) && (m_ClientAddress.bIsReady() == true) && (m_ClientPort.bIsReady() == true);
}

void DiED::ConnectionEstablishedMessage::vReadFrom(Network::Stream & Stream)
{
	Stream >> m_ClientID >> m_ClientAddress >> m_ClientPort;
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

void DiED::ConnectionEstablishedMessage::vWriteToInternal(Network::Stream & Stream) const
{
	Stream << m_ClientID << m_ClientAddress << m_ClientPort;
}

DiED::ConnectionLostMessage::ConnectionLostMessage(void) :
	DiED::BasicMessage(DiED::_ConnectionLostMessage, false)
{
}

DiED::ConnectionLostMessage::ConnectionLostMessage(const DiED::clientid_t & ClientID) :
	DiED::BasicMessage(DiED::_ConnectionLostMessage, true),
	m_ClientID(ClientID)
{
}

bool DiED::ConnectionLostMessage::bIsReady(void) const
{
	return m_ClientID.bIsReady() == true;
}

void DiED::ConnectionLostMessage::vReadFrom(Network::Stream & Stream)
{
	Stream >> m_ClientID;
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

void DiED::ConnectionLostMessage::vWriteToInternal(Network::Stream & Stream) const
{
	Stream << m_ClientID;
}

DiED::TestMessage::TestMessage(void) :
	DiED::BasicMessage(DiED::_TestMessage, true)
{
}

bool DiED::TestMessage::bIsReady(void) const
{
	return m_Values.bIsReady();
}

void DiED::TestMessage::vReadFrom(Network::Stream & Stream)
{
	Stream >> m_Values;
}

void DiED::TestMessage::vExecute(DiED::MessageTarget & MessageTarget)
{
//~ 	std::cout << "Executing a Test message with " << m_Values.size() << " parameter(s):";
//~ 	std::cout << std::endl;
}

Glib::ustring DiED::TestMessage::sGetString(void)
{
	std::stringstream ssString;
	
	ssString << "TestMessage []";
	
	return ssString.str();
}

void DiED::TestMessage::vWriteToInternal(Network::Stream & Stream) const
{
	Stream << m_Values;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

DiED::EventMessage::EventMessage(const Network::BasicMessage::type_t & Type, bool bForSending) :
	DiED::BasicMessage(Type, bForSending)
{
}

DiED::EventMessage::EventMessage(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID, const Network::BasicMessage::type_t & Type, bool bForSending) :
	DiED::BasicMessage(Type, bForSending),
	m_CreatorID(CreatorID),
	m_EventID(EventID),
	m_LostClientID(LostClientID)
{
	
}

bool DiED::EventMessage::bIsEventMessage(void)
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

DiED::InputMessage::InputMessage(void) :
	DiED::EventMessage(DiED::_InputMessage, false)
{
}

DiED::InputMessage::InputMessage(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID, const Glib::ustring & sString) :
	DiED::EventMessage(CreatorID, EventID, LostClientID, DiED::_InputMessage, true),
	m_String(sString)
{
//~ 	vRegisterVariable(m_String, "Text");
}

bool DiED::InputMessage::bIsReady(void) const
{
	return m_String.bIsReady();
}

void DiED::InputMessage::vReadFrom(Network::Stream & Stream)
{
//~ 	std::cout << "Doing the reading" << std::endl;
	Stream >> m_String;
//~ 	std::cout << "Done the reading" << std::endl;
}

void DiED::InputMessage::vWriteToInternal(Network::Stream & Stream) const
{
	Stream << m_String;
}

void DiED::InputMessage::vExecute(DiED::MessageTarget & MessageTarget)
{
	MessageTarget.vInsertText(m_String);
}

Glib::ustring DiED::InputMessage::sGetString(void)
{
	std::stringstream ssString;
	
	ssString << "InputMessage [Text = " << m_String << "]";
	
	return ssString.str();
}
