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

#ifndef COMMON_H
#define COMMON_H

#include <map>
#include <string>

#include <boost/any.hpp>

#include <glibmm/ustring.h>

//~ #define NODEBUG

#ifndef NODEBUG

enum LogLevel
{
	Error,
	Warning,
	Info,
	Debug,
	DebugGUI,
	DebugSystem,
	Object,
	Verbose,
	TODO,
	DebugCurrent,
	NumberOfLogLevels
};

extern bool g_bLogLevels[NumberOfLogLevels];

#include <iomanip>

#define LOG(LEVEL, SCOPE, MESSAGE) \
	if(g_bLogLevels[(LEVEL)] == true) { std::cout << "[" << std::setw(7) << sLogLevelToString((LEVEL)) << "] " << (SCOPE) << ": " << MESSAGE << std::endl; }

#define LOG_NO_NL(LEVEL, SCOPE, MESSAGE) \
	if(g_bLogLevels[(LEVEL)] == true) { std::cout << "[" << std::setw(7) << sLogLevelToString((LEVEL)) << "] " << (SCOPE) << ": " << MESSAGE; }

#define LOG_PURE(LEVEL, SCOPE, MESSAGE) \
	if(g_bLogLevels[(LEVEL)] == true) { std::cout << MESSAGE; }

std::string sLogLevelToString(LogLevel LogLevel);

#else

// define all logging to nothing :)
#define LOG(LEVEL, SCOPE, MESSAGE)
#define LOG_NO_NL(LEVEL, SCOPE, MESSAGE)
#define LOG_PURE(LEVEL, SCOPE, MESSAGE)

#endif

std::string sErrorCodeToString(int iError);

#endif
