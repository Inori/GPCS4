
#include "GPCS4Log.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <memory>
#include <cstdio>

#define LOG_STR_BUFFER_LEN 2000

#ifdef GPCS4_WINDOWS

#include <Windows.h>

static std::unique_ptr<spdlog::logger> logger;

void InitLogging()
{
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	console_sink->set_level(spdlog::level::trace); // message generating filter
	console_sink->set_pattern("[%T.%f][%t][%^%l%$]%v");

	auto msvc_sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
	msvc_sink->set_level(spdlog::level::trace); // message generating filter
	msvc_sink->set_pattern("[Asuka:%t][%l]%v");

	logger.reset(new spdlog::logger("stdout_msvc", { console_sink, msvc_sink }));
	logger->set_level(spdlog::level::trace); // message showing filter
}

void LogPrint(LogLevel nLevel, const char* szFunction, const char* szSourcePath, int nLine, const char* szFormat, ...)
{
	// generate format string
	va_list stArgList;
	va_start(stArgList, szFormat);
	char szTempStr[LOG_STR_BUFFER_LEN + 1] = { 0 };
	vsprintf_s(szTempStr, LOG_STR_BUFFER_LEN, szFormat, stArgList);
	va_end(stArgList);


	switch (nLevel)
	{
	case LogLevel::debug:
		logger->debug("{}({}): {}", szFunction, nLine, szTempStr);
		break;
	case LogLevel::trace:
		logger->trace("{}({}): {}", szFunction, nLine, szTempStr);
		break;
	case LogLevel::fixme:
		logger->warn("<FIXME>{}({}): {}", szFunction, nLine, szTempStr);
		break;
	case LogLevel::warning:
		logger->warn("{}({}): {}", szFunction, nLine, szTempStr);
		break;
	case LogLevel::error:
		logger->error("{}({}): {}", szFunction, nLine, szTempStr);
		break;
	case LogLevel::sce_trace:
		logger->trace("<SCE>{}({}): {}", szFunction, nLine, szTempStr);
		break;
	case LogLevel::sce_graphic:
		logger->trace("<GRAPH>{}({}): {}", szFunction, nLine, szTempStr);
		break;
	}
}

void LogAssert(const char* szExpression, const char* szFunction, const char* szSourcePath, int nLine, const char* szFormat, ...)
{
	// generate format string
	va_list stArgList;
	va_start(stArgList, szFormat);
	char szTempStr[LOG_STR_BUFFER_LEN + 1] = { 0 };
	vsprintf_s(szTempStr, LOG_STR_BUFFER_LEN, szFormat, stArgList);
	char szMsgBoxStr[LOG_STR_BUFFER_LEN + 1] = { 0 };
	sprintf_s(szMsgBoxStr, "[Assert]: %s\n[Cause]: %s\n[Path]: %s(%d): %s", szExpression, szTempStr, szSourcePath, nLine, szFunction);
	va_end(stArgList);

	logger->critical("{}({}): [Assert: {}] {}", szFunction, nLine, szExpression, szTempStr);
	MessageBoxA(NULL, szMsgBoxStr, "Assertion Fail", MB_OK | MB_ICONERROR);
	exit(-1);
}

#else


#endif  //GPCS4_WINDOWS



