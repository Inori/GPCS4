#include "UtilProcess.h"


namespace UtilProcess
{;

#ifdef GPCS4_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN

uint64 GetProcessTimeCounter()
{
	uint64 nCounter = 0;
	do 
	{
		// TODO:
		// this implement maybe not correct
		// Microsoft saids we can't get the frequency of the cpu
		// thus we can't calculate the elapsed time of a process
		ULONG64 nCycleTime = 0;
		if (!QueryProcessCycleTime(GetCurrentProcess(), &nCycleTime))
		{
			break;
		}

		nCounter = nCycleTime;
	} while (false);
	return nCounter;
}

#else

#endif  //GPCS4_WINDOWS


}