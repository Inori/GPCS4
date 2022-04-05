#include "VltLog.h"

LOG_CHANNEL(Graphic.Violet);

namespace sce::vlt
{
	LogLevel Logger::logLevel()
	{
		return LogLevel::Trace;
	}

	void Logger::log(LogLevel level, const std::string& message)
	{
		LOG_DEBUG(message.c_str());
	}

	void Logger::trace(const std::string& message)
	{
		LOG_TRACE(message.c_str());
	}

	void Logger::debug(const std::string& message)
	{
		LOG_DEBUG(message.c_str());
	}

	void Logger::info(const std::string& message)
	{
		LOG_DEBUG(message.c_str());
	}

	void Logger::warn(const std::string& message)
	{
		LOG_WARN(message.c_str());
	}

	void Logger::err(const std::string& message)
	{
		LOG_ERR(message.c_str());
	}

	void Logger::exception(const std::string& message)
	{
		LOG_ASSERT(false, message.c_str());
	}



}  // namespace sce::vlt