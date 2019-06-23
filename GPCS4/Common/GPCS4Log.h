#pragma once

#include "GPCS4Common.h"

#define GPCS4_DEBUG


#define LOG_LEVEL_DEBUG "DEBUG"
#define LOG_LEVEL_TRACE "TRACE"
#define LOG_LEVEL_FIXME "FIXME"
#define LOG_LEVEL_WARNING "WARNING"
#define LOG_LEVEL_ERROR "ERROR"

//do not use this directly
void LogPrint(const char* szLevel, const char* szFunction, int nLine, const char* szFormat, ...);
#define _LOG_PRINT_(level, format, ...) LogPrint(level, __FUNCTION__, __LINE__, format, __VA_ARGS__)


#ifdef GPCS4_DEBUG

// for debug print
#define LOG_DEBUG(format, ...)	_LOG_PRINT_(LOG_LEVEL_DEBUG, format, __VA_ARGS__);
// for trace calling
#define LOG_TRACE(format, ...)	_LOG_PRINT_(LOG_LEVEL_TRACE, format, __VA_ARGS__);
// should be fixed, but without fix, the program should be run
#define LOG_FIXME(format, ...)	_LOG_PRINT_(LOG_LEVEL_FIXME, format, __VA_ARGS__);
// should be fixed, without this, program can run, but behaves unexpected
#define LOG_WARN(format, ...)	_LOG_PRINT_(LOG_LEVEL_WARNING, format, __VA_ARGS__);
// critical error, program can't go on
#define LOG_ERR(format, ...)	_LOG_PRINT_(LOG_LEVEL_ERROR, format, __VA_ARGS__);

// only use to trace sce module export functions
// to trace other functions, use LOG_TRACE
// TODO:
// set specific logger
#define LOG_SCE_TRACE LOG_TRACE

#else


#define LOG_DEBUG(format, ...)	
#define LOG_TRACE(format, ...)	
#define LOG_FIXME(format, ...)
#define LOG_WARN(format, ...)
#define LOG_ERR(format, ...)

#define LOG_SCE_TRACE (format, ...)

#endif  //ENABLE_DEBUG