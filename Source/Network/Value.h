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
		
		Value(Type & Value) :
			m_Value(Value)
		{
		}
		
		operator const Type &(void)
		{
			return m_Value;
		}
		
		virtual void vReadFrom(boost::shared_ptr< Network::BasicReader > Reader)
		{
			m_bReady = Reader->bRead(m_Value);
		}
		
		virtual void vWriteTo(boost::shared_ptr< Network::BasicWriter > Writer) const
		{
			Writer->bWrite(m_Value);
		}
	protected:
		Type m_Value;
	};
}

#endif
