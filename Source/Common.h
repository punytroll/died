#ifndef COMMON_H
#define COMMON_H

#include <map>
#include <string>

#include <boost/any.hpp>

#include <glibmm/ustring.h>

namespace DiED
{
	typedef u_int32_t messageid_t;
	typedef std::map< Glib::ustring, boost::any > ConfirmationParameters;
}

std::string sErrorCodeToString(int iError);
std::string sStatusToString(int iStatus);

#endif
