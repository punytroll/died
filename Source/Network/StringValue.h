#ifndef STRINGVALUE_H
#define STRINGVALUE_H

#include "BasicValue.h"

namespace Network
{
	class StringValue : public Network::BasicValue
	{
	public:
		StringValue(void);
		StringValue(const Glib::ustring & Value);
		virtual size_t stGetSize(void) const;
		operator const Glib::ustring &(void) const;
		const Glib::ustring & Get(void) const;
		virtual void vReadFrom(boost::shared_ptr< Network::BasicReader > Reader);
		virtual void vWriteTo(boost::shared_ptr< Network::BasicWriter > Writer) const;
		virtual std::ostream & operator<<(std::ostream & OStream);
	protected:
		Glib::ustring m_String;
	};
}

#endif
