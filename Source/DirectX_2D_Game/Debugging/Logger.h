#pragma once

#include "Utilities/Singleton.h"
#include <string>
#include <memory>
#include <hash_map>

namespace spdlog
{
	class logger;
}

class Logger : public Singleton<Logger>
{
public:
	Logger();
	virtual ~Logger();

	void Log(const std::string& msg);

protected:
	std::hash_map<int, std::shared_ptr<spdlog::logger>> _loggers;
};