#pragma once

#include "GcnCommon.h"
#include "GcnProgramInfo.h"

#include "Gnm/GnmConstant.h"

namespace sce::gcn
{
	
    /**
     * \brief Binding numbers and properties
     */
	enum DxbcBindingProperties : uint32_t
	{
		// read-only buffer/texture
		GcnRoResourceBindingIndex = 0,
		GcnRoResourceBindingCount = Gnm::kSlotCountResource +
									Gnm::kSlotCountConstantBuffer +
									Gnm::kSlotCountBoolConstant +
									Gnm::kSlotCountFloatConstant,

		// sampler
		GcnSamplerBindingIndex = GcnRoResourceBindingIndex +
								 GcnRoResourceBindingCount,
		GcnSamplerBindingCount = Gnm::kSlotCountSampler,

		// read/write buffer/texture
		GcnRwResourceBindingIndex = GcnSamplerBindingIndex +
									GcnSamplerBindingCount,
		GcnRwResourceBindingCount = Gnm::kSlotCountRwResource,

		GcnStageBindingCount = GcnRoResourceBindingCount +
							   GcnSamplerBindingCount +
							   GcnRwResourceBindingCount,
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
     * \brief Computes read-only buffer/texture binding index
	 * 
	 * Including ImmResource and ImmConstBuffer
     * 
     * \param [in] stage Shader stage
     * \param [in] index Start register index
     * \returns Binding index
     */
	inline uint32_t computeRoResourceBinding(GcnProgramType stage, uint32_t index)
	{
		return computeStageBindingOffset(stage) + GcnRoResourceBindingIndex + index;
	}

	/**
     * \brief Computes read-write buffer/texture binding index
     * 
     * \param [in] stage Shader stage
     * \param [in] index Start register index
     * \returns Binding index
     */
	inline uint32_t computeRwResourceBinding(GcnProgramType stage, uint32_t index)
	{
		return computeStageBindingOffset(stage) + GcnRwResourceBindingIndex + index;
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