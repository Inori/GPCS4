#pragma once

#include "GnmCommon.h"
#include "GnmConstant.h"
#include "GnmStructure.h"
#include "GnmLimits.h"
#include "UtilFlag.h"
#include "Gcn/GcnConstants.h"
#include "Gcn/GcnShaderMeta.h"
#include "Gcn/GcnModule.h"
#include "Violet/VltConstantState.h"

#include <array>

namespace sce
{
	class SceResource;

	namespace vlt
	{
		class VltBuffer;
	}  // namespace vlt

}  // namespace sce

namespace sce::Gnm
{
	/**
     * \brief Graphics pipeline state flags
     * 
     * Stores some information on which state
     * of the graphics and compute pipelines
     * has changed and/or needs to be updated.
     */
	enum class GnmContextFlag : uint32_t
	{
		GpPlaceHolder,            ///< PlaceHolder
	};

	using GnmContextFlags = util::Flags<GnmContextFlag>;


	using UserDataArray = std::array<uint32_t, gcn::kMaxUserDataCount>;
	struct GnmShaderContext
	{
		const void*        code     = nullptr;
		UserDataArray      userData = {};
		gcn::GcnShaderMeta meta     = {};
	};

	struct GnmInputAssemblerState
	{
		vlt::Rc<vlt::VltBuffer> indexBuffer = nullptr;
		VkIndexType             indexType   = VK_INDEX_TYPE_UINT32;
		VkPrimitiveTopology     topology    = VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
	};

	struct GnmRasterizerState
	{
		uint32_t numViewports = 0;
		uint32_t numScissors  = 0;

		std::array<VkViewport, MaxNumViewports> viewports;
		std::array<VkRect2D, MaxNumViewports>   scissors;
		VltRasterizerState state;
	};

	struct GnmOutputMergerState
	{
		std::array<vlt::Rc<VltImageView>, MaxNumRenderTargets> renderTargetViews;
		vlt::Rc<VltImageView>                                  depthStencilView;

		std::array<VltBlendMode, 8> blendModes;
		VltDepthStencilState        dsState;
		VltMultisampleState         msState;
		VltLogicOpState             loState;

		float    blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		uint32_t sampleMask     = 0xFFFFFFFFu;
		uint32_t stencilRef     = 0u;
	};

	struct GnmGraphicsState
	{
		std::array<GnmShaderContext, kShaderStageCount> sc = {};

		GnmInputAssemblerState ia = {};
		GnmRasterizerState     rs = {};
		GnmOutputMergerState   om = {};
	};

	struct GnmComputeState
	{
		GnmShaderContext sc = {};
	};

	struct GnmRenderState
	{
		GnmGraphicsState gp;
		GnmComputeState  cp;
	};
}  // namespace sce::Gnm