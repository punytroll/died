#ifndef MESSAGES_H
#define MESSAGES_H

#include <Network/VectorValue.h>

#include "BasicMessage.h"

namespace DiED
{
	enum
	{
		_NoMessage,
		_ConnectionRequestMessage,
		_ConnectionAcceptMessage,
		_KnownClientsMessage,
		_ClientsRegisteredMessage,
		_InputMessage,
		_PingMessage,
		_PongMessage,
		_ConnectionEstablishedMessage,
		_ConnectionLostMessage,
		_TestMessage,
	};
	
	class NoMessage : public DiED::BasicMessage
	{
	public:
		NoMessage(void);
		virtual void vExecute(DiED::MessageTarget & MessageTarget);
		virtual Glib::ustring sGetString(void);
	};
	
	class ConnectionRequestMessage : public DiED::BasicMessage
	{
	public:
		ConnectionRequestMessage(void);
		ConnectionRequestMessage(const DiED::clientid_t & ClientID, const Network::port_t & Port);
		virtual void vExecute(DiED::MessageTarget & MessageTarget);
		virtual Glib::ustring sGetString(void);
	private:
		Network::Value< DiED::clientid_t > m_ClientID;
		Network::Value< Network::port_t > m_Port;
	};
	
	class ConnectionAcceptMessage : public DiED::BasicMessage
	{
	public:
		ConnectionAcceptMessage(void);
		ConnectionAcceptMessage(const DiED::clientid_t & RemoteClientID, const DiED::clientid_t & LocalClientID);
		virtual void vExecute(DiED::MessageTarget & MessageTarget);
		virtual Glib::ustring sGetString(void);
	private:
		Network::Value< DiED::clientid_t > m_RemoteClientID;
		Network::Value< DiED::clientid_t > m_LocalClientID;
	};
	
	class KnownClientsMessage : public DiED::BasicMessage
	{
	public:
		KnownClientsMessage(void);
		KnownClientsMessage(u_int32_t u32MessageID, std::vector< DiED::clientid_t > ConnectedClientIDs, std::vector< DiED::clientid_t > DisconnectedClientIDs);
		virtual void vExecute(DiED::MessageTarget & MessageTarget);
		virtual Glib::ustring sGetString(void);
	private:
		Network::Value< u_int32_t > m_MessageID;
		Network::VectorValue< DiED::clientid_t > m_ConnectedClientIDs;
		Network::VectorValue< DiED::clientid_t > m_DisconnectedClientIDs;
	};
	
	class ClientsRegisteredMessage : public DiED::BasicMessage
	{
	public:
		ClientsRegisteredMessage(void);
		ClientsRegisteredMessage(DiED::messageid_t MessageID);
		virtual void vExecute(DiED::MessageTarget & MessageTarget);
		virtual Glib::ustring sGetString(void);
	private:
		Network::Value< DiED::messageid_t > m_MessageID;
	};
	
	class PingMessage : public DiED::BasicMessage
	{
	public:
		PingMessage(void);
		virtual void vExecute(DiED::MessageTarget & MessageTarget);
		virtual Glib::ustring sGetString(void);
	};
	
	class PongMessage : public DiED::BasicMessage
	{
	public:
		PongMessage(void);
		virtual void vExecute(DiED::MessageTarget & MessageTarget);
		virtual Glib::ustring sGetString(void);
	};
	
	class ConnectionEstablishedMessage : public DiED::BasicMessage
	{
	public:
		ConnectionEstablishedMessage(void);
		ConnectionEstablishedMessage(const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort);
		virtual void vExecute(DiED::MessageTarget & MessageTarget);
		virtual Glib::ustring sGetString(void);
	private:
		Network::Value< DiED::clientid_t > m_ClientID;
		Network::Value< Network::address_t > m_ClientAddress;
		Network::Value< Network::port_t > m_ClientPort;
	};
	
	class ConnectionLostMessage : public DiED::BasicMessage
	{
	public:
		ConnectionLostMessage(void);
		ConnectionLostMessage(const DiED::clientid_t & ClientID);
		virtual void vExecute(DiED::MessageTarget & MessageTarget);
		virtual Glib::ustring sGetString(void);
	private:
		Network::Value< DiED::clientid_t > m_ClientID;
	};
	
	class TestMessage : public DiED::BasicMessage
	{
	public:
		TestMessage(void);
		virtual void vExecute(DiED::MessageTarget & MessageTarget);
		virtual Glib::ustring sGetString(void);
	private:
		Network::VectorValue< DiED::clientid_t > m_Values;
	public:
		void vAdd(clientid_t ClientID)
		{
			m_Values.push_back(ClientID);
		}
	};
	
	class EventMessage : public DiED::BasicMessage
	{
	public:
		EventMessage(const Network::BasicMessage::type_t & Type);
		EventMessage(const Network::BasicMessage::type_t & Type, const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID);
		virtual bool bIsEventMessage(void);
	private:
		Network::Value< DiED::clientid_t > m_CreatorID;
		Network::Value< DiED::messageid_t > m_EventID;
		Network::Value< DiED::clientid_t > m_LostClientID;
	};
	
	class InputMessage : public DiED::EventMessage
	{
	public:
		InputMessage(void);
		InputMessage(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID, const Glib::ustring & sText);
		virtual void vExecute(DiED::MessageTarget & MessageTarget);
		virtual Glib::ustring sGetString(void);
	private:
		Network::Value< Glib::ustring > m_Text;
	};
}

#endif
