#include "a_log.h"


#include <filesystem>

void Log::Error(const std::string& errorMessage, const std::string& file, int line)
{
	std::string filename = std::filesystem::path(file).filename().string();

	std::string message = "[ERROR!] [" + getTime() + "]"
		+ "[" + filename + "][" + std::to_string(line) + "]: "
		+ errorMessage;

	writeLog(message);
}

void Log::Regular(const std::string& regularMessage, const std::string& file, int line)
{
	std::string filename = std::filesystem::path(file).filename().string();

	std::string message = "[" + getTime() + "]"
		+ "[" + filename + "][" + std::to_string(line) + "]: "
		+ regularMessage;

	writeLog(message);
}

std::string Log::getTime()
{
	char time[100];
	std::time_t currentTime = std::time(nullptr);
	std::tm timeInfo;

	localtime_s(&timeInfo, &currentTime);
	std::strftime(time, sizeof(time), "%Y-%m-%d %H:%M:%S", &timeInfo);

	return std::string(time);
}

void Log::writeLog(const std::string& message)
{
	std::ofstream File;

	File.open(LOG_FILE, std::ios::app);

	if (File.is_open()) {

		File << message << std::endl;

		File.close();
	}
	else {
		// hmm failed
		//MessageBox(NULL, "Error", "Error: Error Opening file log.txt", MB_OK | MB_ICONWARNING);
	}
}

void Log::Clear() {
	std::ofstream logFile;

	logFile.open(LOG_FILE, std::ios::trunc);

	if (logFile.is_open()) {

		logFile.close();
	}
	else {
		//MessageBox(NULL, "Error", "Error: Error Opening file log.txt", MB_OK | MB_ICONWARNING);
	}
}