#include "StringValue.h"

Network::StringValue::StringValue(void) :
	m_String()
{
}

Network::StringValue::StringValue(const Glib::ustring & String) :
	m_String(String)
{
}

size_t Network::StringValue::stGetSize(void) const
{
	return m_String.bytes() + 1;
}

void Network::StringValue::vReadFrom(boost::shared_ptr< Network::BasicReader > Reader)
{
	vSetReady(Reader->bRead(m_String));
}

void Network::StringValue::vWriteTo(boost::shared_ptr< Network::BasicWriter > Writer) const
{
	Writer->bWrite(m_String);
}

Network::StringValue::operator const Glib::ustring &(void) const
{
	return m_String;
}

const Glib::ustring & Network::StringValue::Get(void) const
{
	return m_String;
}
