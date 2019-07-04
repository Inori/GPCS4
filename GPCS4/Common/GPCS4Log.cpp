
#include "GPCS4Log.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#define LOG_STR_BUFFER_LEN 1024 
#define LOG_TAG "[Asuka]"

#ifdef GPCS4_WINDOWS

#include <Windows.h>

void LogPrint(const char* szLevel, const char* szFunction, int nLine, const char* szFormat, ...)
{
	char szDbgStr[LOG_STR_BUFFER_LEN + 1] = { 0 };
	char szTempStr[LOG_STR_BUFFER_LEN + 1] = { 0 };
	unsigned int nTid = 0;
	va_list   stArgList;

	nTid = GetCurrentThreadId();
	va_start(stArgList, szFormat);

	vsprintf_s(szTempStr, LOG_STR_BUFFER_LEN, szFormat, stArgList);
	sprintf_s(szDbgStr, LOG_STR_BUFFER_LEN, "%s:%s:%d:%s:%04d:%s\n",
		LOG_TAG,
		szLevel,
		nTid,
		szFunction,
		nLine,
		szTempStr);

	va_end(stArgList);

	OutputDebugStringA(szDbgStr);
	//printf(szDbgStr);
}

void LogAssert(const char* szLevel, const char* szFunction, int nLine, const char* szFormat, ...)
{
	char szDbgStr[LOG_STR_BUFFER_LEN + 1] = { 0 };
	char szTempStr[LOG_STR_BUFFER_LEN + 1] = { 0 };
	unsigned int nTid = 0;
	va_list   stArgList;

	nTid = GetCurrentThreadId();
	va_start(stArgList, szFormat);

	vsprintf_s(szTempStr, LOG_STR_BUFFER_LEN, szFormat, stArgList);
	sprintf_s(szDbgStr, LOG_STR_BUFFER_LEN, "%s:%s:%d:%s:%04d:%s\n",
		LOG_TAG,
		szLevel,
		nTid,
		szFunction,
		nLine,
		szTempStr);

	va_end(stArgList);
	MessageBoxA(NULL, szDbgStr, "Error", MB_OK | MB_ICONERROR);
	exit(-1);
}

#else


#endif  //GPCS4_WINDOWS



