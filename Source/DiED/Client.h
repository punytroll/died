#ifndef DIED_CLIENT_H
#define DIED_CLIENT_H

#include <list>

#include <Network/MessageStream.h>
#include <Common.h>

#include "ActionTarget.h"
#include "InternalEnvironment.h"
#include "Messages.h"
#include "MessageTarget.h"

namespace DiED
{
	class Client : public DiED::User, public DiED::MessageTarget, public DiED::ActionTarget
	{
	public:
		// constructor and destructor
		Client(DiED::InternalEnvironment & InternalEnvironment);
		virtual ~Client(void);
		
		// socket
		void vSetMessageStream(boost::shared_ptr< Network::MessageStream > MessageStream);
		boost::shared_ptr< Network::MessageStream > GetMessageStream(void);
		void vNewReconnectTimeout(sigc::slot< bool > TimeoutSlot);
		
		// connection stuff
		Network::port_t GetPort(void);
		Network::address_t GetAddress(void);
		void vSetPort(const Network::port_t & Port);
		
		// client info
		void vSetEventCounter(const DiED::messageid_t & EventCounter);
		DiED::clientid_t GetEventCounter(void) const;
		DiED::ClientInfo GetClientInfo(void) const;
		
		// status
		virtual void vSetStatus(const DiED::clientid_t & ClientID, DiED::clientstatus_t Status);
		
		// sending messages
		void vSend(boost::shared_ptr< DiED::BasicMessage > Message);
		void vConnectionRequest(const DiED::clientid_t & ClientID, const Network::port_t & Port);
		void vConnectionAccept(const DiED::clientid_t & AccepterClientID, const DiED::clientid_t & RequesterClientID);
		void vSessionSnapshot(bool bDocumentValid, const Glib::ustring & sDocument, bool bAskForSessionSnapshot = false);
		void vClientsRegistered(const DiED::messageid_t & MessageID);
		void vConnectionEstablished(const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort);
		void vConnectionLost(const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort);
		void vPing(sigc::slot< void > PongSlot, sigc::slot< void > PongTimeoutSlot);
		void vPing(sigc::slot< void > PongTimeoutSlot);
		void vPing(void);
		
		// sending events
		void vInsert(const Glib::ustring & sText, const DiED::messageid_t & EventID);
		void vDelete(int iLineRelative, int iCharacterRelative, int iLineAbsolute, int iCharacterAbsolute, const DiED::messageid_t & EventID);
		void vPosition(int iLineRelative, int iCharacterRelative, int iLineAbsolute, int iCharacterAbsolute, const DiED::messageid_t & EventID);
		void vLogOutNotification(const DiED::messageid_t & EventID);
		
		// eventcounter
		DiED::messageid_t GetNextEventCounter(void);
		
		// message handling
		void vProcessEventQueue(void);
		virtual void vExecuteTopMessage(void);
		
		//signals
		sigc::signal< void > MessageStreamSet;
		sigc::signal< void, boost::weak_ptr< DiED::BasicMessage > > MessageQueued;
		sigc::signal< void, boost::weak_ptr< DiED::BasicMessage > > MessageSent;
	protected:
		virtual void vOnMessageReady(void);
		virtual void vOnMessageBegin(void);
		virtual void vOnMessageExecuted(void);
		void vOnDisconnected(void);
		void vBytesSent(size_t stSize);
		
		// message handler
		virtual void vHandleConnectionRequest(const DiED::clientid_t & ClientID, const Network::port_t & Port);
		virtual void vHandleConnectionAccept(const DiED::clientid_t & AccepterClientID, const DiED::clientid_t & RequesterClientID);
		virtual void vHandleSessionSnapshot(const DiED::messageid_t & MessageID, const std::vector< DiED::ClientInfo > & ClientInfos, bool bDocumentValid, const Glib::ustring & sDocument);
		virtual void vHandleClientsRegistered(const DiED::messageid_t & MessageID);
		virtual void vHandleConnectionEstablished(const DiED::messageid_t & MessageID, const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort);
		virtual void vHandleConnectionLost(const DiED::messageid_t & MessageID, const DiED::clientid_t & ClientID, const Network::address_t & ClientAddress, const Network::port_t & ClientPort);
		virtual void vHandleStatusConfirm(const DiED::messageid_t & MessageID);
		virtual void vHandlePing(const DiED::messageid_t & PingID);
		virtual void vHandlePong(const DiED::messageid_t & PingID);
		virtual void vHandleEvent(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID, const DiED::clientid_t & LostClientID, boost::shared_ptr< DiED::EventAction > EventAction);
		virtual void vHandleEventReceived(const DiED::clientid_t & CreatorID, const DiED::messageid_t & EventID);
		
		// action handler
		virtual void vHandleInsert(const Glib::ustring & sString);
		virtual void vHandleDelete(int iLineRelative, int iCharacterRelative, int iLineAbsolute, int iCharacterAbsolute);
		virtual void vHandlePosition(int iLineRelative, int iCharacterRelative, int iLineAbsolute, int iCharacterAbsolute);
		virtual void vHandleLogOutNotification(void);
		
		// timeout callbacks
		virtual void vHandleConnectionRequestConfirmationTimeout(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters);
		virtual void vHandleSessionSnapshotConfirmationTimeout(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters);
		virtual void vHandleConnectionEstablishedConfirmationTimeout(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters);
		virtual void vHandleConnectionLostConfirmationTimeout(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters);
		virtual void vHandlePingConfirmationTimeout(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters);
		virtual void vHandleEventConfirmationTimeout(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters);
	private:
		DiED::InternalEnvironment & m_InternalEnvironment;
	protected:
		boost::shared_ptr< Network::MessageStream > m_MessageStream;
	private:
		void vHandleAnswer(void);
		void vAddEventAction(const DiED::messageid_t & EventID, boost::shared_ptr< DiED::EventAction > EventAction);
		
		Network::address_t m_Address;
		Network::port_t m_Port;
		unsigned int m_uiReconnectTimeoutInterval;
		sigc::connection m_BytesSentConnection;
		sigc::connection m_MessageBeginConnection;
		sigc::connection m_MessageReadyConnection;
		sigc::connection m_OnDisconnectedConnection;
		DiED::messageid_t m_StatusMessageCounter;
		DiED::messageid_t m_EventCounter;
		size_t m_stBytesSent;
		
		struct WaitingMessage
		{
			boost::shared_ptr< DiED::BasicMessage > m_Message;
			boost::shared_ptr< sigc::signal< void > > m_TimeoutSignal;
		};
		
		DiED::Client::WaitingMessage RemoveWaitingMessage(boost::shared_ptr< DiED::ConfirmationParameters > ConfirmationParameters);
		
		// message queues
		std::deque< WaitingMessage > m_AwaitingConfirmationQueue;
		std::deque< boost::shared_ptr< DiED::EventMessage > > m_EventQueue;
		std::deque< boost::shared_ptr< DiED::BasicMessage > > m_QueuedQueue;
		std::map< DiED::messageid_t, boost::shared_ptr< DiED::EventAction > > m_ActionBuffer;
		std::map< DiED::messageid_t, boost::shared_ptr< sigc::signal< void > > > m_PongSignals;
	};
}

#endif
