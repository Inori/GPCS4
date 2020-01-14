#include "UtilProcess.h"


namespace UtilProcess
{;

#ifdef GPCS4_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN

uint64_t GetProcessTimeCounter()
{
	uint64_t nCounter = 0;
	do 
	{
		// TODO:
		// this implement maybe not correct
		// Microsoft saids we can't get the frequency of the cpu
		// thus we can't calculate the elapsed time of a process
		//ULONG64 nCycleTime = 0;
		//if (!QueryProcessCycleTime(GetCurrentProcess(), &nCycleTime))
		//{
		//	break;
		//}
		//nCounter = nCycleTime;

		// TODO:
		// this implementation maybe not correct either :(
		// what we want is process time counter,
		// but performance counter give us the general system counter
		LARGE_INTEGER stCounter;
		if (!QueryPerformanceCounter(&stCounter))
		{
			break;
		}
		nCounter = stCounter.QuadPart;

	} while (false);
	return nCounter;
}

uint64_t GetProcessTimeFrequency()
{
	uint64_t nFreq = 0;
	do 
	{
		LARGE_INTEGER stFreq;
		if (!QueryPerformanceFrequency(&stFreq))
		{
			break;
		}

		nFreq = stFreq.QuadPart;
	} while (false);
	return nFreq;
}

#else

#endif  //GPCS4_WINDOWS


}