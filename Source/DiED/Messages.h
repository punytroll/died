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
		virtual bool bIsReady(void) const;
		virtual void vReadFrom(Network::Stream & Stream);
		virtual void vExecute(DiED::Client & Client);
		virtual Glib::ustring sGetString(void);
	protected:
		virtual void vWriteToInternal(Network::Stream & Stream) const;
	};
	
	class ConnectionRequestMessage : public DiED::BasicMessage
	{
	public:
		ConnectionRequestMessage(void);
		ConnectionRequestMessage(const DiED::clientid_t & ClientID, const Network::port_t & Port);
		virtual bool bIsReady(void) const;
		virtual void vReadFrom(Network::Stream & Stream);
		virtual void vExecute(DiED::Client & Client);
		virtual Glib::ustring sGetString(void);
	protected:
		virtual void vWriteToInternal(Network::Stream & Stream) const;
	private:
		Network::Value< DiED::clientid_t > m_ClientID;
		Network::Value< Network::port_t > m_Port;
	};
	
	class ConnectionAcceptMessage : public DiED::BasicMessage
	{
	public:
		ConnectionAcceptMessage(void);
		ConnectionAcceptMessage(const DiED::clientid_t & RemoteClientID, const DiED::clientid_t & LocalClientID);
		virtual bool bIsReady(void) const;
		virtual void vReadFrom(Network::Stream & Stream);
		virtual void vExecute(DiED::Client & Client);
		virtual Glib::ustring sGetString(void);
	protected:
		virtual void vWriteToInternal(Network::Stream & Stream) const;
	private:
		Network::Value< DiED::clientid_t > m_RemoteClientID;
		Network::Value< DiED::clientid_t > m_LocalClientID;
	};
	
	class InputMessage : public DiED::BasicMessage
	{
	public:
		InputMessage(void);
		InputMessage(const Glib::ustring & sString);
		virtual bool bIsReady(void) const;
		virtual void vReadFrom(Network::Stream & Stream);
		virtual void vExecute(DiED::Client & Client);
		virtual Glib::ustring sGetString(void);
	protected:
		virtual void vWriteToInternal(Network::Stream & Stream) const;
	private:
		Network::Value< Glib::ustring > m_String;
	};
	
	class PingMessage : public DiED::BasicMessage
	{
	public:
		PingMessage(void);
		virtual bool bIsReady(void) const;
		virtual void vReadFrom(Network::Stream & Stream);
		virtual void vExecute(DiED::Client & Client);
		virtual Glib::ustring sGetString(void);
	protected:
		virtual void vWriteToInternal(Network::Stream & Stream) const;
	};
	
	class PongMessage : public DiED::BasicMessage
	{
	public:
		PongMessage(void);
		virtual bool bIsReady(void) const;
		virtual void vReadFrom(Network::Stream & Stream);
		virtual void vExecute(DiED::Client & Client);
		virtual Glib::ustring sGetString(void);
	protected:
		virtual void vWriteToInternal(Network::Stream & Stream) const;
	};
	
	class ConnectionEstablishedMessage : public DiED::BasicMessage
	{
	public:
		ConnectionEstablishedMessage(void);
		ConnectionEstablishedMessage(const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort);
		virtual bool bIsReady(void) const;
		virtual void vReadFrom(Network::Stream & Stream);
		virtual void vExecute(DiED::Client & Client);
		virtual Glib::ustring sGetString(void);
	protected:
		virtual void vWriteToInternal(Network::Stream & Stream) const;
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
		virtual bool bIsReady(void) const;
		virtual void vReadFrom(Network::Stream & Stream);
		virtual void vExecute(DiED::Client & Client);
		virtual Glib::ustring sGetString(void);
	protected:
		virtual void vWriteToInternal(Network::Stream & Stream) const;
	private:
		Network::Value< DiED::clientid_t > m_ClientID;
	};
	
	class TestMessage : public DiED::BasicMessage
	{
	public:
		TestMessage(void);
		virtual bool bIsReady(void) const;
		virtual void vReadFrom(Network::Stream & Stream);
		virtual void vExecute(DiED::Client & Client);
		virtual Glib::ustring sGetString(void);
	protected:
		virtual void vWriteToInternal(Network::Stream & Stream) const;
	private:
		Network::VectorValue< DiED::clientid_t > m_Values;
	public:
		void vAdd(clientid_t ClientID)
		{
			m_Values.push_back(ClientID);
		}
	};
}

#endif
