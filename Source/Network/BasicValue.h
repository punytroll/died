#ifndef NETWORK_BASICVALUE_H
#define NETWORK_BASICVALUE_H

#include <boost/shared_ptr.hpp>

#include <sigc++/signal.h>

#include "BasicReader.h"
#include "BasicWriter.h"

namespace Network
{
	class BasicValue
	{
	public:
		BasicValue(void);
		virtual ~BasicValue(void);
		bool bIsReady(void) const;
		virtual size_t stGetSize(void) const;
		virtual void vReadFrom(boost::shared_ptr< Network::BasicReader > Reader) = 0;
		virtual void vWriteTo(boost::shared_ptr< Network::BasicWriter > Writer) const = 0;
		
		sigc::signal< void > Ready;
	protected:
		void vSetReady(bool bReady);
	private:
		bool m_bReady;
	};
}

#endif
