#ifndef _LOGGER_
#define _LOGGER_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>

class logger {
	static std::fstream log_file;
	logger();
	static time_t timer;
	static char buffer[256];
public:
	static void open_log();
	static void log(std::stringstream&);
	static void log(std::string&);
	static void log(const char*);
};

#endif
