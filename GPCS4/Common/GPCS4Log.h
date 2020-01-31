#pragma once

#include <string>
#include <vector>

namespace GPCS4Log
{
void Init(int argc, char** argv);

enum class Level : int
{
	kTrace,
	kDebug,
	kFixme,
	kWarning,
	kError,
	kSceTrace,
	kSceGraphic,
};

class Channel
{
	const std::vector<std::string> m_vsChannelName;
	bool m_bEnabled;

public:
	void print(Level nLevel, const char* szFunction, const char* szSourcePath, int nLine, const char* szFormat, ...);
	void assert_(const char* szExpression, const char* szFunction, const char* szSourcePath, int nLine, const char* szFormat, ...);

	void checkSig(const std::string& n);
	std::string getName();
	Channel(const std::string& n);
};

class ChannelContainer
{
	ChannelContainer();
	std::vector<Channel*> m_vpChannels;

public:
	void add(Channel* ch);
	static ChannelContainer* get();
	std::vector<Channel*>& getChannels();
};
}  // namespace GPCS4Log

//do not use these directly
#define _LOG_PRINT_(level, format, ...)    __logger_handle.print(level, __FUNCTION__, __FILE__, __LINE__, format, __VA_ARGS__)
#define _LOG_ASSERT_(expr, format, ...)    (void)(!!(expr) || (__logger_handle.assert_(#expr, __FUNCTION__, __FILE__, __LINE__, format, __VA_ARGS__), 0))
#define _LOG_IF_(expr, level, format, ...) (void)(!!(expr) && (__logger_handle.print(level, __FUNCTION__, __FILE__, __LINE__, format, __VA_ARGS__), 0))

#ifdef GPCS4_DEBUG

// regist current .cpp file
#define LOG_CHANNEL(ch) static ::GPCS4Log::Channel __logger_handle(#ch)

// for debug print
#define LOG_DEBUG(format, ...) _LOG_PRINT_(::GPCS4Log::Level::kDebug, format, __VA_ARGS__);
// for trace calling
#define LOG_TRACE(format, ...) _LOG_PRINT_(::GPCS4Log::Level::kTrace, format, __VA_ARGS__);
// should be fixed, but without fix, the program should run, treat this as TODO
#define LOG_FIXME(format, ...) _LOG_PRINT_(::GPCS4Log::Level::kFixme, format, __VA_ARGS__);
// should be fixed, without this, program can run, but behaves unexpected
#define LOG_WARN(format, ...) _LOG_PRINT_(::GPCS4Log::Level::kWarning, format, __VA_ARGS__);
// critical error, program can't go on
#define LOG_ERR(format, ...) _LOG_PRINT_(::GPCS4Log::Level::kError, format, __VA_ARGS__);
// critical error, log then pop up a window then exit process
#define LOG_ASSERT(expr, format, ...) _LOG_ASSERT_(expr, format, __VA_ARGS__)

#define LOG_DEBUG_IF(expr, format, ...) _LOG_IF_(expr, ::GPCS4Log::Level::kDebug, format, __VA_ARGS__)
#define LOG_TRACE_IF(expr, format, ...) _LOG_IF_(expr, ::GPCS4Log::Level::kTrace, format, __VA_ARGS__)
#define LOG_FIXME_IF(expr, format, ...) _LOG_IF_(expr, ::GPCS4Log::Level::kFixme, format, __VA_ARGS__)
#define LOG_WARN_IF(expr, format, ...)  _LOG_IF_(expr, ::GPCS4Log::Level::kWarning, format, __VA_ARGS__)
#define LOG_ERR_IF(expr, format, ...)   _LOG_IF_(expr, ::GPCS4Log::Level::kError, format, __VA_ARGS__)

// only use to trace sce module export functions
// to trace other functions, use LOG_TRACE
#define LOG_SCE_TRACE(format, ...) _LOG_PRINT_(::GPCS4Log::Level::kSceTrace, format, __VA_ARGS__);

// only use to trace graphic calls, mostly in libVideoOut and libGnmDriver
#define LOG_SCE_GRAPHIC(format, ...) _LOG_PRINT_(::GPCS4Log::Level::kSceGraphic, format, __VA_ARGS__);

// not really implemented
// just return result which looks correct to let the program go on
#define LOG_SCE_DUMMY_IMPL() LOG_FIXME("SCE Dummy implemented");

// only used in PSSL module
// to indicate there's gcn instruction not translated.
#define LOG_PSSL_UNHANDLED_INST()                      \
	{                                                  \
		LOG_FIXME("PSSL Instruction not translated."); \
		__debugbreak();                                \
	}

#else

#define LOG_CHANNEL(ch)

#define LOG_DEBUG(format, ...)
#define LOG_TRACE(format, ...)
#define LOG_FIXME(format, ...)
#define LOG_WARN(format, ...)
#define LOG_ERR(format, ...)
#define LOG_ASSERT(expression, format, ...)

#define LOG_DEBUG_IF(format, ...)
#define LOG_TRACE_IF(format, ...)
#define LOG_FIXME_IF(format, ...)
#define LOG_WARN_IF(format, ...)
#define LOG_ERR_IF(format, ...)

#define LOG_SCE_TRACE(format, ...)
#define LOG_SCE_GRAPHIC(format, ...)
#define LOG_SCE_DUMMY_IMPL()
#define LOG_PSSL_UNHANDLED_INST()

#endif  // GPCS4_DEBUG