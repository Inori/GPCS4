#pragma once

#include "GnmCommon.h"
#include "GnmConstant.h"
#include "GnmStructure.h"
#include "UtilFlag.h"
#include "Gcn/GcnConstants.h"
#include "Gcn/GcnShaderMeta.h"
#include "Gcn/GcnModule.h"

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

	struct GnmDepthStencilState
	{
		bool dbClearDepth = false;
	};

	struct GnmOutputMergerState
	{
		// Display buffer back render target
		SceResource* displayRenderTarget = nullptr;
	};

	struct GnmGraphicsState
	{
		std::array<GnmShaderContext, kShaderStageCount> shaderContext = {};

		GnmInputAssemblerState ia = {};
		GnmDepthStencilState   ds = {};
		GnmOutputMergerState   om = {};
	};

	struct GnmComputeState
	{
		GnmShaderContext shaderContext = {};
	};

	struct GnmRenderState
	{
		GnmGraphicsState gp;
		GnmComputeState  cp;
	};
}  // namespace sce::Gnm