#ifndef NETWORK_VECTORVALUE_H
#define NETWORK_VECTORVALUE_H

#include <set>
#include <vector>

#include "BasicValue.h"

namespace Network
{
	template < typename Type >
	class VectorValue : public Network::BasicValue, public std::vector< Type >
	{
	public:
		VectorValue(void) :
			m_stSize(0)
		{
		}
		
		VectorValue(std::vector< Type > Vector) :
			std::vector< Type >(Vector),
			m_stSize(0)
		{
		}
		
		VectorValue(std::set< Type > Set) :
			m_stSize(0)
		{
			typename std::set< Type >::iterator iValue(Set.begin());
			
			while(iValue != Set.end())
			{
				push_back(*iValue);
				++iValue;
			}
		}
		
		virtual size_t stGetSize(void) const
		{
			return sizeof(size_t) + size() * sizeof(Type);
		}
		
		virtual void vReadFrom(boost::shared_ptr< Network::BasicReader > Reader)
		{
			if(m_stSize == 0)
			{
				if(Reader->bRead(m_stSize) == true)
				{
					if(m_stSize == 0)
					{
						vSetReady(true);
						
						return;
					}
				}
				else
				{
					return;
				}
			}
			while(size() < m_stSize)
			{
				Type Value;
				
				if(Reader->bRead(&Value, sizeof(Type)) == true)
				{
					push_back(Value);
				}
				else
				{
					return;
				}
			}
			vSetReady(true);
		}
		
		virtual void vWriteTo(boost::shared_ptr< Network::BasicWriter > Writer) const
		{
			// TODO: Problem of reentrance when the buffer was full last time
			Writer->bWrite(size());
			
			typename std::vector< Type >::const_iterator iValue(begin());
			
			while(iValue != end())
			{
				// Write returns false if nothing is written -> abort and wait for reentrance
				Writer->bWrite(&*iValue, sizeof(Type));
				++iValue;
			}
		}
	private:
		size_t m_stSize;
	};
}

#endif
