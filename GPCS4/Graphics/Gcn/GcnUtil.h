#pragma once

#include "GcnCommon.h"
#include "GcnProgramInfo.h"
#include "GcnCompilerDefs.h"
#include "Gnm/GnmConstant.h"

namespace sce::gcn
{
	
    /**
     * \brief Binding numbers and properties
     */
	enum DxbcBindingProperties : uint32_t
	{
		GcnConstBufferBindingIndex = 0,
		GcnConstBufferBindingCount = GcnMaxSGPR,

		GcnSamplerBindingIndex = GcnConstBufferBindingIndex +
								 GcnConstBufferBindingCount,
		GcnSamplerBindingCount = GcnMaxSGPR,

		GcnResourceBindingIndex = GcnSamplerBindingIndex +
								  GcnSamplerBindingCount,
		GcnResourceBindingCount = GcnMaxSGPR,

		GcnStageBindingCount = GcnConstBufferBindingCount +
							   GcnSamplerBindingCount +
							   GcnResourceBindingCount,
	};

	/**
     * \brief Computes first binding index for a given stage
     *
     * \param [in] stage The shader stage
     * \returns Index of first binding
     */
	inline uint32_t computeStageBindingOffset(GcnProgramType stage)
	{
		return GcnStageBindingCount * static_cast<uint32_t>(stage);
	}

	/**
     * \brief Computes constant buffer binding index
	 * 
	 * Including ImmResource and ImmConstBuffer
     * 
     * \param [in] stage Shader stage
     * \param [in] index Start register index
     * \returns Binding index
     */
	inline uint32_t computeConstantBufferBinding(GcnProgramType stage, uint32_t index)
	{
		return computeStageBindingOffset(stage) + GcnConstBufferBindingIndex + index;
	}

	/**
     * \brief Computes buffer/texture resource binding index
     * 
     * \param [in] stage Shader stage
     * \param [in] index Start register index
     * \returns Binding index
     */
	inline uint32_t computeResourceBinding(GcnProgramType stage, uint32_t index)
	{
		return computeStageBindingOffset(stage) + GcnResourceBindingIndex + index;
	}

	/**
     * \brief Computes sampler binding index
     * 
     * \param [in] stage Shader stage
     * \param [in] index Start register index
     * \returns Binding index
     */
	inline uint32_t computeSamplerBinding(GcnProgramType stage, uint32_t index)
	{
		return computeStageBindingOffset(stage) + GcnSamplerBindingIndex + index;
	}
}  // namespace sce::gcn