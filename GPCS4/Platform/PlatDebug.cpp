#include "PlatDebug.h"

#ifdef GPCS4_WINDOWS

#else
#include <signal.h>
#endif

namespace plat
{

#ifdef GPCS4_WINDOWS

void debugBreakPoint()
{
	__debugbreak();
}

#else

void debugBreakPoint()
{
	raise(SIGTRAP);
}

#endif


}  // namespace UtilDebug