#include <string>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <filesystem>

#pragma once

class VTAutoLogger
{
public:
	std::string logFile;

	VTAutoLogger();
	std::string getCurrentTimestamp();
	void writeLog(std::string message);
	void writeLog(int message);
};
