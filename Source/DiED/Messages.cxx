#include "Messages.h"

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

void DiED::NoMessage::vExecute(DiED::Client & Client)
{
}

void DiED::NoMessage::vWriteToInternal(Network::Stream & Stream) const
{
}

DiED::ConnectMessage::ConnectMessage(void) :
	DiED::BasicMessage(DiED::_ConnectMessage, false)
{
}

DiED::ConnectMessage::ConnectMessage(const DiED::clientid_t & ClientID, const Network::port_t & Port) :
	DiED::BasicMessage(DiED::_ConnectMessage, true),
	m_ClientID(ClientID),
	m_Port(Port)
{
}

bool DiED::ConnectMessage::bIsReady(void) const
{
	return (m_ClientID.bIsReady() == true) && (m_Port.bIsReady() == true);
}

void DiED::ConnectMessage::vReadFrom(Network::Stream & Stream)
{
	Stream >> m_ClientID >> m_Port;
}

void DiED::ConnectMessage::vExecute(DiED::Client & Client)
{
	std::cout << "Executing a Connect message with parameters:\n\tClientID = " << m_ClientID << "\n\tPort = " << m_Port << std::endl;
}

void DiED::ConnectMessage::vWriteToInternal(Network::Stream & Stream) const
{
	Stream << m_ClientID << m_Port;
}

DiED::InputMessage::InputMessage(void) :
	DiED::BasicMessage(DiED::_InputMessage, false)
{
}

DiED::InputMessage::InputMessage(const Glib::ustring & sString) :
	DiED::BasicMessage(DiED::_InputMessage, true),
	m_String(sString)
{
}

bool DiED::InputMessage::bIsReady(void) const
{
	return m_String.bIsReady();
}

void DiED::InputMessage::vReadFrom(Network::Stream & Stream)
{
	Stream >> m_String;
}

void DiED::InputMessage::vWriteToInternal(Network::Stream & Stream) const
{
	Stream << m_String;
}

void DiED::InputMessage::vExecute(DiED::Client & Client)
{
	Client.vInsertText(m_String);
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

void DiED::PingMessage::vExecute(DiED::Client & Client)
{
	Client << DiED::PongMessage();
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

void DiED::PongMessage::vExecute(DiED::Client & Client)
{
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

void DiED::ConnectionEstablishedMessage::vExecute(DiED::Client & Client)
{
	std::cout << "Executing a ConnectionEstablished message with parameters:\n\tClientID = " << m_ClientID << "\n\tClientAddress = " << m_ClientAddress << "\n\tClientPort = " << m_ClientPort << std::endl;
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

void DiED::ConnectionLostMessage::vExecute(DiED::Client & Client)
{
	std::cout << "Executing a ConnectionLost message with parameters:\n\tClientID = " << m_ClientID << std::endl;
}

void DiED::ConnectionLostMessage::vWriteToInternal(Network::Stream & Stream) const
{
	Stream << m_ClientID;
}
