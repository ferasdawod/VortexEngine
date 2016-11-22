#pragma once

#include "Utilities/Singleton.h"
#include <string>
#include <memory>

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
};