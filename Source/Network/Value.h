#ifndef VALUE_H
#define VALUE_H

#include "BasicValue.h"

namespace Network
{
	template < typename Type >
	class Value : public Network::BasicValue
	{
	public:
		Value(void) :
			m_Value()
		{
		}
		
		operator const Type &(void)
		{
			return m_Value;
		}
		
		virtual void vReadFrom(boost::shared_ptr< Network::BasicReader > ReaderPtr)
		{
			Network::BasicReader & Reader(*ReaderPtr);
			
			m_bReady = Reader.bRead(m_Value);
		}
	protected:
		Type m_Value;
	};
}

#endif
