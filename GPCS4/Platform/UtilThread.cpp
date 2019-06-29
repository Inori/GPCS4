#include "UtilThread.h"

namespace UtilThread
{;

#ifdef GPCS4_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN


uint64 GetThreadId(void)
{
	return GetCurrentThreadId();
}


#elif defined(GPCS4_LINUX)

#include <sys/types.h>
uint64 GetThreadId(void)
{
	return gettid();
}


#endif  //GPCS4_WINDOWS



}