#include "Messages.h"

DiED::NoMessage::NoMessage(void) :
	DiED::BasicMessage(DiED::_NoMessage, false)
{
}

bool DiED::NoMessage::bIsReady(void)
{
	return true;
}

void DiED::NoMessage::vReadFrom(Network::Stream & Stream)
{
}

void DiED::NoMessage::vWriteToInternal(Network::Stream & Stream) const
{
}

void DiED::NoMessage::vExecute(DiED::Client & Client)
{
}

DiED::HelloMessage::HelloMessage(void) :
	DiED::BasicMessage(DiED::_HelloMessage, true)
{
}

bool DiED::HelloMessage::bIsReady(void)
{
	return true;
}

void DiED::HelloMessage::vReadFrom(Network::Stream & Stream)
{
}

void DiED::HelloMessage::vWriteToInternal(Network::Stream & Stream) const
{
}

void DiED::HelloMessage::vExecute(DiED::Client & Client)
{
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

bool DiED::InputMessage::bIsReady(void)
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

bool DiED::PingMessage::bIsReady(void)
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

bool DiED::PongMessage::bIsReady(void)
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
