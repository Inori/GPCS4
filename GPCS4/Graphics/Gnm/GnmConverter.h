#pragma once

#include "GnmCommon.h"
#include "GnmDataFormat.h"
#include "SceVideoOut/sce_videoout_types.h"

#include "Violet/VltLimit.h"

#include <array>

namespace spv
{
	enum Dim;
}  // namespace spv

namespace sce::Gnm::cvt
{
	VkFormat convertZFormat(ZFormat zfmt);

	VkSampleCountFlagBits 
		convertNumFragments(NumFragments numFragments);

	VkFormat convertDataFormat(DataFormat dataFormat);

	VkFormat convertVideoOutPixelFormat(SceVideoOutPixelFormat format);

	DataFormat convertDataFormatFromVideoOutPixelFormat(SceVideoOutPixelFormat format);

	VkCompareOp convertCompareFunc(CompareFunc cmpFunc);

	std::array<VkColorComponentFlags, vlt::MaxNumRenderTargets>
	convertRenderTargetMask(uint32_t mask);

	VkBlendFactor convertBlendMultiplier(BlendMultiplier blendMul);

	VkBlendOp convertBlendFunc(BlendFunc func);

	VkPrimitiveTopology convertPrimitiveType(PrimitiveType primType);

	VkIndexType convertIndexSize(IndexSize indexSize);

	VkPolygonMode convertPolygonMode(PrimitiveSetupPolygonMode polyMode);

	VkCullModeFlags convertCullMode(PrimitiveSetupCullFaceMode cullMode);

	VkImageType convertTextureType(TextureType textureType);

	VkImageViewType convertTextureTypeView(TextureType textureType);

	VkFilter convertFilterMode(FilterMode mode);

	VkSamplerMipmapMode convertMipFilterMode(MipFilterMode mode);

	VkSamplerAddressMode convertWrapMode(WrapMode mode);

	VkCompareOp convertDepthCompare(DepthCompare compare);

	spv::Dim convertTextureTypeToDim(TextureType textureType);

}  // namespace sce::Gnm