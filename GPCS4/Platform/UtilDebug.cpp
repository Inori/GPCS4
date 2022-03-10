#include "UtilDebug.h"

namespace UtilDebug
{;

#ifdef GPCS4_WINDOWS

void debugBreakPoint()
{
	__debugbreak();
}

#else

//this header defines SIGTRAP
#include <signal.h>

void debugBreakPoint()
{
	raise(SIGTRAP);
}

#endif


}  // namespace UtilDebug
