
#include "GPCS4Log.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <memory>
#include <cstdio>

#define LOG_STR_BUFFER_LEN 2000

#ifdef GPCS4_WINDOWS

#include <Windows.h>

static std::unique_ptr<spdlog::logger> g_logger;

void initLogging()
{
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	console_sink->set_level(spdlog::level::trace); // message generating filter
	console_sink->set_pattern("[%t][%^%l%$]%v");

	auto msvc_sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
	msvc_sink->set_level(spdlog::level::trace); // message generating filter
	msvc_sink->set_pattern("[%t][%^%l%$]%v");

	auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("GPCS4.log", true);
	file_sink->set_level(spdlog::level::trace);
	file_sink->set_pattern("[%t][%^%l%$]%v");

	//g_logger.reset(new spdlog::logger("GPCS4", { console_sink, msvc_sink }));
	//g_logger.reset(new spdlog::logger("GPCS4", { console_sink, file_sink }));
	g_logger.reset(new spdlog::logger("GPCS4", { msvc_sink }));
	g_logger->set_level(spdlog::level::trace); // message showing filter
	//g_logger->flush_on(spdlog::level::trace); // I/O cost
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
	case LogLevel::kDebug:
		g_logger->debug("{}({}): {}", szFunction, nLine, szTempStr);
		break;
	case LogLevel::kTrace:
		g_logger->trace("{}({}): {}", szFunction, nLine, szTempStr);
		break;
	case LogLevel::kFixme:
		g_logger->warn("<FIXME>{}({}): {}", szFunction, nLine, szTempStr);
		break;
	case LogLevel::kWarning:
		g_logger->warn("{}({}): {}", szFunction, nLine, szTempStr);
		break;
	case LogLevel::kError:
		g_logger->error("{}({}): {}", szFunction, nLine, szTempStr);
		break;
	case LogLevel::kSceTrace:
		g_logger->trace("<SCE>{}({}): {}", szFunction, nLine, szTempStr);
		break;
	case LogLevel::kSceGraphic:
		g_logger->trace("<GRAPH>{}({}): {}", szFunction, nLine, szTempStr);
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
	sprintf_s(szMsgBoxStr, LOG_STR_BUFFER_LEN, "[Assert]: %s\n[Cause]: %s\n[Path]: %s(%d): %s", szExpression, szTempStr, szSourcePath, nLine, szFunction);
	va_end(stArgList);

	g_logger->critical("{}({}): [Assert: {}] {}", szFunction, nLine, szExpression, szTempStr);
	MessageBoxA(NULL, szMsgBoxStr, "Assertion Fail", MB_OK | MB_ICONERROR);
	exit(-1);
}

#else


#endif  //GPCS4_WINDOWS



