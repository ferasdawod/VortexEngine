#include "Logger.h"

#include "Debug.h"
#include <ctime>

#include <spdlog/spdlog.h>

namespace spd = spdlog;

Logger::Logger()
{
#if defined(DEBUG) || defined(_DEBUG)
	//std::string fileName = GetLocalTime();
	//
	//for (auto itr = fileName.begin(); itr != fileName.end(); ++itr)
	//{
	//	if (*itr == ':')
	//		*itr = '_';
	//}
	//
	//fileName = "Logs\\" + fileName + ".txt";
	//_outStream.open(fileName, std::ios::app | std::ios::out);
	//if (_outStream.bad())
	//{
	//	OutputDebugString("Failed to initialize the logger system\n");
	//}
	//else
	//{
	//	Log("Began Logging");
	//}

	const auto queueSize = 4096;
	spd::set_async_mode(queueSize, spdlog::async_overflow_policy::block_retry);
	std::vector<spdlog::sink_ptr> sinks;
	sinks.push_back(std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>());
	//sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>("log", "txt"));
	auto _logger = std::make_shared<spdlog::logger>("engine", begin(sinks), end(sinks));
	// hello
	_logger->info("Hello");
	_logger->error("OMG Error NOW");
#endif
}

Logger::~Logger()
{
//#if defined(DEBUG) || defined(_DEBUG)
//	Log("Finished Logging");
//	Log("==========================================");
//	_outStream.close();
//#endif
}

void Logger::Log(const std::string& msg)
{
//#if defined(DEBUG) || defined(_DEBUG)
//	auto currentTime = GetLocalTime();
//	_outStream << "[" << currentTime << "] : " << msg << std::endl;
//#endif
}

std::string GetLocalTime()
{
#if defined(DEBUG) || defined(_DEBUG)
	auto time = std::time(NULL);
	tm localTm;
	localtime_s(&localTm, &time);
	char buffer[50];
	asctime_s(buffer, &localTm);
	buffer[strlen(buffer) - 1] = '\0';
	std::string timeStr(buffer);

	return timeStr;
#endif
}
