#ifndef NETWORK_SOCKET_H
#define NETWORK_SOCKET_H

#include <sigc++/sigc++.h>

#include <glibmm/iochannel.h>

namespace Network
{
	extern const int g_iInvalidSocket;
	
	typedef Glib::ustring address_t;
	typedef u_int16_t port_t;
	
	class Socket : virtual public sigc::trackable
	{
	public:
		Socket(void);
		Socket(int iSocket);
		virtual ~Socket(void);
		bool bIsOpen(void) const;
		int iGetError(void) const;
		void vClose(void);
		void vSetSocket(int iSocket);
		Network::address_t GetAddress(void);
		Network::port_t GetPort(void);
		
		// signals emitted after successfull connection establishing or disconnecting
		sigc::signal< void > OnConnected;
		sigc::signal< void > OnDisconnected;
	protected:
		void vMonitor(void);
		void vRequestOnOut(void);
		void vIgnoreOnOut(void);
		void vGetError(void);
		
		// virtual functions to make inherited classes act on socket events
		virtual void vOnIn(void);
		virtual void vOnOut(void);
		
		int m_iSocket;
		int m_iError;
		bool m_bOnDisconnected;
	private:
		bool bNotify(const Glib::IOCondition & Condition);
		Glib::RefPtr< Glib::IOSource > m_OSource;
	};
}

#endif
