#include "UtilThread.h"

namespace UtilThread
{;

#ifdef GPCS4_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN


uint64_t GetThreadId(void)
{
	return GetCurrentThreadId();
}

void ThreadYield()
{
	SwitchToThread();
}


#elif defined(GPCS4_LINUX)

#include <sys/types.h>
uint64_t GetThreadId(void)
{
	return gettid();
}


void ThreadYield()
{

}

#endif  //GPCS4_WINDOWS



}