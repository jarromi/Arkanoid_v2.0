#include "logger.h"

std::fstream logger::log_file;

logger::logger(){}

void logger::open_log() {
	log_file.open("logger/run_log.txt", std::fstream::out);
	time_t timer;
	time(&timer);
	char buffer[256];
	ctime_s(buffer, 256, &timer);
	log(buffer);
}

void logger::log(std::stringstream& ss) {
	log_file << ss.str();
}

void logger::log(std::string& s) {
	log_file << s;
}

void logger::log(const char *cs) {
	log_file << cs;
}

void logger::close_log() {
	log_file.close();
}
