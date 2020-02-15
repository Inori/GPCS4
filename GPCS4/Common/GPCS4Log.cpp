
#define SPDLOG_NO_NAME
#define SPDLOG_NO_ATOMIC_LEVELS

#include "GPCS4Log.h"

#include "Platform/UtilString.h"

#include <cstdio>
#include <memory>
#include <cxxopts/cxxopts.hpp>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#define LOG_STR_BUFFER_LEN 2000

#ifdef GPCS4_WINDOWS

#include <Windows.h>

void showMessageBox(const char* title, const char* message)
{
	MessageBoxA(NULL, message, title, MB_OK | MB_ICONERROR);
}

#else

void showMessageBox(const char* title, const char* message)
{
}

#endif  //GPCS4_WINDOWS


namespace logsys
{;

static std::unique_ptr<spdlog::logger> g_logger;

void initSpdLog()
{
	/// Init spdlog

	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	console_sink->set_level(spdlog::level::trace);  // message generating filter
	console_sink->set_pattern("[%t][%^%l%$]%v");

	auto msvc_sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
	msvc_sink->set_level(spdlog::level::trace);  // message generating filter
	msvc_sink->set_pattern("[%t][%^%l%$]%v");

	auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("GPCS4.log", true);
	file_sink->set_level(spdlog::level::trace);
	file_sink->set_pattern("[%t][%^%l%$]%v");

	//g_logger.reset(new spdlog::logger("GPCS4", { console_sink }));
	//g_logger.reset(new spdlog::logger("GPCS4", { msvc_sink, file_sink, console_sink }));
	g_logger.reset(new spdlog::logger("GPCS4", { msvc_sink }));
	g_logger->set_level(spdlog::level::trace);  // message showing filter

	//g_logger->flush_on(spdlog::level::trace); // I/O cost
}

void initLogChannel(const cxxopts::ParseResult& optResult)
{
	/// Init log channel

	if (optResult.count("D"))
	{
		auto ls = optResult["D"].as<std::vector<std::string>>();

		for (auto&& s : ls)
		{
			for (auto&& p : ChannelContainer::get()->getChannels())
			{
				p->checkSig(s);
			}
		}
	}
	else if (optResult.count("L"))
	{
		for (auto&& p : ChannelContainer::get()->getChannels())
		{
			printf("%s\n", p->getName().c_str());
		}
	}
	//else
	//{
	//	for (auto&& p : ChannelContainer::get()->getChannels())
	//	{
	//		p->checkSig("ALL");
	//	}
	//}
}

void init(const cxxopts::ParseResult& optResult)
{
	initSpdLog();
	initLogChannel(optResult);
}

void Channel::print(Level nLevel, const char* szFunction, const char* szSourcePath, int nLine, const char* szFormat, ...)
{
	if (!m_enabled)
	{
		return;
	}
	// generate format string
	va_list stArgList;
	va_start(stArgList, szFormat);
	char szTempStr[LOG_STR_BUFFER_LEN + 1] = { 0 };
	vsprintf_s(szTempStr, LOG_STR_BUFFER_LEN, szFormat, stArgList);
	va_end(stArgList);

	switch (nLevel)
	{
	case Level::kDebug:
		g_logger->debug("{}({}): {}", szFunction, nLine, szTempStr);
		break;
	case Level::kTrace:
		g_logger->trace("{}({}): {}", szFunction, nLine, szTempStr);
		break;
	case Level::kFixme:
		g_logger->warn("<FIXME>{}({}): {}", szFunction, nLine, szTempStr);
		break;
	case Level::kWarning:
		g_logger->warn("{}({}): {}", szFunction, nLine, szTempStr);
		break;
	case Level::kError:
		g_logger->error("{}({}): {}", szFunction, nLine, szTempStr);
		break;
	case Level::kSceTrace:
		g_logger->trace("<SCE>{}({}): {}", szFunction, nLine, szTempStr);
		break;
	case Level::kSceGraphic:
		g_logger->trace("<GRAPH>{}({}): {}", szFunction, nLine, szTempStr);
		break;
	}
}

void Channel::assert_(const char* szExpression, const char* szFunction, const char* szSourcePath, int nLine, const char* szFormat, ...)
{
	if (!m_enabled)
	{
		// just always enable assert
	}
	// generate format string
	va_list stArgList;
	va_start(stArgList, szFormat);
	char szTempStr[LOG_STR_BUFFER_LEN + 1] = { 0 };
	vsprintf_s(szTempStr, LOG_STR_BUFFER_LEN, szFormat, stArgList);
	char szMsgBoxStr[LOG_STR_BUFFER_LEN + 1] = { 0 };
	sprintf_s(szMsgBoxStr, LOG_STR_BUFFER_LEN, "[Assert@%s]: %s\n[Cause]: %s\n[Path]: %s(%d): %s", getName().c_str(), szExpression, szTempStr, szSourcePath, nLine, szFunction);
	va_end(stArgList);

	g_logger->critical("[{}]{}({}): [Assert: {}] {}", getName(), szFunction, nLine, szExpression, szTempStr);

	showMessageBox("Assertion Fail", szMsgBoxStr);

#ifdef GPCS4_DEBUG
	::__debugbreak();
#else
	exit(-1);
#endif  // !GPCS4_DEBUG
}

Channel::Channel(const std::string& n) :
	m_channelNameList(UtilString::Split(n, '.')),
	m_enabled(false)
{
	auto cc = ChannelContainer::get();
	cc->add(this);
}

void Channel::checkSig(const std::string& n)
{
	do 
	{
		if (m_enabled)
		{
			// Already enabled, shouldn't be disabled again. 
			break;
		}

		if (n == "ALL")
		{
			m_enabled = true;
			break;
		}
		m_enabled = false;

		bool notMatch = false;
		auto up = UtilString::Split(n, '.');
		for (size_t i = 0; i < up.size() && i < m_channelNameList.size(); ++i)
		{
			if (up[i] != m_channelNameList[i])
			{
				notMatch = true;
				break;
			}
		}

		if (notMatch)
		{
			break;
		}

		// Cmd args enable "Solar.Earth"
		// If this name is "Solar.Earth.Asia", should enable
		if (up.size() <= m_channelNameList.size())
		{
			m_enabled = true;
		}
	} while (false);
}

std::string Channel::getName()
{
	return UtilString::Concat(m_channelNameList, ".");
}

///

void ChannelContainer::add(Channel* ch)
{
	m_channels.push_back(ch);
}

ChannelContainer* ChannelContainer::get()
{
	static ChannelContainer cc;
	return &cc;
}

std::vector<Channel*>& ChannelContainer::getChannels()
{
	return m_channels;
}

ChannelContainer::ChannelContainer()
{
}

}  // namespace log
