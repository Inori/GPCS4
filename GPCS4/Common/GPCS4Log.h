#pragma once

#include "GPCS4Common.h"

void initLogging();

enum class LogLevel : int
{
	kTrace,
	kDebug,
	kFixme,
	kWarning,
	kError,
	kSceTrace,
	kSceGraphic,
};

//do not use these directly
void LogPrint(LogLevel nLevel, const char* szFunction, const char* szSourcePath, int nLine, const char* szFormat, ...);
void LogAssert(const char* szExpression, const char* szFunction, const char* szSourcePath, int nLine, const char* szFormat, ...);
#define _LOG_PRINT_(level, format, ...) LogPrint(level, __FUNCTION__, __FILE__, __LINE__, format, __VA_ARGS__)
#define _LOG_ASSERT_(expr, format, ...) LogAssert(expr, __FUNCTION__, __FILE__, __LINE__, format, __VA_ARGS__)


#ifdef GPCS4_DEBUG

// for debug print
#define LOG_DEBUG(format, ...)	_LOG_PRINT_(LogLevel::kDebug, format, __VA_ARGS__);
// for trace calling
#define LOG_TRACE(format, ...)	_LOG_PRINT_(LogLevel::kTrace, format, __VA_ARGS__);
// should be fixed, but without fix, the program should run, treat this as TODO
#define LOG_FIXME(format, ...)	_LOG_PRINT_(LogLevel::kFixme, format, __VA_ARGS__);
// should be fixed, without this, program can run, but behaves unexpected
#define LOG_WARN(format, ...)	_LOG_PRINT_(LogLevel::kWarning, format, __VA_ARGS__);
// critical error, program can't go on
#define LOG_ERR(format, ...)	_LOG_PRINT_(LogLevel::kError, format, __VA_ARGS__);
// critical error, log then pop up a window then exit process
#define LOG_ASSERT(expression, format, ...) (void)(                 \
            (!!(expression)) ||                                     \
            (_LOG_ASSERT_(#expression, format, __VA_ARGS__), 0)		\
        )

// only use to trace sce module export functions
// to trace other functions, use LOG_TRACE
#define LOG_SCE_TRACE(format, ...)	_LOG_PRINT_(LogLevel::kSceTrace, format, __VA_ARGS__);

// only use to trace graphic calls, mostly in libVideoOut and libGnmDriver
#define LOG_SCE_GRAPHIC(format, ...)	_LOG_PRINT_(LogLevel::kSceGraphic, format, __VA_ARGS__);

// not really implemented
// just return result which looks correct to let the program go on
#define LOG_SCE_DUMMY_IMPL()	LOG_FIXME("SCE Dummy implemented");

// only used in PSSL module
// to indicate there's gcn instruction not translated.
#define LOG_PSSL_UNHANDLED_INST()                            \
		{                                                    \
			LOG_FIXME("PSSL Instruction not translated.");   \
			__debugbreak();                                  \
		}


#else

#define LOG_DEBUG(format, ...)	
#define LOG_TRACE(format, ...)	
#define LOG_FIXME(format, ...)
#define LOG_WARN(format, ...)
#define LOG_ERR(format, ...)
#define LOG_ASSERT(expression, format, ...) 

#define LOG_SCE_TRACE(format, ...)
#define LOG_SCE_GRAPHIC(format, ...)	
#define LOG_SCE_DUMMY_IMPL()
#define LOG_PSSL_UNHANDLED_INST()

#endif // GPCS4_DEBUG