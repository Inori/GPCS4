#pragma once

#include "GnmCommon.h"
#include "GnmConstant.h"
#include "GnmDataFormat.h"


// enumeration/constant/format value converters
namespace cvt
{;

VkFormat convertZFormatToVkFormat(ZFormat zfmt);

VkFormat convertDataFormatToVkFormat(DataFormat dataFormat);

VkCompareOp convertCompareFunc(CompareFunc cmpFunc);

VkPolygonMode convertPolygonMode(PrimitiveSetupPolygonMode polyMode);

VkCullModeFlags convertCullMode(PrimitiveSetupCullFaceMode cullMode);

VkBlendFactor convertBlendMultiplierToFactor(BlendMultiplier blendMul);

VkBlendOp convertBlendFuncToOp(BlendFunc func);

VkPrimitiveTopology convertPrimitiveTypeToTopology(PrimitiveType primType);

VkIndexType convertIndexSize(IndexSize indexSize);



}  // namespace convertor