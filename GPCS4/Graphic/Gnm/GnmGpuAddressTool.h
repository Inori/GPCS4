#pragma once

#include "GnmCommon.h"
#include "GnmConstant.h"

namespace GpuAddress
{;

uint32_t nextPowerOfTwo(uint32_t x);

bool isPowerOfTwo(uint32_t x);

uint32_t getPipeCount(PipeConfig pipeConfig);

bool isLinear(ArrayMode arrayMode);

bool isMacroTiled(ArrayMode arrayMode);

uint32_t getMicroTileThickness(ArrayMode arrayMode);

ArrayMode degradeLargeThickArrayMode(ArrayMode oldArrayMode, uint32_t bitsPerElement);

uint32_t fastIntLog2(uint32_t i);

}  // namespace GpuAddress