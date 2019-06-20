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

#define LOG_DEBUG(format, ...)	_LOG_PRINT_(LOG_LEVEL_DEBUG, format, __VA_ARGS__);
#define LOG_TRACE(format, ...)	_LOG_PRINT_(LOG_LEVEL_TRACE, format, __VA_ARGS__);
#define LOG_FIXME(format, ...)	_LOG_PRINT_(LOG_LEVEL_FIXME, format, __VA_ARGS__);
#define LOG_WARN(format, ...)	_LOG_PRINT_(LOG_LEVEL_WARNING, format, __VA_ARGS__);
#define LOG_ERR(format, ...)	_LOG_PRINT_(LOG_LEVEL_ERROR, format, __VA_ARGS__);

#else


#define LOG_DEBUG(format, ...)	
#define LOG_TRACE(format, ...)	
#define LOG_FIXME(format, ...)
#define LOG_WARN(format, ...)
#define LOG_ERR(format, ...)

#endif  //ENABLE_DEBUG