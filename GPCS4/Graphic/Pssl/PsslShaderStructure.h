#pragma once

#include "PsslShaderFileBinary.h"
#include "PsslShaderRegister.h"
#include <vector>

namespace pssl
{;


enum FetchShaderInstancingMode
{
	kFetchShaderUseVertexIndex = 0x0,
	kFetchShaderUseInstanceId = 0x1,
	kFetchShaderUseInstanceIdOverStepRate0 = 0x2,
	kFetchShaderUseInstanceIdOverStepRate1 = 0x3,
};

struct FetchShaderBuildState
{

	uint16_t                         fetchShaderBufferSize;
	uint16_t						 fetchShaderFlags;
	uint8_t						     firstFreeSgpr;
	uint8_t                          vsharpSlotCount;
	uint32_t                         shaderModifier;

	std::vector<FetchShaderInstancingMode> fetchShaderInstancingData;

	// From the SC:
	uint8_t							 vertexBaseUsgpr;
	uint8_t							 instanceBaseUsgpr;

	std::vector<VertexInputSemantic> inputSemantics;
	std::vector<InputUsageSlot>		 inputUsageSlots;

	// Semantic Remap Table
	std::vector<uint32_t>			 semanticsRemapTable;
};


/**
 * \brief Shader resource buffer
 *
 * User data sent to a shader.
 */
struct PsslShaderResource
{
	uint32_t startSlot = 0;  // Start sgpr register index or EUD index
	void* resource = nullptr;  // Will be cast to proper buffer type when interpreted.
};



}  // pssl