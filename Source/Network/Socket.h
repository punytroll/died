#ifndef SOCKET_H
#define SOCKET_H

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
	protected:
		void vMonitor(void);
		void vRequestOnOut(void);
		void vIgnoreOnOut(void);
		void vGetError(void);
		virtual void vOnIn(void);
		virtual void vOnOut(void);
		int m_iError;
		int m_iSocket;
	private:
		bool bNotify(const Glib::IOCondition & Condition);
		Glib::RefPtr< Glib::IOSource > m_OSource;
	};
}

#endif
