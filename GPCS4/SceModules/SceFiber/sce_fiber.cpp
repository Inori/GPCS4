#include "sce_fiber.h"
#include "Platform/UtilFiber.h"

// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs

LOG_CHANNEL(SceModules.SceFiber);



//////////////////////////////////////////////////////////////////////////
// library: libSceFiber
//////////////////////////////////////////////////////////////////////////

int PS4API _sceFiberInitializeImpl(SceFiber* fiber, const char* name, SceFiberEntry entry, uint64_t argOnInitialize,
	void* addrContext, uint64_t sizeContext)
{
	LOG_SCE_TRACE("init fiber '%s'", name);
	return _sceFiberInitializeWithInternalOptionImpl(fiber, name, entry, argOnInitialize, addrContext, sizeContext, NULL);
}

int PS4API _sceFiberInitializeWithInternalOptionImpl(SceFiber* fiber, const char* name, SceFiberEntry entry, uint64_t argOnInitialize,
	void* addrContext, uint64_t sizeContext, const SceFiberOptParam* option)
{
	LOG_SCE_TRACE("init fiber impl '%s'", name);

	fiber->name = name;
	fiber->entry = entry;
	fiber->argOnInitialize = argOnInitialize;
	fiber->addrContext = addrContext;
	fiber->sizeContext = sizeContext;
	fiber->option = option;
	fiber->fiber = NULL;

	UtilFiber::InitializeFiber();

	return SCE_OK;
}

int PS4API sceFiberFinalize(SceFiber* fiber)
{
	LOG_SCE_TRACE("fiber finalize: '%s'", fiber->name);
	UtilFiber::FinalizeFiber(fiber);
	return SCE_OK;
}


int PS4API sceFiberReturnToThread(void)
{
	// LOG_SCE_TRACE("fiber return to thread"); // remove spam on libfiber sample
	UtilFiber::FiberReturnToThread();
	return SCE_OK;
}


int PS4API sceFiberRun(SceFiber* fiber, uint64_t argOnRunTo, uint64_t* argOnReturn)
{
	LOG_SCE_TRACE("fiber run: '%s'", fiber->name);
	UtilFiber::FiberRun(fiber, argOnRunTo);
	*argOnReturn = 0;
	return SCE_OK;
}


int PS4API sceFiberSwitch(SceFiber* fiber, uint64_t argOnRunTo, uint64_t* argOnRun)
{
	LOG_SCE_TRACE("fiber switch: '%s'", fiber->name);
	UtilFiber::SwitchToFiber(fiber, argOnRunTo, argOnRun);
	return SCE_OK;
}


int PS4API sceFiberGetSelf(SceFiber** fiber)
{
	*fiber = UtilFiber::FiberGetSelf();
	LOG_SCE_TRACE("fiber get self: '%s'", (*fiber)->name);
	return SCE_OK;
}


int PS4API sceFiberStartContextSizeCheck(uint32_t flags)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceFiberStopContextSizeCheck(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}

int PS4API sceFiberGetInfo(SceFiber* fiber, SceFiberInfo* fiberInfo)
{
	LOG_SCE_TRACE("fiber get info");
	fiberInfo->size = 128;
	fiberInfo->entry = fiber->entry;
	fiberInfo->argOnInitialize = fiber->argOnInitialize;
	fiberInfo->addrContext = fiber->addrContext;
	fiberInfo->sizeContext = fiber->sizeContext;
	::strncpy(fiberInfo->name, fiber->name, 32);
	fiberInfo->sizeContextMargin = 0; // TODO: set right value
	return SCE_OK;
}