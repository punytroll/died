#ifndef DIED_COMMON_H
#define DIED_COMMON_H

#include <sys/types.h>

#include <map>

#include <boost/any.hpp>

#include <glibmm/ustring.h>

#include "../Common.h"

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

#endif
