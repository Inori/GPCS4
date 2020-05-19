#include "UtilFiber.h"
#include "SceModules/SceFiber/sce_fiber.h"

#ifdef GPCS4_WINDOWS

#include <Windows.h>


namespace UtilFiber
{;


struct FiberStruct
{
	SceFiber *fiber;
	uint64_t argOnRun;
};


static void FiberFunc(void* arg)
{
	auto fs = static_cast<FiberStruct*>(arg);
	fs->fiber->entry(fs->fiber->argOnInitialize, fs->argOnRun);
}



void InitializeFiber()
{
	if (IsThreadAFiber() == false)
	{
		ConvertThreadToFiber(NULL);
	}
}


void FiberRun(SceFiber *fiber, uint64_t argOnRun)
{
	FiberStruct* fs = new FiberStruct;
	fs->fiber = fiber;
	fs->argOnRun = argOnRun;

	fiber->fiber = CreateFiber(0, FiberFunc, fs);

	::SwitchToFiber(fiber->fiber);
}


void SwitchToFiber(SceFiber *fiber, uint64_t argOnRunTo, uint64_t* argOnRun)
{
	if (argOnRun != NULL)
	{
		*argOnRun = 0;
	}
	if (fiber->fiber == NULL)
	{
		FiberStruct* fs = new FiberStruct;
		fs->fiber = fiber;
		fs->argOnRun = argOnRunTo;

		fiber->fiber = CreateFiber(0, FiberFunc, fs);
	}
	else if (argOnRun != NULL)
	{
		FiberStruct* fs = reinterpret_cast<FiberStruct*>(::GetFiberData());
		*argOnRun = fs->argOnRun;
	}
	
	::SwitchToFiber(fiber->fiber);
}


void FinalizeFiber(SceFiber *fiber)
{
	::DeleteFiber(fiber->fiber);
}


void FiberReturnToThread()
{
	::ConvertFiberToThread();
}


SceFiber* FiberGetSelf()
{
	SceFiber* fiber = NULL;
	do
	{
		FiberStruct* fs = reinterpret_cast<FiberStruct*>(::GetFiberData());

		if (fs == NULL)
		{
			break;
		}

		fiber = fs->fiber;

	} while (false);

	return fiber;
}

}


#else

#endif // GPCS4_WINDOWS