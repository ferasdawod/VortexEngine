#include "Logger.h"

#include "Debug.h"
#include <ctime>

#include <spdlog/spdlog.h>

namespace spd = spdlog;

Logger::Logger()
{
#if defined(DEBUG) || defined(_DEBUG)
	const auto queueSize = 4096;
	spd::set_async_mode(queueSize, spdlog::async_overflow_policy::block_retry);

	std::vector<spdlog::sink_ptr> sinks;
	sinks.push_back(std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>());
	sinks.push_back(std::make_shared<spd::sinks::daily_file_sink_mt>("Logs\\log", "txt", 0, 0));

	_logger.reset(DBG_NEW spd::logger("engine_core", sinks.begin(), sinks.end()));
	
	_logger->info("Welcome to the core engine");
	_logger->info("began logging");
#endif
}

Logger::~Logger()
{
#if defined(DEBUG) || defined(_DEBUG)
	spd::drop_all();
#endif
}

void Logger::Log(MessageType type, const char* msg) const
{
#if defined(DEBUG) || defined(_DEBUG)
	if (type == MessageType::Error)
		_logger->error(msg);
	else if (type == MessageType::Info)
		_logger->info(msg);
	else if (type == MessageType::Warning)
		_logger->warn(msg);
#endif
}

//std::string GetLocalTime()
//{
//#if defined(DEBUG) || defined(_DEBUG)
//	auto time = std::time(NULL);
//	tm localTm;
//	localtime_s(&localTm, &time);
//	char buffer[50];
//	asctime_s(buffer, &localTm);
//	buffer[strlen(buffer) - 1] = '\0';
//	std::string timeStr(buffer);
//
//	return timeStr;
//#endif
//}
