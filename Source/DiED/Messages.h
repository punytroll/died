#ifndef MESSAGES_H
#define MESSAGES_H

#include <Network/StringValue.h>
#include <Network/VectorValue.h>

#include "BasicMessage.h"
#include "EventAction.h"

namespace DiED
{
	enum MessageTypes
	{
		_NoMessage,
		_ConnectionRequestMessage,
		_ConnectionAcceptMessage,
		_KnownClientsMessage,
		_ClientsRegisteredMessage,
		_ConnectionEstablishedMessage,
		_ConnectionLostMessage,
		_StatusConfirmMessage,
		_PingMessage,
		_PongMessage,
		_EventReceivedMessage,
		_InsertEvent,
		_DeleteEvent,
		_PositionEvent,
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
		virtual bool bRequiresConfirmation(void);
		virtual bool bIsConfirmedBy(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters);
		virtual Glib::ustring sGetString(void);
		virtual bool bOnTimeout(DiED::MessageTarget * pMessageTarget);
	private:
		Network::Value< DiED::clientid_t > m_ClientID;
		Network::Value< Network::port_t > m_Port;
	};
	
	class ConnectionAcceptMessage : public DiED::BasicMessage
	{
	public:
		ConnectionAcceptMessage(void);
		ConnectionAcceptMessage(const DiED::clientid_t & AccepterClientID, const DiED::clientid_t & RequesterClientID);
		virtual void vExecute(DiED::MessageTarget & MessageTarget);
		virtual boost::shared_ptr< DiED::ConfirmationParameters > GetConfirmationParameters(void);
		virtual Glib::ustring sGetString(void);
	private:
		Network::Value< DiED::clientid_t > m_AccepterClientID;
		Network::Value< DiED::clientid_t > m_RequesterClientID;
	};
	
	class KnownClientsMessage : public DiED::BasicMessage
	{
	public:
		KnownClientsMessage(void);
		KnownClientsMessage(DiED::messageid_t MessageID, std::vector< DiED::ClientInfo > ClientInfos);
		virtual void vExecute(DiED::MessageTarget & MessageTarget);
		virtual bool bRequiresConfirmation(void);
		virtual bool bIsConfirmedBy(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters);
		virtual boost::shared_ptr< DiED::ConfirmationParameters > GetConfirmationParameters(void);
		virtual Glib::ustring sGetString(void);
		virtual bool bOnTimeout(DiED::MessageTarget * pMessageTarget);
	private:
		Network::Value< DiED::messageid_t > m_MessageID;
		Network::VectorValue< DiED::ClientInfo > m_ClientInfos;
	};
	
	class ClientsRegisteredMessage : public DiED::BasicMessage
	{
	public:
		ClientsRegisteredMessage(void);
		ClientsRegisteredMessage(DiED::messageid_t MessageID);
		virtual void vExecute(DiED::MessageTarget & MessageTarget);
		virtual boost::shared_ptr< DiED::ConfirmationParameters > GetConfirmationParameters(void);
		virtual Glib::ustring sGetString(void);
	private:
		Network::Value< DiED::messageid_t > m_MessageID;
	};
	
	class ConnectionEstablishedMessage : public DiED::BasicMessage
	{
	public:
		ConnectionEstablishedMessage(void);
		ConnectionEstablishedMessage(const DiED::messageid_t & MessageID, const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort);
		virtual void vExecute(DiED::MessageTarget & MessageTarget);
		virtual bool bRequiresConfirmation(void);
		virtual bool bIsConfirmedBy(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters);
		virtual Glib::ustring sGetString(void);
		virtual bool bOnTimeout(DiED::MessageTarget * pMessageTarget);
	private:
		Network::Value< DiED::messageid_t > m_MessageID;
		Network::Value< DiED::clientid_t > m_ClientID;
		Network::StringValue m_ClientAddress;
		Network::Value< Network::port_t > m_ClientPort;
	};
	
	class ConnectionLostMessage : public DiED::BasicMessage
	{
	public:
		ConnectionLostMessage(void);
		ConnectionLostMessage(const DiED::messageid_t & MessageID, const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort);
		virtual void vExecute(DiED::MessageTarget & MessageTarget);
		virtual bool bRequiresConfirmation(void);
		virtual bool bIsConfirmedBy(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters);
		virtual Glib::ustring sGetString(void);
		virtual bool bOnTimeout(DiED::MessageTarget * pMessageTarget);
	private:
		Network::Value< DiED::messageid_t > m_MessageID;
		Network::Value< DiED::clientid_t > m_ClientID;
		Network::StringValue m_ClientAddress;
		Network::Value< Network::port_t > m_ClientPort;
	};
	
	class StatusConfirmMessage : public DiED::BasicMessage
	{
	public:
		StatusConfirmMessage(void);
		StatusConfirmMessage(const DiED::messageid_t & MessageID);
		virtual void vExecute(DiED::MessageTarget & MessageTarget);
		virtual boost::shared_ptr< DiED::ConfirmationParameters > GetConfirmationParameters(void);
		virtual Glib::ustring sGetString(void);
	private:
		Network::Value< DiED::messageid_t > m_MessageID;
	};
	
	class PingMessage : public DiED::BasicMessage
	{
	public:
		PingMessage(void);
		PingMessage(const DiED::messageid_t & PingID);
		virtual void vExecute(DiED::MessageTarget & MessageTarget);
		virtual Glib::ustring sGetString(void);
		virtual bool bRequiresConfirmation(void);
		virtual bool bIsConfirmedBy(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters);
		virtual bool bOnTimeout(DiED::MessageTarget * pMessageTarget);
	private:
		Network::Value< DiED::messageid_t > m_PingID;
	};
	
	class PongMessage : public DiED::BasicMessage
	{
	public:
		PongMessage(void);
		PongMessage(const DiED::messageid_t & PingID);
		virtual void vExecute(DiED::MessageTarget & MessageTarget);
		virtual Glib::ustring sGetString(void);
		virtual boost::shared_ptr< DiED::ConfirmationParameters > GetConfirmationParameters(void);
	private:
		Network::Value< DiED::messageid_t > m_PingID;
	};
	
	class EventReceivedMessage : public DiED::BasicMessage
	{
	public:
		EventReceivedMessage(void);
		EventReceivedMessage(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID);
		virtual boost::shared_ptr< DiED::ConfirmationParameters > GetConfirmationParameters(void);
		virtual void vExecute(DiED::MessageTarget & MessageTarget);
		virtual Glib::ustring sGetString(void);
	private:
		Network::Value< DiED::clientid_t > m_CreatorID;
		Network::Value< DiED::messageid_t > m_EventID;
	};
	
	class EventMessage : public DiED::BasicMessage
	{
	public:
		EventMessage(const Network::BasicMessage::type_t & Type);
		EventMessage(const Network::BasicMessage::type_t & Type, const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID);
		virtual bool bIsConfirmedBy(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters);
		void vSetLostClientID(const DiED::clientid_t & ClientID);
		virtual void vExecute(DiED::MessageTarget & MessageTarget);
		virtual bool bIsEventMessage(void);
		virtual bool bRequiresConfirmation(void);
		virtual Glib::ustring sGetString(void);
		virtual bool bOnTimeout(DiED::MessageTarget * pMessageTarget);
	protected:
		virtual boost::shared_ptr< DiED::EventAction > GetAction(void) = 0;
		Network::Value< DiED::clientid_t > m_CreatorID;
		Network::Value< DiED::messageid_t > m_EventID;
		Network::Value< DiED::clientid_t > m_LostClientID;
	};
	
	class InsertEvent : public DiED::EventMessage
	{
	public:
		InsertEvent(void);
		InsertEvent(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID, const Glib::ustring & sText);
		virtual Glib::ustring sGetString(void);
		virtual boost::shared_ptr< DiED::BasicMessage > Clone(void);
	protected:
		virtual boost::shared_ptr< DiED::EventAction > GetAction(void);
	private:
		Network::StringValue m_Text;
	};
	
	class DeleteEvent : public DiED::EventMessage
	{
	public:
		DeleteEvent(void);
		DeleteEvent(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID, int iLineRelative, int iCharacterRelative, int iLineAbsolute, int iCharacterAbsolute);
		virtual Glib::ustring sGetString(void);
		virtual boost::shared_ptr< DiED::BasicMessage > Clone(void);
	protected:
		virtual boost::shared_ptr< DiED::EventAction > GetAction(void);
	private:
		Network::Value< int > m_LineRelative;
		Network::Value< int > m_CharacterRelative;
		Network::Value< int > m_LineAbsolute;
		Network::Value< int > m_CharacterAbsolute;
	};
	
	class PositionEvent : public DiED::EventMessage
	{
	public:
		PositionEvent(void);
		PositionEvent(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID, int iLineRelative, int iCharacterRelative, int iLineAbsolute, int iCharacterAbsolute);
		virtual Glib::ustring sGetString(void);
		virtual boost::shared_ptr< DiED::BasicMessage > Clone(void);
	protected:
		virtual boost::shared_ptr< DiED::EventAction > GetAction(void);
	private:
		Network::Value< int > m_LineRelative;
		Network::Value< int > m_CharacterRelative;
		Network::Value< int > m_LineAbsolute;
		Network::Value< int > m_CharacterAbsolute;
	};
}

#endif
