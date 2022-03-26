#include "PlatDebug.h"

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