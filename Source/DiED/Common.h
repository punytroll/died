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
		Undefined,
		Local,
		Connecting,
		Connected,
		Disconnected
	};
	
	typedef u_int32_t messageid_t;
	typedef u_int32_t clientid_t;
	typedef std::map< Glib::ustring, boost::any > ConfirmationParameters;
	
	const unsigned int g_uiDefaultReconnectTimeoutInterval = 4000;
	
	std::string sStatusToString(const DiED::clientstatus_t & iStatus);
}

#endif
