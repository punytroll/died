/* DiED - A distributed Editor.
 * Copyright (C) 2005 Hagen Möbius & Aram Altschudjian
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

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
			return sizeof(size_t) + std::vector< Type >::size() * sizeof(Type);
		}
		
		virtual void vReadFrom(Network::BasicReader & Reader)
		{
			if(m_stSize == 0)
			{
				if(Reader.bRead(m_stSize) == true)
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
			while(std::vector< Type >::size() < m_stSize)
			{
				Type Value;
				
				if(Reader.bRead(&Value, sizeof(Type)) == true)
				{
					this->push_back(Value);
				}
				else
				{
					return;
				}
			}
			vSetReady(true);
		}
		
		virtual void vWriteTo(Network::BasicWriter & Writer) const
		{
			// TODO: Problem of reentrance when the buffer was full last time
			Writer.bWrite(static_cast< u_int32_t >(std::vector< Type >::size()));
			
			typename std::vector< Type >::const_iterator iValue(std::vector< Type >::begin());
			
			while(iValue != std::vector< Type >::end())
			{
				// Write returns false if nothing is written -> abort and wait for reentrance
				Writer.bWrite(&*iValue, sizeof(Type));
				++iValue;
			}
		}
	private:
		u_int32_t m_stSize;
	};
}

#endif
