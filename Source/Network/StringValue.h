#ifndef NETWORK_STRINGVALUE_H
#define NETWORK_STRINGVALUE_H

#include "BasicValue.h"

namespace Network
{
	class StringValue : public Network::BasicValue
	{
	public:
		StringValue(void);
		StringValue(const Glib::ustring & Value);
		virtual size_t stGetSize(void) const;
		virtual void vReadFrom(boost::shared_ptr< Network::BasicReader > Reader);
		virtual void vWriteTo(boost::shared_ptr< Network::BasicWriter > Writer) const;
		operator const Glib::ustring &(void) const;
		const Glib::ustring & Get(void) const;
	protected:
		Glib::ustring m_String;
	};
}

#endif
