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

#include "Common.h"

#include <errno.h>

#include <iostream>

#ifndef NODEBUG
bool g_bLogLevels[NumberOfLogLevels] = { false, false, false, false, false, false, false, false, false, false };
#endif

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

#ifndef NODEBUG
std::string sLogLevelToString(LogLevel LogLevel)
{
	switch(LogLevel)
	{
	case Error:
		return "Error";
	case Warning:
		return "Warning";
	case Info:
		return "Info";
	case Debug:
		return "Debug";
	case DebugGUI:
		return "DebugGUI";
	case DebugSystem:
		return "DebugSystem";
	case Object:
		return "Object";
	case Verbose:
		return "Verbose";
	case TODO:
		return "TODO";
	case DebugCurrent:
		return "DebugCurrent";
	case NumberOfLogLevels:
		{
			std::cerr << "Wrong LogLevel specified." << std::endl;
			
			return "";
		}
	}
	
	return "";
}
#endif
