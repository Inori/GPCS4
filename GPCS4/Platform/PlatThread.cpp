#include "PlatThread.h"

#ifdef GPCS4_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#undef WIN32_LEAN_AND_MEAN
#else
	#include <unistd.h>
#endif

namespace plat
{;

#ifdef GPCS4_WINDOWS

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