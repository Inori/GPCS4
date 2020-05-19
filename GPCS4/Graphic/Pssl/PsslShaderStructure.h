#pragma once

#include "PsslShaderFileBinary.h"
#include "PsslShaderRegister.h"

#include <array>
#include <optional>
#include <vector>

namespace pssl
{;

constexpr size_t GcnMaxUserSgprCount = 16;

enum FetchShaderInstancingMode
{
	kFetchShaderUseVertexIndex             = 0x0,
	kFetchShaderUseInstanceId              = 0x1,
	kFetchShaderUseInstanceIdOverStepRate0 = 0x2,
	kFetchShaderUseInstanceIdOverStepRate1 = 0x3,
};

struct FetchShaderBuildState
{

	uint16_t fetchShaderBufferSize;
	uint16_t fetchShaderFlags;
	uint8_t  firstFreeSgpr;
	uint8_t  vsharpSlotCount;
	uint32_t shaderModifier;

	std::vector<FetchShaderInstancingMode> fetchShaderInstancingData;

	// From the SC:
	uint8_t vertexBaseUsgpr;
	uint8_t instanceBaseUsgpr;

	std::vector<VertexInputSemantic> inputSemantics;
	std::vector<InputUsageSlot>      inputUsageSlots;

	// Semantic Remap Table
	std::vector<uint32_t> semanticsRemapTable;
};

struct PsslShaderMetaVs
{
	uint32_t userSgprCount;
};

struct PsslShaderMetaPs
{
	uint32_t userSgprCount;
	// Used to initialize VGPRs for pixel shader,
	// see ISA manual.
	uint32_t spiPsInputAddr;
};

struct PsslShaderMetaCs
{
	uint32_t userSgprCount;
	uint32_t ldsSize;
	uint32_t computePgmRsrc2;

	uint32_t threadGroupX;
	uint32_t threadGroupY;
	uint32_t threadGroupZ;
};

union PsslShaderMeta 
{
	PsslShaderMetaVs vs;
	PsslShaderMetaPs ps;
	PsslShaderMetaCs cs;
};

enum class PsslSharpType
{
	VSharp,  // GnmBuffer
	TSharp,  // GnmTexture
	SSharp   // GnmSampler
};

/**
 * \brief Shader resource buffer
 *
 * User data sent to a shader.
 */
struct PsslShaderResource
{
	uint32_t    startRegister = 0;        // Start sgpr register index or EUD index
	const void* resource      = nullptr;  // Will be cast to proper buffer type when interpreted.
	uint32_t    sizeDwords    = 0;
};

using PsslShaderResourceTable = std::vector<PsslShaderResource>;

/**
 * \brief A single shader input resource.
 *
 * V# T# S# buffer and any other shader resource input to the shader
 */
struct GcnShaderResourceInstance
{
	ShaderInputUsageType usageType;
	PsslSharpType        sharpType;  // Some usage types could be interpreted as either V# or T#, e.g. ImmResource
	PsslShaderResource   res;
};

/**
 * \brief Vertex input attribute
 *
 */
struct GcnVertexInputAttribute
{
	uint32_t    bindingId = 0;
	const void* vsharp    = nullptr;
};

using GcnVertexInputAttributeTable = std::vector<GcnVertexInputAttribute>;

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

struct GcnShaderResourceDeclaration
{
	// Resources in 16 User Data Registers.
	std::vector<GcnShaderResourceInstance> ud;

	// Vertex attributes
	std::optional<GcnVertexInputAttributeTable> iat = std::nullopt;

	// EUD resources
	std::optional<GcnShaderResourceEUD> eud = std::nullopt;

	// SRT resources
	// TODO:
	// Support SRT
	std::optional<GcnShaderResourceSRT> srt = std::nullopt;
};

/**
 * \brief Shader input information
 * 
 * Convenience struct to prevent too many parameters
 * in GCNCompiler's constructor.
 *
 */

using GcnUserDataRegister = std::array<uint32_t, GcnMaxUserSgprCount>;

struct GcnShaderInput
{
	PsslShaderMeta     meta;
	GcnShaderResourceDeclaration shaderResources;
	// TODO:
	// For the 16 user data registers, we should
	// use specialization constants, because these
	// values could be changed during runtime.
	GcnUserDataRegister                             userSgpr         = {};
	std::optional<std::vector<VertexInputSemantic>> vsInputSemantics = {};
};

}  // namespace pssl