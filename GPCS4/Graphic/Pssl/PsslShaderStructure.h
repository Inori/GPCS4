#pragma once

#include "PsslShaderFileBinary.h"
#include "PsslShaderRegister.h"

#include <vector>
#include <optional>

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
	uint32_t startRegister = 0;  // Start sgpr register index or EUD index
	const void* resource = nullptr;  // Will be cast to proper buffer type when interpreted.
	uint32_t sizeDwords = 0;
};


/**
 * \brief A single shader input resource.
 *
 * V# T# S# buffer and any other shader resource input to the shader
 */
struct GcnShaderResourceInstance
{
	ShaderInputUsageType usageType;
	PsslShaderResource res;
};

/**
 * \brief Resources in EUD.
 */
struct GcnShaderResourceEUD
{
	// EUD start register
	uint32_t startRegister;

	// Resources in Extended User Data (EUD)
	// First field in the pair is EUD offset in dword.
	std::vector<std::pair<uint32_t, GcnShaderResourceInstance>> resources;
};

/**
 * \brief Resources in SRT.
 */
// TODO:
struct GcnShaderResourceSRT
{
	uint32_t placeHolder;
};

/**
 * \brief All shader input resources.
 *
 * V# T# S# buffer and any other shader resource input to the shader
 * including resources in EUD and SRT.
 */

struct GcnShaderResources
{
	// Resources in 16 User Data Registers.
	std::vector<GcnShaderResourceInstance> ud;

	// EUD resources
	std::optional<GcnShaderResourceEUD> eud = std::nullopt;

	// SRT resources
	// TODO:
	// Support SRT
	std::optional<GcnShaderResourceSRT> srt = std::nullopt;
};


}  // pssl