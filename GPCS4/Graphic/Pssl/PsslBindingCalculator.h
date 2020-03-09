#pragma once

#include "PsslCommon.h"
#include "PsslEnums.h"

#include <vector>

namespace pssl
{;


// Binding indices
// TODO:
// Currently the binding count only include 16 UD registers, need to extend to EUD in the future
enum PsslBindingIndex : uint32_t 
{
	PsslConstBufBindingIndex = 0,
	PsslConstBufBindingCount = 16,

	PsslSamplerBindingIndex = PsslConstBufBindingIndex + PsslConstBufBindingCount,
	PsslSamplerBindingCount = 16,

	PsslResourceBindingIndex = PsslSamplerBindingIndex + PsslSamplerBindingCount,
	PsslResourceBindingCount = 16,

	PsslStageBindingCount = PsslConstBufBindingCount + 
							PsslSamplerBindingCount + 
							PsslResourceBindingCount,
	PsslBindingIndexMax = PsslStageBindingCount * uint32_t(PsslProgramType::ShaderTypeCount)
};


/**
 * \brief Computes first binding index for a given stage
 *
 * \param [in] stage The shader stage
 * \returns Index of first binding
 */
inline uint32_t computeStageBindingOffset(PsslProgramType stage) 
{
	return PsslStageBindingCount * uint32_t(stage);
}


/**
 * \brief Computes constant buffer binding index
 *
 * \param [in] stage Shader stage
 * \param [in] index Constant buffer index
 * \returns Binding index
 */
inline uint32_t computeConstantBufferBinding(PsslProgramType stage, uint32_t index)
{
	return computeStageBindingOffset(stage) + PsslConstBufBindingIndex + index;
}


/**
 * \brief Computes sampler binding index
 *
 * \param [in] stage Shader stage
 * \param [in] index Sampler index
 * \returns Binding index
 */
inline uint32_t computeSamplerBinding(PsslProgramType stage, uint32_t index) 
{
	return computeStageBindingOffset(stage) + PsslSamplerBindingIndex + index;
}


/**
 * \brief Computes resource binding index
 *
 * \param [in] stage Shader stage
 * \param [in] index Resource index
 * \returns Binding index
 */
inline uint32_t computeResBinding(PsslProgramType stage, uint32_t index)
{
	return computeStageBindingOffset(stage) + PsslResourceBindingIndex + index;
}


}  // namespace pssl
