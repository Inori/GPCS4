#pragma once

#include "GnmCommon.h"
#include "GnmConstant.h"
#include "GnmDataFormat.h"

#include "../Pssl/PsslEnums.h"
#include "../Violet/VltLimit.h"

#include <array>

// enumeration/constant/format value converters
namespace cvt
{;

VkFormat convertZFormatToVkFormat(ZFormat zfmt);

VkFormat convertDataFormatToVkFormat(DataFormat dataFormat);

VkCompareOp convertCompareFunc(CompareFunc cmpFunc);

VkPolygonMode convertPolygonMode(PrimitiveSetupPolygonMode polyMode);

VkCullModeFlags convertCullMode(PrimitiveSetupCullFaceMode cullMode);

VkBlendFactor convertBlendMultiplierToVkFactor(BlendMultiplier blendMul);

VkBlendOp convertBlendFuncToVkOp(BlendFunc func);

VkPrimitiveTopology convertPrimitiveTypeToVkTopology(PrimitiveType primType);

VkIndexType convertIndexSize(IndexSize indexSize);

std::array<VkColorComponentFlags, vlt::VltLimits::MaxNumRenderTargets>
convertRrenderTargetMask(uint32_t mask);

VkPipelineStageFlagBits convertShaderStage(pssl::PsslProgramType type);

}  // namespace convertor