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
public:
	static void open_log();
	static void log(std::stringstream&);
	static void log(std::string&);
	static void log(const char*);
	static void close_log();
};

#endif
