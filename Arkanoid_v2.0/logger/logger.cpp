#include "logger.h"

std::fstream logger::log_file;
time_t logger::timer;
char logger::buffer[256];

logger::logger(){}

void logger::open_log() {
	log_file.open("logger/run_log.txt", std::fstream::out);
	time(&timer);
	struct tm buf;
	localtime_s(&buf, &timer);
	strftime(buffer, 256, "%Y %B %d %T", &buf);
	log_file << buffer << "\n";
	log_file.close();
}

void logger::log(std::stringstream& ss) {
	log_file.open("logger/run_log.txt", std::fstream::app);
	time(&timer);
	struct tm buf;
	localtime_s(&buf, &timer);
	strftime(buffer, 256, "%Y %B %d %T", &buf);
	log_file << buffer << "\t" << ss.str();
	log_file.close();
}

void logger::log(std::string& s) {
	log_file.open("logger/run_log.txt", std::fstream::app);
	time(&timer);
	struct tm buf;
	localtime_s(&buf, &timer);
	strftime(buffer, 256, "%Y %B %d %T", &buf);
	log_file << buffer << "\t" << s;
	log_file.close();
}

void logger::log(const char *cs) {
	log_file.open("logger/run_log.txt", std::fstream::app);
	time(&timer);
	struct tm buf;
	localtime_s(&buf, &timer);
	strftime(buffer, 256, "%Y %B %d %T", &buf);
	log_file << buffer << "\t" << cs;
	log_file.close();
}
