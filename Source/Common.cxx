#include "Common.h"

#include <errno.h>

#include "DiED/User.h"

std::string sErrorCodeToString(int iError)
{
	switch(iError)
	{
	case EACCES:
		return "EACCES";
	case EADDRINUSE:
		return "EADDRINUSE";
	case EAGAIN:
		return "EAGAIN";
	case EAFNOSUPPORT:
		return "EAFNOSUPPORT";
	case EALREADY:
		return "EALREADY";
	case EBADF:
		return "EBADF";
	case ECONNREFUSED:
		return "ECONNREFUSED";
	case EDEADLK:
		return "EDEADLK";
	case EFAULT:
		return "EFAULT";
	case EINTR:
		return "EINTR";
	case EINVAL:
		return "EINVAL";
	case EINPROGRESS:
		return "EINPROGRESS";
	case EISCONN:
		return "EISCONN";
	case EMFILE:
		return "EMFILE";
	case ENETUNREACH:
		return "ENETUNREACH";
	case ENFILE:
		return "ENFILE";
	case ENOBUFS:
		return "ENOBUFS";
	case ENOLCK:
		return "ENOLCK";
	case ENOMEM:
		return "ENOMEM";
	case ENOTCONN:
		return "ENOTCONN";
	case ENOTSOCK:
		return "ENOTSOCK";
	case EPERM:
		return "EPERM";
	case EPROTONOSUPPORT:
		return "EPROTONOSUPPORT";
	case ETIMEDOUT:
		return "ETIMEDOUT";
	default:
		return "Unknown";
	}
}

std::string sStatusToString(const DiED::clientstatus_t & Status)
{
	switch(Status)
	{
	case DiED::Connected:
		return "Connected";
	case DiED::Disconnected:
		return "Disconnected";
	case DiED::Connecting:
		return "Connecting";
	case DiED::Deleted:
		return "Deleted";
	default:
		return "Unknown";
	}
}
