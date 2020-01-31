
#define SPDLOG_NO_NAME
#define SPDLOG_NO_ATOMIC_LEVELS

#include "GPCS4Log.h"

#include <cstdio>
#include <cxxopts/cxxopts.hpp>
#include <memory>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

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

#define LOG_STR_BUFFER_LEN 2000

namespace GPCS4Log
{

std::vector<std::string> Split(const std::string& str, const std::string d = ".")
{
	std::vector<std::string> ret;
	size_t prev = 0;
	size_t pos  = 0;
	while ((pos = str.find(d, pos)) != std::string::npos)
	{
		ret.push_back(str.substr(prev, pos - prev));
		prev = (pos += d.size());
	}
	ret.push_back(str.substr(prev));

	return ret;
}
std::string Concat(const std::vector<std::string>& ss, const std::string d = ".")
{
	std::string ret;
	for (size_t i = 0; i < ss.size(); ++i)
	{
		if (i > 0)
		{
			ret += d;
		}
		ret += ss[i];
	}
	return ret;
}

static std::unique_ptr<spdlog::logger> g_logger;

void Init(int argc, char** argv)
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

	//g_logger.reset(new spdlog::logger("GPCS4", { console_sink, msvc_sink }));
	g_logger.reset(new spdlog::logger("GPCS4", { msvc_sink, file_sink, console_sink }));
	//g_logger.reset(new spdlog::logger("GPCS4", { msvc_sink }));
	g_logger->set_level(spdlog::level::trace);  // message showing filter

	//g_logger->flush_on(spdlog::level::trace); // I/O cost

	/// Init log channel
	cxxopts::Options opts("LogSystem", "Logger system opts");
	opts.add_options()

		("D,debug-channel", "Enable debug channel", cxxopts::value<std::vector<std::string>>())

			("L,list-channels", "List debug channel")

		;

	auto rslt = opts.parse(argc, argv);

	if (rslt.count("D"))
	{
		auto ls = rslt["D"].as<std::vector<std::string>>();

		for (auto&& s : ls)
		{
			for (auto&& p : ChannelContainer::get()->getChannels())
			{
				p->checkSig(s);
			}
		}
	}
	else if (rslt.count("L"))
	{
		for (auto&& p : ChannelContainer::get()->getChannels())
		{
			printf("%s\n", p->getName().c_str());
		}
	}
	else
	{
		for (auto&& p : ChannelContainer::get()->getChannels())
		{
			p->checkSig("ALL");
		}
	}
}

void Channel::print(Level nLevel, const char* szFunction, const char* szSourcePath, int nLine, const char* szFormat, ...)
{
	if (!m_bEnabled)
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
		g_logger->debug("[{}]{}({}): {}", getName(), szFunction, nLine, szTempStr);
		break;
	case Level::kTrace:
		g_logger->trace("[{}]{}({}): {}", getName(), szFunction, nLine, szTempStr);
		break;
	case Level::kFixme:
		g_logger->warn("[{}]<FIXME>{}({}): {}", getName(), szFunction, nLine, szTempStr);
		break;
	case Level::kWarning:
		g_logger->warn("[{}]{}({}): {}", getName(), szFunction, nLine, szTempStr);
		break;
	case Level::kError:
		g_logger->error("[{}]{}({}): {}", getName(), szFunction, nLine, szTempStr);
		break;
	case Level::kSceTrace:
		g_logger->trace("[{}]<SCE>{}({}): {}", getName(), szFunction, nLine, szTempStr);
		break;
	case Level::kSceGraphic:
		g_logger->trace("[{}]<GRAPH>{}({}): {}", getName(), szFunction, nLine, szTempStr);
		break;
	}
}
void Channel::assert_(const char* szExpression, const char* szFunction, const char* szSourcePath, int nLine, const char* szFormat, ...)
{
	if (!m_bEnabled)
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
	m_vsChannelName(Split(n)), m_bEnabled(false)
{
	auto cc = ChannelContainer::get();
	cc->add(this);
}
void Channel::checkSig(const std::string& n)
{
	if (n == "ALL")
	{
		m_bEnabled = true;
		return;
	}
	m_bEnabled = false;

	auto up = Split(n);
	for (size_t i = 0; i < up.size() && i < m_vsChannelName.size(); ++i)
	{
		if (up[i] != m_vsChannelName[i])
		{
			return;
		}
	}

	// Cmd args enable "Solar.Earth"
	// If this name is "Solar.Earth.Asia", should enable
	if (up.size() <= m_vsChannelName.size())
	{
		m_bEnabled = true;
	}
}
std::string Channel::getName()
{
	return Concat(m_vsChannelName);
}

void ChannelContainer::add(Channel* ch)
{
	m_vpChannels.push_back(ch);
}
ChannelContainer* ChannelContainer::get()
{
	static ChannelContainer cc;
	return &cc;
}
std::vector<Channel*>& ChannelContainer::getChannels()
{
	return m_vpChannels;
}
ChannelContainer::ChannelContainer()
{
}

}  // namespace GPCS4Log
