#pragma once

#include <cstdint>

struct SceFiber;

namespace UtilFiber
{;

void InitializeFiber();

void FiberRun(SceFiber *fiber, uint64_t argOnRun);

void SwitchToFiber(SceFiber *fiber, uint64_t argOnRunTo, uint64_t* argOnRun);

void FinalizeFiber(SceFiber *fiber);

void FiberReturnToThread();

SceFiber* FiberGetSelf();

}