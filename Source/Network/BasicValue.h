#ifndef BASICVALUE_H
#define BASICVALUE_H

#include <boost/shared_ptr.hpp>

#include "BasicReader.h"
#include "BasicWriter.h"

namespace Network
{
	class BasicValue
	{
	public:
		BasicValue(void) :
			m_bReady(false)
		{
		}
		
		bool bIsReady(void)
		{
			return m_bReady;
		}
		
		virtual void vReadFrom(boost::shared_ptr< Network::BasicReader > Reader) = 0;
		virtual void vWriteTo(boost::shared_ptr< Network::BasicWriter > Writer) const = 0;
	protected:
		bool m_bReady;
	};
}

#endif
