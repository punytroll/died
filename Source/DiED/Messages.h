#ifndef MESSAGES_H
#define MESSAGES_H

#include "DiED/BasicMessage.h"

namespace DiED
{
	enum
	{
		_NoMessage,
		_HelloMessage,
		_InputMessage,
		_PingMessage,
		_PongMessage
	};
	
	class NoMessage : public DiED::BasicMessage
	{
	public:
		NoMessage(void);
		virtual bool bIsReady(void) const;
		virtual void vReadFrom(Network::Stream & Stream);
		virtual void vExecute(DiED::Client & Client);
	protected:
		virtual void vWriteToInternal(Network::Stream & Stream) const;
	};
	
	class HelloMessage : public DiED::BasicMessage
	{
	public:
		HelloMessage(void);
		virtual bool bIsReady(void) const;
		virtual void vReadFrom(Network::Stream & Stream);
		virtual void vExecute(DiED::Client & Client);
	protected:
		virtual void vWriteToInternal(Network::Stream & Stream) const;
	};
	
	class InputMessage : public DiED::BasicMessage
	{
	public:
		InputMessage(void);
		InputMessage(const Glib::ustring & sString);
		virtual bool bIsReady(void) const;
		virtual void vReadFrom(Network::Stream & Stream);
		virtual void vExecute(DiED::Client & Client);
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
	protected:
		virtual void vWriteToInternal(Network::Stream & Stream) const;
	};
}

#endif
