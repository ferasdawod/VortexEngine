#pragma once

#include "Utilities/Singleton.h"
#include <string>
#include <memory>

namespace spdlog
{
	class logger;
}

namespace spdlog
{
	class logger;
}

enum class MessageType
{
	Info,
	Warning,
	Error,
};

class Logger : public Singleton<Logger>
{
public:
	Logger();
	virtual ~Logger();

	void Log(MessageType type, const char* msg) const;
	void Log(MessageType type, const std::string& msg) const
	{
		Log(type, msg.c_str());
	}

protected:
	std::unique_ptr<spdlog::logger> _logger;
};