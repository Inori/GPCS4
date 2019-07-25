#pragma once

#include "ShaderFileBinary.h"
#include "ShaderRegister.h"
#include <vector>

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
