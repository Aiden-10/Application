#pragma once

#include <fstream>
#include <string>
#include <ctime>

class Log
{
public:
	static void Error(const std::string& errorMessage, const std::string& file, int line);

	static void Regular(const std::string& regularMessage, const std::string& file, int line);

	static void Clear();

private:

	static std::string getTime();

	static void writeLog(const std::string& message);

	// log file name
	static constexpr const char* LOG_FILE = "log.txt";
};

#define LOGERROR(message) Log::Error(message, __FILE__, __LINE__)

#define LOG(message) Log::Regular(message, __FILE__, __LINE__)