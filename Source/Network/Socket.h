#ifndef SOCKET_H
#define SOCKET_H

#include <sigc++/sigc++.h>

#include <glibmm/iochannel.h>

namespace Network
{
	extern const int g_iInvalidSocket;
	
	class Socket : virtual public sigc::trackable
	{
	public:
		Socket(void);
		Socket(int iSocket);
		virtual ~Socket(void);
		bool bIsOpen(void) const;
		int iGetError(void) const;
	protected:
		void vMonitor(void);
		void vGetError(void);
		virtual void vOnIn(void);
		int m_iSocket;
	private:
		bool bNotify(const Glib::IOCondition & Condition);
		int m_iError;
	};
}

#endif
