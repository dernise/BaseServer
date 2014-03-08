#ifndef _LOGGING_H
#define _LOGGING_H

#include <fstream>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <iostream>
#include <string.h>
#include "Types.hpp"
#include <boost/asio.hpp>

#if defined (WIN32)
	#include <SDKDDKVer.h>
	#include <windows.h>
#endif

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define sLog Logging::getInstance()

enum LogLevel //Unimplemented: Log level
{
    LOG_LVL_MINIMAL = 0, 
    LOG_LVL_BASIC   = 1,
    LOG_LVL_DETAIL  = 2,
    LOG_LVL_DEBUG   = 3
};

enum Color
{
    BLACK,
    RED,
    GREEN,
    BROWN,
    BLUE,
    MAGENTA,
    CYAN,
    GREY,
    YELLOW,
    LRED,
    LGREEN,
    LBLUE,
    LMAGENTA,
    LCYAN,
    WHITE
};

const int Color_count = int(WHITE) + 1;

class Logging{
public:
	static Logging& getInstance()
    {
        static Logging instance;
        return instance;
    }
	void Initialize(const char* fileName, int logLevel, ...);
	void outString(const char* str, ...);
	void outError(const char* str, ...);
	void outWarning(const char* str, ...);
private:
	Logging(){}
    void SetColor(bool stdout_stream, Color color);
	std::string getCurrentTime();
	void ResetColor(bool stdout_stream);
	int m_logLevel;
	std::ofstream *m_file;
};

#endif