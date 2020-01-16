#include "UtilHardware.h"


namespace UtilHardware
{;

#ifdef GPCS4_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN

uint64_t GetTscFrequency()
{
	uint64_t nFreq = 0;
	do 
	{
		LARGE_INTEGER nFrequency;
		if (QueryPerformanceFrequency(&nFrequency) == 0)
		{
			break;
		}
		
		nFreq = nFrequency.QuadPart;
	} while (false);
	return nFreq;
}


#else


#endif  //GPCS4_WINDOWS


}