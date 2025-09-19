#include "Logger.h"

VTAutoLogger::VTAutoLogger() {
	logFile = "C:\\Users\\nguye\\source\\debugging\\vtauto.log";
}

std::string VTAutoLogger::getCurrentTimestamp() {
	//// Get current time as a time_point
	auto now = std::chrono::system_clock::now();
	//// Convert to time_t for formatting
	std::time_t now_c = std::chrono::system_clock::to_time_t(now);

	//// Convert to local time
	std::tm local_tm;
	localtime_s(&local_tm, &now_c);

	//// Use stringstream to format the date and time
	std::ostringstream oss;
	oss << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S");

	return oss.str();

	return "";
}

void VTAutoLogger::writeLog(std::string message) {
	FILE* f;
	if (fopen_s(&f, this->logFile.c_str(), "a+") == 0) {
		fprintf(f, "%s - %s\n", this->getCurrentTimestamp().c_str(), message.c_str());
		fclose(f);
	}
}

void VTAutoLogger::writeLog(int message)
{
	FILE* f;
	if (fopen_s(&f, this->logFile.c_str(), "a+") == 0) {
		fprintf(f, "%s - %s\n", this->getCurrentTimestamp().c_str(), std::to_string(message).c_str());
		fclose(f);
	}
}
