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
