#pragma once

#include "GnmCommon.h"
#include "GnmConstant.h"
#include "GnmLimits.h"
#include "GnmStructure.h"
#include "UtilFlag.h"

#include "Gcn/GcnConstants.h"
#include "Gcn/GcnModule.h"
#include "Gcn/GcnShaderMeta.h"
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
		DirtyViewportScissor,
		DirtyRasterizerState,
		DirtyRenderTargets,
		DirtyBlendState,
		DirtyDepthStencilState,
		DirtyDepthStencilClear,
	};

	using GnmContextFlags = util::Flags<GnmContextFlag>;

	using UserDataSlot = std::array<uint32_t, gcn::kMaxUserDataCount>;

	struct GnmShaderContext
	{
		const void*        code     = nullptr;
		UserDataSlot       userData = {};
		gcn::GcnShaderMeta meta;
	};

	struct GnmInputAssemblerState
	{
		vlt::Rc<vlt::VltBuffer>    indexBuffer = nullptr;
		VkIndexType                indexType   = VK_INDEX_TYPE_UINT32;
		vlt::VltInputAssemblyState isState;
	};

	struct GnmRasterizerState
	{
		uint32_t numViewports = 0;
		uint32_t numScissors  = 0;

		VkRect2D                                screenScissor;
		std::array<VkRect2D, MaxNumViewports>   scissors;
		std::array<VkViewport, MaxNumViewports> viewports;
		vlt::VltRasterizerState                 state;
	};

	struct GnmRenderTargets
	{
		std::array<RenderTarget, MaxNumRenderTargets> color;
		DepthRenderTarget                             depth;
	};

	struct GnmOutputMergerState
	{
		GnmRenderTargets                 targets;
		vlt::VltDepthStencilState        dsState;
		vlt::VltMultisampleState         msState;
		vlt::VltLogicOpState             loState;
		vlt::VltDepthStencilClear        dsClear;

		std::array<vlt::VltBlendMode, MaxNumRenderTargets> blendModes;

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