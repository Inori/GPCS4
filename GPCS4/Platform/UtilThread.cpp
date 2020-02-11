#include "UtilThread.h"


#ifdef GPCS4_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN

#elif defined(GPCS4_LINUX)
#include <unistd.h>
#include <sys/syscall.h>
#endif

namespace UtilThread
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

uint64_t GetThreadId(void)
{
	//return gettid();
	return syscall(SYS_gettid);
}


void ThreadYield()
{

}

#endif  //GPCS4_WINDOWS



}
