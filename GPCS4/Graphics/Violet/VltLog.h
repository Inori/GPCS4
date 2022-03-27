#pragma once

#include "VltCommon.h"

namespace sce::vlt
{
	enum class LogLevel : uint32_t
	{
		Trace = 0,
		Debug = 1,
		Info  = 2,
		Warn  = 3,
		Error = 4,
		None  = 5,
	};

    class Logger
	{
	public:
		static void trace(const std::string& message);
		static void debug(const std::string& message);
		static void info(const std::string& message);
		static void warn(const std::string& message);
		static void err(const std::string& message);
		static void exception(const std::string& message);
	};

}  // namespace sce::vlt