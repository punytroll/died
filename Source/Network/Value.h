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
		
		Value(const Type & Value) :
			m_Value(Value)
		{
		}
		
		operator const Type &(void) const
		{
			return m_Value;
		}
		
		virtual Glib::ustring sGetString(void) const
		{
			return typeid(Type).name();
		}
		
		virtual void vReadFrom(boost::shared_ptr< Network::BasicReader > Reader)
		{
			vSetReady(Reader->bRead(m_Value));
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
