#ifndef COMMON_H
#define COMMON_H

#include <string>

namespace DiED
{
	typedef u_int32_t messageid_t;
}

std::string sErrorCodeToString(int iError);
std::string sStatusToString(int iStatus);

#endif
