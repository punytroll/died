#ifndef COMMON_H
#define COMMON_H

#include <map>
#include <string>

#include <boost/any.hpp>

#include <glibmm/ustring.h>

namespace DiED
{
	enum clientstatus_t
	{
		Connecting,
		Connected,
		Disconnected,
		Deleted
	};
	
	typedef u_int32_t messageid_t;
	typedef u_int32_t clientid_t;
	typedef std::map< Glib::ustring, boost::any > ConfirmationParameters;
	
	std::string sStatusToString(const DiED::clientstatus_t & iStatus);
}

std::string sErrorCodeToString(int iError);

#endif
