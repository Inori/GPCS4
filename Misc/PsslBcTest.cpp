#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#define SCE_GNM_SET_ALIGN(n) __attribute__((__aligned__(n)))
#define SCE_GNM_ASSERT(x) ((void)(x))


typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;

typedef          char   int8;
typedef   signed char   sint8;
typedef unsigned char   uint8;
typedef          short  int16;
typedef   signed short  sint16;
typedef unsigned short  uint16;
typedef          int    int32;
typedef   signed int    sint32;
typedef unsigned int    uint32;

#define LAST_IND(x,part_type)    (sizeof(x)/sizeof(part_type) - 1)
#define HIGH_IND(x,part_type)  LAST_IND(x,part_type)
#define LOW_IND(x,part_type)   0


// Partially defined types. They are used when the decompiler does not know
// anything about the type except its size.
#define _BYTE  uint8
#define _WORD  uint16
#define _DWORD uint32
#define _QWORD uint64
typedef int32_t _BOOL4;
#define BYTEn(x, n)   (*((_BYTE*)&(x)+n))
#define WORDn(x, n)   (*((_WORD*)&(x)+n))
#define DWORDn(x, n)  (*((_DWORD*)&(x)+n))

#define LOBYTE(x)  BYTEn(x,LOW_IND(x,_BYTE))
#define LOWORD(x)  WORDn(x,LOW_IND(x,_WORD))
#define LODWORD(x) DWORDn(x,LOW_IND(x,_DWORD))
#define HIBYTE(x)  BYTEn(x,HIGH_IND(x,_BYTE))
#define HIWORD(x)  WORDn(x,HIGH_IND(x,_WORD))
#define HIDWORD(x) DWORDn(x,HIGH_IND(x,_DWORD))


static unsigned s_vex_vv[] = {
	#include "vex_vv.h"
};

struct ShaderBinaryInfo
{
	uint8_t			m_signature[7];				// 'OrbShdr'
	uint8_t			m_version;					// ShaderBinaryInfoVersion

	unsigned int	m_pssl_or_cg : 1;	// 1 = PSSL / Cg, 0 = IL / shtb
	unsigned int	m_cached : 1;	// 1 = when compile, debugging source was cached.  May only make sense for PSSL=1
	uint32_t		m_type : 4;	// See enum ShaderBinaryType
	uint32_t		m_source_type : 2;	// See enum ShaderSourceType
	unsigned int	m_length : 24;	// Binary code length (does not include this structure or any of its preceding associated tables)

	uint8_t			m_chunkUsageBaseOffsetInDW;			// in DW, which starts at ((uint32_t*)&ShaderBinaryInfo) - m_chunkUsageBaseOffsetInDW; max is currently 7 dwords (128 T# + 32 V# + 20 CB V# + 16 UAV T#/V#)
	uint8_t			m_numInputUsageSlots;				// Up to 16 user data reg slots + 128 extended user data dwords supported by CUE; up to 16 user data reg slots + 240 extended user data dwords supported by InputUsageSlot
	uint8_t         m_isSrt : 1;	// 1 if this shader uses shader resource tables and has an SrtDef table embedded below the input usage table and any extended usage info
	uint8_t         m_isSrtUsedInfoValid : 1;	// 1 if SrtDef::m_isUsed=0 indicates an element is definitely unused; 0 if SrtDef::m_isUsed=0 indicates only that the element is not known to be used (m_isUsed=1 always indicates a resource is known to be used)
	uint8_t         m_isExtendedUsageInfo : 1;	// 1 if this shader has extended usage info for the InputUsage table embedded below the input usage table
	uint8_t         m_reserved2 : 5;	// For future use
	uint8_t         m_reserved3;						// For future use

	uint32_t		m_shaderHash0;				// Association hash first 4 bytes
	uint32_t		m_shaderHash1;				// Association hash second 4 bytes
	uint32_t		m_crc32;					// crc32 of shader + this struct, just up till this field
};

typedef enum ShaderStage
{
	kShaderStageCs = 0x00000000,	///< Compute shader stage.
	kShaderStagePs = 0x00000001,	///< Pixel shader stage.
	kShaderStageVs = 0x00000002,	///< Vertex shader stage.
	kShaderStageGs = 0x00000003,	///< Geometry shader stage.
	kShaderStageEs = 0x00000004,	///< Export shader stage.
	kShaderStageHs = 0x00000005,	///< Hull shader stage.
	kShaderStageLs = 0x00000006,	///< LDS shader stage.

	kShaderStageCount											///< The number of shader stages.
} ShaderStage;


typedef enum ShaderConstantDwordSize
{
	kDwordSizeResource = 8,	///< <c>T#</c> or padded <c>V#</c>.
	kDwordSizeRwResource = 8,	///< <c>T#</c> or padded <c>V#</c>.
	kDwordSizeSampler = 4,	///< <c>S#</c>.
	kDwordSizeVertexBuffer = 4,	///< <c>V#</c>.
	kDwordSizeConstantBuffer = 4,	///< <c>V#</c>.
	kDwordSizeBoolConstant = 1,	///< 32 packed bits.
	kDwordSizeFloatConstant = 1,	///< IEEE single-precision float.
	kDwordSizeAppendConsumeCounterRange = 1,    ///< Two 16-bit integers packed together.
	kDwordSizeStreamoutBuffer = 4,    ///< <c>V#</c>.
	kDwordSizeExtendedUserData = 128,  ///< Space for extra shader user data.
	kDwordSizeDispatchDrawData = 32,   ///< Space for 32 <c>DWORD</c>s of DispatchDraw data.
	kDwordSizeGdsMemoryRange = 1,    ///< Two 16-bit integers packed together.
} ShaderConstantDwordSize;


typedef enum FetchShaderInstancingMode
{
	kFetchShaderUseVertexIndex = 0x0, ///< No instancing; use Vertex Index. Default.
	kFetchShaderUseInstanceId = 0x1, ///< Use the instance ID to index the data.
	kFetchShaderUseInstanceIdOverStepRate0 = 0x2, ///< Use the (instance ID / step rate 0) to index the data.
	kFetchShaderUseInstanceIdOverStepRate1 = 0x3, ///< Use the (instance ID / step rate 1) to index the data.
} FetchShaderInstancingMode;


const int32_t kMaxResourceCount = 16;
const int32_t kMaxRwResourceCount = 16;
const int32_t kMaxSamplerCount = 16;
const int32_t kMaxVertexBufferCount = 16;
const int32_t kMaxConstantBufferCount = 20;
const int32_t kMaxStreamOutBufferCount = 4;


const int32_t kMaxUserDataCount = 16;	///< PSSL compiler limit is 16, count not tracked by the InputResourceOffsets table
const int32_t kMaxSrtUserDataCount = 16;	///< PSSL compiler limit is 16, count not tracked by the InputResourceOffsets table

const int32_t kMaxResourceBufferCount = 4;	///< Maximum number for supported splits for the resource buffer per LCUE instance
const int32_t kMaxPsInputUsageCount = 32;	///< Maximum number of interpolants a PS Stage can receive

const int32_t kDefaultFetchShaderPtrSgpr = 0;	///< Default SGPR in PSSL
const int32_t kDefaultVertexBufferTablePtrSgpr = 2;	///< Default SGPR in PSSL
const int32_t kDefaultGlobalInternalTablePtrSgpr = 0;	///< Default SGPR in PSSL, Note: it has lower priority than FetchPtr (sgpr would be s[4:5], after FetchPtr and VbPtr)
const int32_t kDefaultStreamOutTablePtrSgpr = 2;	///< Default SGPR in PSSL, only used by VS copy shader in GS active stage
const int32_t kDefaultVertexldsEsGsSizeSgpr = 0;	///< Default SGPR in PSSL, only used by VS copy shader in GS active stage

const int32_t kResourceInUserDataSgpr = 0x8000;	///< In User data resource Mask
const int32_t kResourceIsVSharp = 0x4000;	///< VSharp resource Mask Note: only used/available for immediate resources
const int32_t kResourceValueMask = 0x3FFF;	///< Resource memory offset is stored in the lower 14-bits

// On-chip GS constants
const uint32_t kOnChipGsInvalidSignature = 0xFFFFFFFFU;

// Tessellation distribution constants (Neo only)
const uint32_t kTesselationDistrbutionMask = 0x7FFFFFFFU;					///< Tessellation Distribution mask HS shader stage
const uint32_t kTesselationDistrbutionEnabledMask = ~kTesselationDistrbutionMask;	///< Tessellation Distribution enabled for HS shader stage

// Shader stage constants
const uint32_t kShaderStageAsynchronousCompute = kShaderStageCount;		///< Additional stage for CsVs dispatch Draw shader
const uint32_t kNumShaderStages = kShaderStageCount + 1;	///< Number of unique shader stages for resource binding

// 6KB is enough to store anything you can bind to a GPU shader stage, all counted in <c>DWORD</c>s
const int32_t kGpuStageBufferSizeInDwords = (6 * 1024) / sizeof(uint32_t);	///< Size of Single buffer Stage
const int32_t kComputeScratchBufferSizeInDwords = kGpuStageBufferSizeInDwords;	///< Size of the Compute Scratch buffer
const int32_t kGraphicsScratchBufferSizeInDwords = kNumShaderStages * kGpuStageBufferSizeInDwords; ///< Size of the Graphics Scratch buffer (encompasses all graphics shader stages)
//const int32_t kGlobalInternalTableSizeInDwords = sce::kShaderGlobalResourceCount * sizeof(sce::Buffer) / sizeof(uint32_t); ///< Size of a global resource table

// Internal constant buffers that are expected at fixed API-slots
const int32_t kConstantBufferInternalApiSlotForEmbeddedData = 15; ///< Immediate/Embedded constant buffer fixed API-slot (any GPU stage).
const int32_t kConstantBufferInternalApiSlotReserved0 = 16; ///< Slot 16 is reserved by compiler
const int32_t kConstantBufferInternalApiSlotReserved1 = 17; ///< Slot 17 is reserved by compiler
const int32_t kConstantBufferInternalApiSlotReserved2 = 18; ///< Slot 18 is reserved by compiler
const int32_t kConstantBufferInternalApiSlotForTessellation = 19; ///< Tessellation constant buffer (with strides for LDS data) fixed API-slot (HS,VS/ES GPU stages).

// Internal constants for ShaderBinaryInfo
const uint64_t kShaderBinaryInfoSignatureMask = 0x00ffffffffffffffLL;
const uint64_t kShaderBinaryInfoSignatureU64 = 0x007264685362724fLL;

// DispatchDraw constants
const uint32_t kNumDispatchDrawRingBuffersRolls = 16; ///< Number of times the dispatch ring buffers can be rolled in a LCUE instance, see setupDispatchDrawRingBuffers()


struct InputResourceOffsets
{
	uint16_t requiredBufferSizeInDwords;	///< Specifies how much memory needs to be reserved to store all memory-based resources. These are things not set through PM4.
	bool	 isSrtShader;					///< A flag that specifies whether the shader makes use of SRTs.
	uint8_t	 shaderStage;					///< The shader stage (LS/HS/ES/GS/VS/PS) for the shader resources offsets.

	// For each available shader-resource-ptr, store the starting SGPR s[0:254] where it'll be set (<c>0xFF</c> means not used). Pointers take 2 SGPRs (64b) and must be 2DW aligned
	uint8_t	fetchShaderPtrSgpr;				///< The SGPR containing the fetch shader pointer. If this exists, <c>s[0:1]</c> is always used.
	uint8_t	vertexBufferPtrSgpr;			///< The SGPR containing the vertex buffer table pointer. If this exists, <c>s[2:3]</c> is always used, but only in the vertex pipeline.
	uint8_t streamOutPtrSgpr;				///< The SGPR containing the stream out buffer pointer. If this exists, <c>s[2:3]</c> is always used, but only in the Geometry pipeline.
	uint8_t userExtendedData1PtrSgpr;		///< The SGPR containing the user extended data table pointer.
//			uint8_t userInternalSrtDataPtrSgpr;		///< *Note: Not supported for now*.
	uint8_t constBufferPtrSgpr;				///< The SGPR containing the constant buffer table pointer.
	uint8_t resourcePtrSgpr;				///< The SGPR containing the resource buffer table pointer.
	uint8_t rwResourcePtrSgpr;				///< The SGPR containing the read/write resource buffer table pointer.
	uint8_t samplerPtrSgpr;					///< The SGPR containing the sampler buffer table pointer.
	uint8_t globalInternalPtrSgpr;			///< The SGPR containing the global internal pointer, which is either stored in <c>s[0:1]</c> or <c>s[4:5]</c>.
	uint8_t appendConsumeCounterSgpr;		///< The SGPR containing the 32bit value address and size used from GDS.
	uint8_t gdsMemoryRangeSgpr;				///< The SGPR containing the GDS address range for storage.
	uint8_t ldsEsGsSizeSgpr;				///< The SGPR containing the GWS resource base offset.
	uint8_t userSrtDataSgpr;				///< The SGPR containing the start offset of the SRT Data Buffer.
	uint8_t userSrtDataCount;				///< The number of <c>DWORD</c>s in use by the SRT Data Buffer. The size will be between 1-8.

	uint8_t gdsKickRingBufferOffsetSgpr;	///< The SGPR containing the GDS kick ring buffer offset for DispatchDraw.
	uint8_t vertexRingBufferOffsetSgpr;		///< The SGPR containing the Vertex kick ring buffer offset for DispatchDraw.	
	uint8_t dispatchDrawPtrSgpr;			///< The SGPR containing the DispatchDraw data pointer to DispatchDrawTriangleCullData structure.
	uint8_t	dispatchDrawInstancesSgpr;		///< The SGPR containing the number of instances for DispatchDraw.

	// For each available shader-resource-flat-table (aka array), store the memory offset (from the start of the buffer) to the beginning of its flat-table (0xFFFF means it's not used).
	// Note: arrays are 0 indexed but the user can skip/set any index inside the range, allowing gaps at any place. This accelerates setting the pointer to the beginning of flat-tables.
	uint16_t constBufferArrayDwOffset;		///< The constant buffer table offset into the main buffer.
	uint16_t vertexBufferArrayDwOffset;		///< The vertex buffer table offset into the main buffer.
	uint16_t resourceArrayDwOffset;			///< The resource buffer table offset into the main buffer.
	uint16_t rwResourceArrayDwOffset;		///< The read/write resource buffer table offset into the main buffer.
	uint16_t samplerArrayDwOffset;			///< The sampler buffer table offset into the main buffer.
	uint16_t streamOutArrayDwOffset;		///< The stream out buffer table offset into the main buffer. This is only for the Geometry pipeline.

	// For each logical shader API slot, store either: an offset to a memory location, or a User Data (UD) SGPR where the resource should be set.
	// Note: if (item[i]&kResourceInUserDataSgpr) it's set directly into s[0:15] using PM4 packets, otherwise it's copied into the scratch buffer using the offset.
	uint16_t resourceDwOffset[kMaxResourceCount];		///< The start offset of a resource in the resource buffer table or user data.
	uint16_t rwResourceDwOffset[kMaxRwResourceCount];		///< The start offset of a resource in the read/write resource buffer table or user data.
	uint16_t samplerDwOffset[kMaxSamplerCount];		///< The start offset of a sampler in the sampler buffer table or user data.
	uint16_t constBufferDwOffset[kMaxConstantBufferCount];	///< The start offset of a constant buffer in the constant buffer table or user data.
	uint16_t vertexBufferDwOffset[kMaxVertexBufferCount];	///< The start offset of a vertex array in the vertex buffer table or user data.
	uint16_t streamOutDwOffset[kMaxStreamOutBufferCount];///< The start offset of a stream out buffer in the stream out buffer table or user data. This is only for the Geometry pipeline.

	uint8_t resourceSlotCount;			///< The number of resource slots used by the shader.
	uint8_t rwResourceSlotCount;		///< The number of rw resource slots used by the shader.
	uint8_t samplerSlotCount;			///< The number of sampler slots used by the shader.
	uint8_t constBufferSlotCount;		///< The number of constant buffer slots used by the shader.
	uint8_t vertexBufferSlotCount;		///< The number of vertex buffer slots used by the shader.

	uint8_t pad[1];


	/** @brief Initializes several resource slots that the shader uses.
	 */
	void initSupportedResourceCounts()
	{
		resourceSlotCount = kMaxResourceCount;
		rwResourceSlotCount = kMaxRwResourceCount;
		samplerSlotCount = kMaxSamplerCount;
		constBufferSlotCount = kMaxConstantBufferCount;
		vertexBufferSlotCount = kMaxVertexBufferCount;
	}

	//			kShaderInputUsageImmAluFloatConst	// Immediate float const (scalar or vector). *Not Supported*
	//			kShaderInputUsageImmAluBool32Const	// 32 immediate Booleans packed into one UINT. *Not Supported*
};

typedef enum ShaderInputUsageType
{
	kShaderInputUsageImmResource = 0x00, ///< Immediate read-only buffer/texture descriptor.
	kShaderInputUsageImmSampler = 0x01, ///< Immediate sampler descriptor.
	kShaderInputUsageImmConstBuffer = 0x02, ///< Immediate constant buffer descriptor.
	kShaderInputUsageImmVertexBuffer = 0x03, ///< Immediate vertex buffer descriptor.
	kShaderInputUsageImmRwResource = 0x04, ///< Immediate read/write buffer/texture descriptor.
	kShaderInputUsageImmAluFloatConst = 0x05, ///< Immediate float const (scalar or vector).
	kShaderInputUsageImmAluBool32Const = 0x06, ///< 32 immediate Booleans packed into one UINT.
	kShaderInputUsageImmGdsCounterRange = 0x07, ///< Immediate UINT with GDS address range for counters (used for append/consume buffers).
	kShaderInputUsageImmGdsMemoryRange = 0x08, ///< Immediate UINT with GDS address range for storage.
	kShaderInputUsageImmGwsBase = 0x09, ///< Immediate UINT with GWS resource base offset.
	kShaderInputUsageImmShaderResourceTable = 0x0A, ///< Pointer to read/write resource indirection table.
	kShaderInputUsageImmLdsEsGsSize = 0x0D, ///< Immediate LDS ESGS size used in on-chip GS
	// Skipped several items here...
	kShaderInputUsageSubPtrFetchShader = 0x12, ///< Immediate fetch shader subroutine pointer.
	kShaderInputUsagePtrResourceTable = 0x13, ///< Flat resource table pointer.
	kShaderInputUsagePtrInternalResourceTable = 0x14, ///< Flat internal resource table pointer.
	kShaderInputUsagePtrSamplerTable = 0x15, ///< Flat sampler table pointer.
	kShaderInputUsagePtrConstBufferTable = 0x16, ///< Flat const buffer table pointer.
	kShaderInputUsagePtrVertexBufferTable = 0x17, ///< Flat vertex buffer table pointer.
	kShaderInputUsagePtrSoBufferTable = 0x18, ///< Flat stream-out buffer table pointer.
	kShaderInputUsagePtrRwResourceTable = 0x19, ///< Flat read/write resource table pointer.
	kShaderInputUsagePtrInternalGlobalTable = 0x1A, ///< Internal driver table pointer.
	kShaderInputUsagePtrExtendedUserData = 0x1B, ///< Extended user data pointer.
	kShaderInputUsagePtrIndirectResourceTable = 0x1C, ///< Pointer to resource indirection table.
	kShaderInputUsagePtrIndirectInternalResourceTable = 0x1D, ///< Pointer to internal resource indirection table.
	kShaderInputUsagePtrIndirectRwResourceTable = 0x1E, ///< Pointer to read/write resource indirection table.
	// Skipped several items here...
	kShaderInputUsageImmGdsKickRingBufferOffset = 0x22,	///< Immediate UINT offset into GDS kick ring buffer for DispatchDraw. This must not be in extended user data.
	kShaderInputUsageImmVertexRingBufferOffset = 0x23,	///< Immediate UINT offset into vertex ring buffer for DispatchDraw. This must not be in extended user data.
	kShaderInputUsagePtrDispatchDraw = 0x24,	///< Pointer to DispatchDraw data. This must not be in extended user data.
	kShaderInputUsageImmDispatchDrawInstances = 0x25,	///< Immediate UINT ((firstInstance<<16)|(numInstances-1)). This must not be in extended user data.
} ShaderInputUsageType;

class VsShader;
class ShaderInfo
{
public:

	union
	{
		const void *m_shaderStruct;		///< A pointer to the shader struct -- typeless.
		const VsShader* m_vsShader;
	};

	const uint32_t *m_gpuShaderCode;		///< A pointer to the GPU Shader Code which will need to be copied into GPU visible memory.
	uint32_t m_gpuShaderCodeSize;			///< The size of the GPU Shader Code in bytes.
	uint32_t m_reserved;
};

class PipelineStage
{
public:
	/// Represents vertex shader information.
	class VsInfo
	{
	public:
		uint8_t		m_vertexShaderVariant;	///< The <c>PsslVertexVariant</c> such as <c>kVertexVariantVertex</c>, <c>kVertexVariantExport</c>, <c>kVertexVariantLocal</c> etc.
		uint8_t		m_paddingTo32[3];		///< Padding.	
	};

	/// Represents domain shader information.
	class DsInfo
	{
	public:
		uint8_t		m_domainShaderVariant;	///< The <c>PsslDomainVariant</c> such as <c>kDomainVariantVertex</c>, <c>kDomainVariantExport</c> etc. 
		uint8_t		m_paddingTo32[3];		///< Padding.
	};

	/// Represents geometry shader information.
	class GsInfo
	{
	public:
		uint8_t		m_geometryShaderVariant;	///< The <c>PsslGeometryVariant</c> such as <c>kGeometryVariantOnBuffer</c>, <c>kGeometryVariantOnChip</c> etc. 
		uint8_t		m_paddingTo32[3];		///< Padding.
	};

	/// Represents geometry shader information.
	class HsInfo
	{
	public:
		uint8_t		m_hullShaderVariant;	///< The <c>PsslHullVariant</c> such as <c>kHullVariantOnBuffer</c>, <c>kHullVariantOnChip</c> etc. 
		uint8_t		m_paddingTo32[3];		///< Padding.
	};

	/// Stores data as different class types, depending on the type of shader. 
	union
	{
		uint32_t    m_u32;                  ///< An unsigned 32 bit integer. 
		VsInfo      m_vsInfo;				///< The vertex shader information. 
		DsInfo      m_dsInfo;				///< The domain shader information. 
		GsInfo      m_gsInfo;               ///< The geometry shader information.
		HsInfo      m_hsInfo;               ///< The hull shader information.
	};
};

class SystemAttributes
{
public:
	/// Represents CS pipeline stage information.
	class CsInfo
	{
	public:
		uint16_t	m_numThreads[3];        ///< The number of threads.
	};

	/// Represents GS pipeline stage information.
	class GsInfo
	{
	public:
		uint16_t	m_instance;				///< The instance of the GS Shader. 
		uint16_t	m_maxVertexCount;       ///< The maximum number of vertices count.
		uint8_t		m_inputType;            ///< The GS Input Type (<c>PsslGsIoType</c>) such as triangle, line, point, adjacent tri + line, or patch.
		uint8_t		m_outputType;           ///< The GS Output Type (<c>PsslGsIoType</c>) such as triangles, lines, or points.
		uint8_t		m_patchSize;	        ///< The patch size in case of patch topology.
	};

	/// Represents DS pipeline stage information.
	class DsInfo
	{
	public:
		uint8_t		m_domainPatchType;      ///< The <c>PsslHsDsPatchType</c>: triangle, quad, or isoline.  
		uint8_t		m_inputControlPoints;   ///< The number of points in the input patch.
	};

	/// Represents HS pipeline stage information.
	class HsInfo
	{
	public:
		uint8_t		m_domainPatchType;      ///< The <c>PsslHsDsPatchType</c>: triangle, quad, or isoline. 
		uint8_t		m_inputControlPoints;   ///< The number of points in the input patch.
		uint8_t		m_outputTopologyType;   ///< The <c>PsslHsTopologyType</c>: point, line, cwtri, or ccwtri.
		uint8_t		m_partitioningType;	    ///< The <c>PsslHsPartitioningType</c>: integer, powof2, odd_fractional, or even_fractional.

		uint8_t		m_outputControlPoints;  ///< The number of points in the output patch.
		uint8_t		m_patchSize; 		    ///< The size of patch.
		uint8_t		m_paddingTo32[2];       ///< Padding.

		float		m_maxTessFactor;        ///< The maximum tessellation factor.
	};

	/// Stores data as different class types, depending on the type of shader. 
	union
	{
		uint32_t	m_u32[3];               ///< 12 bytes. 
		CsInfo      m_csInfo;               ///< The compute shader information.
		GsInfo      m_gsInfo;               ///< The geometry shader information.
		DsInfo      m_dsInfo;               ///< The domain shader information.
		HsInfo      m_hsInfo;               ///< The hull shader information.
	};
};

class Header
{
public:
	uint8_t				m_formatVersionMajor;         ///< The version of shader binary format: major numbering. 
	uint8_t				m_formatVersionMinor;         ///< The version of shader binary format: minor numbering.
	uint16_t			m_compilerRevision;           ///< The compiler type specific version of shader compiler: this is the svn revision for m_compilerType==kCompilerTypeOrbisPsslc or kCompilerTypeOrbisEsslc or for kCompilerTypeUnspecified (pre-SDK 2.500 versions of these compilers)

	uint32_t			m_associationHash0;           ///< The shader association hash 1.
	uint32_t			m_associationHash1;           ///< The shader association hash 2.

	uint8_t				m_shaderType;                 ///< The <c>PsslShaderType</c>: VS, PS, GS, CS, GS, HS, or DS.
	uint8_t				m_codeType;                   ///< The <c>PsslCodeType</c>: IL, ISA, or SCU.
	uint8_t             m_usesShaderResourceTable;    ///< The shader uses resource table.
	uint8_t		    	m_compilerType : 4;      ///< The <c>PsslCompilerType</c>; 0
	uint8_t				m_paddingTo32 : 4;      // 0; reserved for future use

	uint32_t			m_codeSize;                   ///< The size of code section.

	PipelineStage		m_shaderTypeInfo;             ///< The shader pipeline stage info.
	SystemAttributes	m_shaderSystemAttributeInfo;  ///< The shader system attribute info.
};

class ShaderFileHeader
{
public:
	uint32_t        m_fileHeader;			///< File identifier. Should be equal to kShaderFileHeaderId
	uint16_t        m_majorVersion;			///< Major version of the shader binary.
	uint16_t        m_minorVersion;			///< Minor version of the shader binary.
	uint8_t         m_type;					///< Type of shader. Comes from ShaderType.
	uint8_t			m_shaderHeaderSizeInDW;	///< <c>\<Type\>Shader.computeSize()/4</c>. For example, see CsShader::computeSize().
	uint8_t			m_shaderAuxData;		///< A flag that indicates whether shader auxiliary data is present after end of the shader data ( <c>sizeof(ShaderFileHeader) +</c>
														///< <c>m_shaderHeaderSizeInDW * 4 + ShaderCommonData::m_shaderSize +</c>
														///< <c>ShaderCommonData::m_embeddedConstantBufferSizeInDQW * 16)</c>. Set to 1 to indicate it is
	uint8_t         m_targetGpuModes;		///< Union of all TargetGpuMode values for which this shader binary is valid.
	uint32_t        m_reserved1;			///< Must be 0.
};

class ShaderCommonData
{
public:
	// Memory Layout:
	// - Shader setup data (starting with ShaderCommonData)
	// - n InputUsage (4 bytes each)
	// - immediateConstants
	uint32_t        m_shaderSize : 23;		   ///< The size of the shader binary code block in bytes.
	uint32_t        m_shaderIsUsingSrt : 1;		   ///< A bitflag that indicates if the shader is using a Shader Resource Table.
	uint32_t        m_numInputUsageSlots : 8;           ///< The number of InputUsageSlot entries following the main shader structure.
	uint16_t        m_embeddedConstantBufferSizeInDQW; ///< The size of the embedded constant buffer in 16-byte <c>DWORD</c>s.
	uint16_t        m_scratchSizeInDWPerThread;        ///< The scratch size required by each thread in 4-byte <c>DWORD</c>s.

	/** @brief Calculates and returns the size of the shader code including its embedded CB size in bytes */
	uint32_t computeShaderCodeSizeInBytes() const { return m_shaderSize + m_embeddedConstantBufferSizeInDQW * 16; }
};

#ifdef __cplusplus
class InputUsageSlot
#else  // __cplusplus
typedef struct InputUsageSlot
#endif // __cplusplus
{
#ifdef __cplusplus
public:
#endif // __cplusplus
	uint8_t m_usageType;		///< From ShaderInputUsageType.
	uint8_t m_apiSlot;			///< API slot or chunk ID.
	uint8_t m_startRegister;	///< User data slot.

	union
	{
		struct
		{
			uint8_t m_registerCount : 1; ///< If 0, count is 4DW; if 1, count is 8DW. Other sizes are defined by the usage type.
			uint8_t m_resourceType : 1;  ///< If 0, resource type <c>V#</c>; if 1, resource type <c>T#</c>, in case of a kShaderInputUsageImmResource.
			uint8_t m_reserved : 2; ///< Unused; must be set to zero.
			uint8_t m_chunkMask : 4; ///< Internal usage data.
		};
		uint8_t m_srtSizeInDWordMinusOne; ///< Size of the SRT data; used for kShaderInputUsageImmShaderResourceTable.
	};
#ifdef __cplusplus
} SCE_GNM_SET_ALIGN(4);
#else // __cplusplus
		} InputUsageSlot;
#endif // __cplusplus


#ifdef __cplusplus
class VertexInputSemantic
#else  // __cplusplus
typedef struct VertexInputSemantic
#endif // __cplusplus
{
#ifdef __cplusplus
public:
#endif // __cplusplus
	uint8_t m_semantic;
	uint8_t m_vgpr;
	uint8_t m_sizeInElements;
	uint8_t m_reserved; ///< Unused; must be set to zero.
#ifdef __cplusplus
};
#else // __cplusplus
		} VertexInputSemantic;
#endif // __cplusplus

#ifdef __cplusplus
class VertexExportSemantic
#else  // __cplusplus
typedef struct VertexExportSemantic
#endif // __cplusplus
{
#ifdef __cplusplus
public:
#endif // __cplusplus
	uint8_t m_semantic;  ///< Description to be specified.
	uint8_t m_outIndex : 5;  ///< Description to be specified.
	uint8_t m_reserved : 1;
	uint8_t m_exportF16 : 2;		///< if (m_exportF16 == 0) this shader exports a 32-bit value to this parameter; if (m_exportF16 & 1) this shader exports a 16-bit float value to the low 16-bits of each channel; if (m_exportF16 & 2) this shader exports a 16-bit float value to the high 16-bits of each channel
#ifdef __cplusplus
};
#else // __cplusplus
		} VertexExportSemantic;
#endif // __cplusplus


#ifdef __cplusplus
class VsStageRegisters
#else  // __cplusplus
typedef struct VsStageRegisters
#endif // __cplusplus
{
#ifdef __cplusplus
public:
#endif // __cplusplus
	uint32_t           m_spiShaderPgmLoVs; ///< The pointer to shader program (bits 39:8).
	uint32_t           m_spiShaderPgmHiVs; ///< The pointer to shader program (bits 47:40). This must be set to zero.

	uint32_t           m_spiShaderPgmRsrc1Vs;
	uint32_t           m_spiShaderPgmRsrc2Vs;

	uint32_t           m_spiVsOutConfig;
	uint32_t           m_spiShaderPosFormat;
	uint32_t		   m_paClVsOutCntl;

#ifdef __cplusplus


	/** @brief Patches the GPU address of the shader code.

		@param[in] gpuAddress		This address to patch. This must be aligned to a 256-byte boundary.
	 */
	void patchShaderGpuAddress(void *gpuAddress)
	{
		m_spiShaderPgmLoVs = static_cast<uint32_t>(uintptr_t(gpuAddress) >> 8);
		m_spiShaderPgmHiVs = static_cast<uint32_t>(uintptr_t(gpuAddress) >> 40);
	}

	bool isSharingContext(const VsStageRegisters shader) const
	{
		return	!((m_spiVsOutConfig - shader.m_spiVsOutConfig)
			| (m_spiShaderPosFormat - shader.m_spiShaderPosFormat)
			| (m_paClVsOutCntl - shader.m_paClVsOutCntl));
	}

#endif // __cplusplus

#ifdef __cplusplus
};
#else // __cplusplus
		} VsStageRegisters;
#endif // __cplusplus


#ifdef __cplusplus
class FetchShaderBuildState
#else  // __cplusplus
typedef struct FetchShaderBuildState
#endif // __cplusplus
{
#ifdef __cplusplus
public:
#endif // __cplusplus
	// Filled up by: Generate[xx]FSBuildState functions
	uint16_t                         m_fetchShaderBufferSize;
	uint16_t						 m_fetchShaderFlags;  ///< Description to be specified.
	uint8_t						     m_firstFreeSgpr;  ///< Description to be specified.
	uint8_t                          m_vsharpSlotCount;
	uint16_t                         m_numElementsInInstancingData; ///< Description to be specified.
	uint32_t                         m_shaderModifier; // to be passed to set[x]sShader functions.
	uint32_t                         m_reserved0; ///< Unused; must be set to zero.
	const FetchShaderInstancingMode *m_fetchShaderInstancingData;  ///< Description to be specified.

	// From the SC:
	uint8_t						     m_numInputSemantics;
	uint8_t							 m_vertexBaseUsgpr;
	uint8_t							 m_instanceBaseUsgpr;
	uint8_t							 m_reserved1; ///< Unused; must be set to zero.
	uint32_t						 m_numInputUsageSlots;
	const VertexInputSemantic		*m_inputSemantics;
	const InputUsageSlot			*m_inputUsageSlots;

	// Vertex Buffer Semantic Remap Table: <optional>
	uint32_t						m_numElementsInRemapTable;
	uint32_t						m_reserved2;
	const uint32_t					*m_semanticsRemapTable; // vertex buffer index -> vertex buffer semantic (e.g. vbSemantic = m_semanticsRemapTable[vbIndex])
#ifdef __cplusplus
};
#else // __cplusplus
		} FetchShaderBuildState;
#endif // __cplusplus

class VsShader
{
public:
	ShaderCommonData m_common;			///< The common data for all shader stages.

	VsStageRegisters m_vsStageRegisters;	///< The data to be loaded into the VS shader stage registers. Please see DrawCommandBuffer::setVsShader() for more information.
	// not used if domain shader => vertex shader

	uint8_t m_numInputSemantics;				///< The number of entries in the input semantic table.
	uint8_t m_numExportSemantics;				///< The number of entries in the export semantic table.
	uint8_t m_gsModeOrNumInputSemanticsCs;		///< Stores a union of VsShaderGsMode values for a VsShader or GsShader::getCopyShader(), which are translated into a GsMode constant. For CsVsShader::getVertexShader() with CsVsShader::getComputeShader()->m_version==0, the number of input semantic table entries to use for the CsVsShader::getComputeShader() fetch shader is stored.
	uint8_t m_fetchControl;						///< The user registers that receive vertex and instance offsets for use in the fetch shader.


	/** @brief Patches the GPU address of the shader code.

		@param[in] gpuAddress		This address to patch. This must be aligned to a 256-byte boundary.
	 */
	void patchShaderGpuAddress(void *gpuAddress)
	{
		m_vsStageRegisters.patchShaderGpuAddress(gpuAddress);
	}

	void *getBaseAddress() const
	{
		return (void *)((((uintptr_t)m_vsStageRegisters.m_spiShaderPgmHiVs) << 40) | (((uintptr_t)m_vsStageRegisters.m_spiShaderPgmLoVs) << 8));
	}

	/** @brief Gets a pointer to this shader's input usage slot table that immediately follows this shader's structure in memory.

		@return A pointer to this shader's input usage slot table.
		*/
	const InputUsageSlot       *getInputUsageSlotTable() const { return (const InputUsageSlot *)(this + 1); }

	/** @brief Gets a pointer to this shader's input semantic table that immediately follows the input usage table in memory.

		@return A pointer to this shader's input semantic table.
		*/
	const VertexInputSemantic  *getInputSemanticTable()  const { return (const VertexInputSemantic *)(getInputUsageSlotTable() + m_common.m_numInputUsageSlots); }

	/** @brief Gets a pointer to this shader's export semantic table that immediately follows the input semantic table in memory.

		@return A pointer to this shader's export semantic table.
		*/
	const VertexExportSemantic *getExportSemanticTable() const { return (const VertexExportSemantic *)(getInputSemanticTable() + m_numInputSemantics); }

	/** @brief Computes the total size (in bytes) of the shader binary including this structure, the input usage table, and the input and export semantic tables.

		@return The total size in bytes of this shader binary and its associated tables.
		*/
	uint32_t computeSize() const
	{
		const uint32_t size = sizeof(VsShader) +
			sizeof(InputUsageSlot) * m_common.m_numInputUsageSlots +
			sizeof(VertexInputSemantic) * m_numInputSemantics +
			sizeof(VertexExportSemantic) * m_numExportSemantics;

		return (size + 3) & ~3U;
	}
	/** @brief Gets the user register that contains the vertex offset.

		@return The index of the register containing the vertex offset. A value of 0 indicates no register contains the vertex offset.
		*/
	uint8_t getVertexOffsetUserRegister() const
	{
		return m_fetchControl & 0xf;
	}
	/** @brief Gets the user register that contains the instance offset.

		@return The index of the register containing the instance offset. A value of 0 indicates no register contains the instance offset.
		*/
	uint8_t getInstanceOffsetUserRegister() const
	{
		return (m_fetchControl >> 4) & 0xf;
	}
};

void parseShader(ShaderInfo *shaderInfo, const void* data)
{
	const Header		*binaryHeader = (const Header*)(data);
	const ShaderFileHeader		*header = (const ShaderFileHeader*)(binaryHeader + 1);
	const ShaderCommonData		*shaderCommon = (const ShaderCommonData*)(header + 1);
	const uint32_t						*sbAddress = (const uint32_t*)(shaderCommon + 1);

	const uint32_t		sbOffsetInDW = sbAddress[0] >> 2;

	shaderInfo->m_shaderStruct = (void*)shaderCommon;
	shaderInfo->m_gpuShaderCode = (uint32_t*)shaderCommon + sbOffsetInDW;
	shaderInfo->m_gpuShaderCodeSize = shaderCommon->computeShaderCodeSizeInBytes();
}

ShaderBinaryInfo* findShaderBinInfo(uint8_t* code)
{
	for (size_t i = 0; i != 0xFFFFFFFF; i++)
	{
		if (!std::memcmp(&code[i], "OrbShdr", 7))
		{
			return (ShaderBinaryInfo*)&code[i];
		}
	}
	return nullptr;
}

void generateInputResourceOffsetTable(InputResourceOffsets* outTable, ShaderBinaryInfo* sb)
{
	SCE_GNM_ASSERT(outTable != NULL);
	//SCE_GNM_ASSERT(shaderStage <= kShaderStageCount);

	// Get resource info to populate ShaderResourceOffsets
	//ShaderBinaryInfo const *shaderBinaryInfo = (ShaderBinaryInfo const*)((uintptr_t)shaderCode + shaderCodeSizeInBytes - sizeof(ShaderBinaryInfo));
	ShaderBinaryInfo const *shaderBinaryInfo = sb;
	//SCE_GNM_ASSERT((*(reinterpret_cast<uint64_t const*>(shaderBinaryInfo->m_signature)) & kShaderBinaryInfoSignatureMask) == kShaderBinaryInfoSignatureU64);

	// Get usage masks and input usage slots

	uint32_t const* usageMasks = reinterpret_cast<unsigned int const*>((unsigned char const*)shaderBinaryInfo - shaderBinaryInfo->m_chunkUsageBaseOffsetInDW * 4);
	int32_t inputUsageSlotsCount = shaderBinaryInfo->m_numInputUsageSlots;
	InputUsageSlot const* inputUsageSlots = (InputUsageSlot const*)usageMasks - inputUsageSlotsCount;

	// Cache shader input information into the ShaderResource Offsets table
	__builtin_memset(outTable, 0xFF, sizeof(InputResourceOffsets));
	outTable->initSupportedResourceCounts();
	//outTable->shaderStage = isDispatchDraw && shaderStage == kShaderStageCs ? kShaderStageAsynchronousCompute : shaderStage;
	//outTable->isSrtShader = isSrtUsed;
	int32_t	lastUserDataResourceSizeInDwords = 0;
	uint16_t requiredMemorySizeInDwords = 0;

	// Here we handle all immediate resources s[1:16] plus s[16:48] (extended user data)
	// resources that go into the extended user data also have "immediate" usage type, although they are stored in a table (not loaded by the SPI)
	for (int32_t i = 0; i < inputUsageSlotsCount; ++i)
	{
		uint8_t apiSlot = inputUsageSlots[i].m_apiSlot;
		uint8_t startRegister = inputUsageSlots[i].m_startRegister;
		bool isVSharp = (inputUsageSlots[i].m_resourceType == 0);
		uint16_t vsharpFlag = (isVSharp) ? kResourceIsVSharp : 0;

		uint16_t extendedRegisterOffsetInDwords = (startRegister >= kMaxUserDataCount) ?
			(startRegister - kMaxUserDataCount) : 0;
		requiredMemorySizeInDwords = (requiredMemorySizeInDwords > extendedRegisterOffsetInDwords) ?
			requiredMemorySizeInDwords : extendedRegisterOffsetInDwords;

		// Handle immediate resources, including some pointer types
		switch (inputUsageSlots[i].m_usageType)
		{
		case kShaderInputUsageImmGdsCounterRange:
			outTable->appendConsumeCounterSgpr = startRegister;
			break;

		case kShaderInputUsageImmGdsMemoryRange:
			outTable->gdsMemoryRangeSgpr = startRegister;
			break;

		case kShaderInputUsageImmLdsEsGsSize:
			outTable->ldsEsGsSizeSgpr = startRegister;
			break;

		case kShaderInputUsageSubPtrFetchShader:
			SCE_GNM_ASSERT(apiSlot == 0);
			outTable->fetchShaderPtrSgpr = startRegister;
			break;

		case kShaderInputUsagePtrInternalGlobalTable:
			SCE_GNM_ASSERT(apiSlot == 0);
			outTable->globalInternalPtrSgpr = startRegister;
			break;

		case kShaderInputUsagePtrExtendedUserData:
			SCE_GNM_ASSERT(apiSlot == 1);
			outTable->userExtendedData1PtrSgpr = startRegister;
			break;

		case kShaderInputUsageImmGdsKickRingBufferOffset:
			SCE_GNM_ASSERT(apiSlot == 0);
			SCE_GNM_ASSERT(startRegister < kMaxUserDataCount);
			outTable->gdsKickRingBufferOffsetSgpr = startRegister;
			break;

		case kShaderInputUsageImmVertexRingBufferOffset:
			SCE_GNM_ASSERT(apiSlot == 0);
			SCE_GNM_ASSERT(startRegister < kMaxUserDataCount);
			outTable->vertexRingBufferOffsetSgpr = startRegister;
			break;

		case kShaderInputUsagePtrDispatchDraw:
			SCE_GNM_ASSERT(apiSlot == 0);
			outTable->dispatchDrawPtrSgpr = startRegister;
			break;

		case kShaderInputUsageImmDispatchDrawInstances:
			SCE_GNM_ASSERT(apiSlot == 0);
			SCE_GNM_ASSERT(startRegister < kMaxUserDataCount);
			outTable->dispatchDrawInstancesSgpr = startRegister;
			break;

			// below resources can either be inside UserData or the EUD
		case kShaderInputUsageImmResource:
			SCE_GNM_ASSERT(apiSlot >= 0 && apiSlot < outTable->resourceSlotCount);
			outTable->resourceDwOffset[apiSlot] = (startRegister < kMaxUserDataCount) ?
				(kResourceInUserDataSgpr | vsharpFlag | startRegister) : (vsharpFlag | extendedRegisterOffsetInDwords);
			lastUserDataResourceSizeInDwords = (startRegister < kMaxUserDataCount) ? 0 : kDwordSizeResource;
			break;

		case kShaderInputUsageImmRwResource:
			SCE_GNM_ASSERT(apiSlot >= 0 && apiSlot < outTable->rwResourceSlotCount);
			outTable->rwResourceDwOffset[apiSlot] = (startRegister < kMaxUserDataCount) ?
				(kResourceInUserDataSgpr | vsharpFlag | startRegister) : (vsharpFlag | extendedRegisterOffsetInDwords);
			lastUserDataResourceSizeInDwords = (startRegister < kMaxUserDataCount) ? 0 : kDwordSizeRwResource;
			break;

		case kShaderInputUsageImmSampler:
			SCE_GNM_ASSERT(apiSlot >= 0 && apiSlot < outTable->samplerSlotCount);
			outTable->samplerDwOffset[apiSlot] = (startRegister < kMaxUserDataCount) ?
				(kResourceInUserDataSgpr | startRegister) : extendedRegisterOffsetInDwords;
			lastUserDataResourceSizeInDwords = (startRegister < kMaxUserDataCount) ? 0 : kDwordSizeSampler;
			break;

		case kShaderInputUsageImmConstBuffer:
			SCE_GNM_ASSERT(apiSlot >= 0 && apiSlot < outTable->constBufferSlotCount);
			outTable->constBufferDwOffset[apiSlot] = (startRegister < kMaxUserDataCount) ?
				(kResourceInUserDataSgpr | startRegister) : extendedRegisterOffsetInDwords;
			lastUserDataResourceSizeInDwords = (startRegister < kMaxUserDataCount) ? 0 : kDwordSizeConstantBuffer;
			break;

		case kShaderInputUsageImmVertexBuffer:
			SCE_GNM_ASSERT(apiSlot >= 0 && apiSlot < outTable->vertexBufferSlotCount);
			outTable->vertexBufferDwOffset[apiSlot] = (startRegister < kMaxUserDataCount) ?
				(kResourceInUserDataSgpr | startRegister) : extendedRegisterOffsetInDwords;
			lastUserDataResourceSizeInDwords = (startRegister < kMaxUserDataCount) ? 0 : kDwordSizeVertexBuffer;
			break;

			// SRTs will always reside inside the Imm UserData (dwords 0-15), as opposed to the 
			// above resources which can exist in the EUD
		case kShaderInputUsageImmShaderResourceTable:
			SCE_GNM_ASSERT(apiSlot >= 0 && apiSlot < kMaxUserDataCount);
			outTable->userSrtDataSgpr = inputUsageSlots[i].m_startRegister;
			outTable->userSrtDataCount = inputUsageSlots[i].m_srtSizeInDWordMinusOne + 1;
			break;

			// 		case kShaderInputUsagePtrSoBufferTable: // Only present in the VS copy-shader that doesn't have a footer
			// 			outTable->streamOutPtrSgpr = startRegister;
			// 			break;

		}
	}
	/*
	// Make sure we can fit a T# (if required) in the last userOffset
	requiredMemorySizeInDwords += lastUserDataResourceSizeInDwords;

	// Now handle only pointers to resource-tables. Items handled below cannot be found more than once
#if SCE_GNM_LCUE_USE_VERTEX_BUFFER_TABLE_MASK_IF_AVAILABLE
	// Note: in order to maintain binary compatibility, we can only put a new chunk mask for kShaderInputUsagePtrVertexBufferTable at the end of all other chunk masks
	bool bUseVertexBufferTableChunkMask = false;
#endif

	for (int32_t i = 0; i < inputUsageSlotsCount; ++i)
	{
		uint8_t maskChunks = inputUsageSlots[i].m_chunkMask;

		const uint64_t kNibbleToCount = 0x4332322132212110ull;
		uint8_t chunksCount = (kNibbleToCount >> ((maskChunks & 0xF) * 4)) & 0xF;
		SCE_GNM_ASSERT(usageMasks + chunksCount <= (uint32_t const*)shaderBinaryInfo);

		// Lets fill the resource indices first
		uint32_t usedApiSlots[kSlotCountResource]; // Use the size of the biggest resource table
		uint32_t usedApiSlotCount;

		// This thing will break if there's more than 1 table for any resource type
		uint8_t startRegister = inputUsageSlots[i].m_startRegister;

		switch (inputUsageSlots[i].m_usageType)
		{
		case kShaderInputUsagePtrResourceTable:
		{
			SCE_GNM_ASSERT(inputUsageSlots[i].m_apiSlot == 0);
			outTable->resourcePtrSgpr = startRegister;
			outTable->resourceArrayDwOffset = requiredMemorySizeInDwords;

			if (!(maskChunks & 0xF))
				break;

			SCE_GNM_ASSERT(usageMasks < (uint32_t const*)shaderBinaryInfo);
			uint32_t maskArray[4] = { 0, 0, 0, 0 };	// Max 128 slots are supported in the kShaderInputUsagePtrResourceTable
			if (maskChunks & 1) maskArray[0] = *usageMasks++;	// get slots 0-31 which are set in Chunk 0
			if (maskChunks & 2) maskArray[1] = *usageMasks++;	// get slots 32-63 which are set in Chunk 1
			if (maskChunks & 4) maskArray[2] = *usageMasks++;	// get slots 64-95 which are set in Chunk 2
			if (maskChunks & 8) maskArray[3] = *usageMasks++;	// get slots 96-127 which are set in Chunk 3
			SCE_GNM_ASSERT(usageMasks <= (uint32_t const*)shaderBinaryInfo);

			usedApiSlotCount = getUsedApiSlotsFromMask(&usedApiSlots[0], outTable->resourceSlotCount, maskArray, kSlotCountResource);
			SCE_GNM_ASSERT(usedApiSlotCount > 0);

			uint32_t lastUsedApiSlot = usedApiSlots[usedApiSlotCount - 1];
			for (uint8_t j = 0; j < usedApiSlotCount; j++)
			{
				uint16_t currentApiSlot = static_cast<uint16_t>(usedApiSlots[j]);
				outTable->resourceDwOffset[currentApiSlot] = requiredMemorySizeInDwords + currentApiSlot * kDwordSizeResource;
			}
			requiredMemorySizeInDwords += (lastUsedApiSlot + 1) * kDwordSizeResource;
		}
		break;

		case kShaderInputUsagePtrRwResourceTable:
		{
			SCE_GNM_ASSERT(inputUsageSlots[i].m_apiSlot == 0);
			outTable->rwResourcePtrSgpr = startRegister;
			outTable->rwResourceArrayDwOffset = requiredMemorySizeInDwords;

			if (!(maskChunks & 1))
				break;
			SCE_GNM_ASSERT(usageMasks < (uint32_t const*)shaderBinaryInfo);

			usedApiSlotCount = getUsedApiSlotsFromMask(&usedApiSlots[0], outTable->rwResourceSlotCount, *usageMasks++, kSlotCountRwResource);
			SCE_GNM_ASSERT(usedApiSlotCount > 0);

			uint32_t lastUsedApiSlot = usedApiSlots[usedApiSlotCount - 1];
			for (uint8_t j = 0; j < usedApiSlotCount; j++)
			{
				uint16_t currentApiSlot = static_cast<uint16_t>(usedApiSlots[j]);
				outTable->rwResourceDwOffset[currentApiSlot] = requiredMemorySizeInDwords + currentApiSlot * kDwordSizeRwResource;
			}
			requiredMemorySizeInDwords += (lastUsedApiSlot + 1) * kDwordSizeRwResource;
		}
		break;

		case kShaderInputUsagePtrConstBufferTable:
		{
			SCE_GNM_ASSERT(inputUsageSlots[i].m_apiSlot == 0);
			outTable->constBufferPtrSgpr = startRegister;
			outTable->constBufferArrayDwOffset = requiredMemorySizeInDwords;

			if (!(maskChunks & 1))
				break;
			SCE_GNM_ASSERT(usageMasks < (uint32_t const*)shaderBinaryInfo);

			usedApiSlotCount = getUsedApiSlotsFromMask(&usedApiSlots[0], outTable->constBufferSlotCount, *usageMasks++, kSlotCountConstantBuffer);
			SCE_GNM_ASSERT(usedApiSlotCount > 0);

			uint32_t lastUsedApiSlot = usedApiSlots[usedApiSlotCount - 1];
			for (uint8_t j = 0; j < usedApiSlotCount; j++)
			{
				uint16_t currentApiSlot = static_cast<uint16_t>(usedApiSlots[j]);
				outTable->constBufferDwOffset[currentApiSlot] = requiredMemorySizeInDwords + currentApiSlot * kDwordSizeConstantBuffer;
			}
			requiredMemorySizeInDwords += (lastUsedApiSlot + 1) * kDwordSizeConstantBuffer;
		}
		break;

		case kShaderInputUsagePtrSamplerTable:
		{
			SCE_GNM_ASSERT(inputUsageSlots[i].m_apiSlot == 0);
			outTable->samplerPtrSgpr = startRegister;
			outTable->samplerArrayDwOffset = requiredMemorySizeInDwords;

			if (!(maskChunks & 1))
				break;
			SCE_GNM_ASSERT(usageMasks < (uint32_t const*)shaderBinaryInfo);

			usedApiSlotCount = getUsedApiSlotsFromMask(&usedApiSlots[0], outTable->samplerSlotCount, *usageMasks++, kSlotCountSampler);
			SCE_GNM_ASSERT(usedApiSlotCount > 0);

			uint32_t lastUsedApiSlot = usedApiSlots[usedApiSlotCount - 1];
			for (uint8_t j = 0; j < usedApiSlotCount; j++)
			{
				uint16_t currentApiSlot = static_cast<uint16_t>(usedApiSlots[j]);
				outTable->samplerDwOffset[currentApiSlot] = requiredMemorySizeInDwords + currentApiSlot * kDwordSizeSampler;
			}
			requiredMemorySizeInDwords += (lastUsedApiSlot + 1) * kDwordSizeSampler;
		}
		break;

		case kShaderInputUsagePtrVertexBufferTable:
		{
			SCE_GNM_ASSERT(shaderStage == kShaderStageLs || shaderStage == kShaderStageEs || shaderStage == kShaderStageVs || shaderStage == kShaderStageCs);
			SCE_GNM_ASSERT(inputUsageSlots[i].m_apiSlot == 0);
			outTable->vertexBufferPtrSgpr = startRegister;
			outTable->vertexBufferArrayDwOffset = requiredMemorySizeInDwords;

#if SCE_GNM_LCUE_USE_VERTEX_BUFFER_TABLE_MASK_IF_AVAILABLE
			if (maskChunks & 1)
			{
				// Skip updating for the vertex buffer table below, since we are using the chunk mask at the end
				// we'll update it after everything else.
				bUseVertexBufferTableChunkMask = true;
				continue;
			}
#endif
			SCE_GNM_ASSERT(gnmxShaderStruct != NULL);
			const VertexInputSemantic* semanticTable = NULL;
			usedApiSlotCount = 0;
			if (shaderStage == kShaderStageVs && isDispatchDraw)
			{
				VsShader const* pVsShader = ((CsVsShader*)gnmxShaderStruct)->getVertexShader();
				usedApiSlotCount = pVsShader->m_numInputSemantics;
				semanticTable = pVsShader->getInputSemanticTable();
			}
			else if (shaderStage == kShaderStageVs)
			{
				usedApiSlotCount = ((VsShader*)gnmxShaderStruct)->m_numInputSemantics;
				semanticTable = ((VsShader*)gnmxShaderStruct)->getInputSemanticTable();
			}
			else if (shaderStage == kShaderStageLs)
			{
				usedApiSlotCount = ((LsShader*)gnmxShaderStruct)->m_numInputSemantics;
				semanticTable = ((LsShader*)gnmxShaderStruct)->getInputSemanticTable();
			}
			else if (shaderStage == kShaderStageEs)
			{
				usedApiSlotCount = ((EsShader*)gnmxShaderStruct)->m_numInputSemantics;
				semanticTable = ((EsShader*)gnmxShaderStruct)->getInputSemanticTable();
			}
			if (shaderStage == kShaderStageCs && isDispatchDraw)
			{
				VsShader const* pVsShader = ((CsVsShader*)gnmxShaderStruct)->getVertexShader();
				CsShader const* pCsShader = ((CsVsShader*)gnmxShaderStruct)->getComputeShader();
				if (pCsShader->m_version >= kDdCsShaderVersion_IndependentCsFetchShader)
				{
					usedApiSlotCount = pCsShader->m_numInputSemantics;
					semanticTable = pCsShader->getInputSemanticTable();
				}
				else
				{
					usedApiSlotCount = pVsShader->m_gsModeOrNumInputSemanticsCs;
					semanticTable = pVsShader->getInputSemanticTable();
				}
			}
			else if (shaderStage == kShaderStageCs)
			{
				CsShader const* pCsShader = ((CsShader*)gnmxShaderStruct);
				if (pCsShader->m_version >= kDdCsShaderVersion_IndependentCsFetchShader)
				{
					usedApiSlotCount = pCsShader->m_numInputSemantics;
					semanticTable = pCsShader->getInputSemanticTable();
				}
				else
				{
					usedApiSlotCount = 0;
					semanticTable = NULL;
				}
			}
			// Check if the shader uses any API-slot over the maximum count configured for the InputResourceOffset table
			SCE_GNM_ASSERT(usedApiSlotCount > 0 && usedApiSlotCount <= outTable->vertexBufferSlotCount);

			uint32_t usedApiSlots[kSlotCountVertexBuffer];

			// First use what the shader generated
			for (uint8_t i = 0; i < usedApiSlotCount; i++)
			{
				uint8_t semanticIndex = semanticTable[i].m_semantic;
				SCE_GNM_ASSERT(semanticIndex >= 0 && semanticIndex < outTable->vertexBufferSlotCount);
				usedApiSlots[i] = semanticIndex;
			}

			uint32_t firstUsedApiSlot = usedApiSlots[0];
			uint32_t lastUsedApiSlot = usedApiSlots[usedApiSlotCount - 1];

			// If a semanticRemapTable has been provided, override the shaders defined usage slots to conform with the remapped layout
			if (semanticRemapTable && numElementsInSemanticRemapTable != 0)
			{
				// Override values defined in the shader binary header above
				SCE_GNM_ASSERT(usedApiSlotCount <= numElementsInSemanticRemapTable);
				usedApiSlotCount = remapVertexBufferOffsetsWithSemanticTable(&usedApiSlots[0], firstUsedApiSlot, lastUsedApiSlot, outTable->vertexBufferSlotCount,
					semanticRemapTable, numElementsInSemanticRemapTable);
			}

			// Generate the final dword offsets for the vertex buffer table
			lastUsedApiSlot = usedApiSlots[usedApiSlotCount - 1];
			for (uint8_t j = 0; j < usedApiSlotCount; j++)
			{
				uint16_t currentApiSlot = static_cast<uint16_t>(usedApiSlots[j]);
				outTable->vertexBufferDwOffset[currentApiSlot] = requiredMemorySizeInDwords + currentApiSlot * kDwordSizeVertexBuffer;
			}
			requiredMemorySizeInDwords += (lastUsedApiSlot + 1) * kDwordSizeVertexBuffer;
		}
		break;
		}
	}
	*/

	// Note: this must be called after all other tables are processed above, as the vertex buffer table chunk mask (*usageMasks) 
	// is always stored at the end of the chunk mask table
/*
	if (bUseVertexBufferTableChunkMask)
	{
		uint32_t usedApiSlots[kSlotCountVertexBuffer];
		uint32_t usedApiSlotCount;

		SCE_GNM_ASSERT(usageMasks < (uint32_t const*)shaderBinaryInfo);

		usedApiSlotCount = getUsedApiSlotsFromMask(&usedApiSlots[0], outTable->vertexBufferSlotCount, *usageMasks++, kSlotCountVertexBuffer);
		SCE_GNM_ASSERT(usedApiSlotCount > 0);

		uint32_t firstUsedApiSlot = usedApiSlots[0];
		uint32_t lastUsedApiSlot = usedApiSlots[usedApiSlotCount - 1];

		// If a semanticRemapTable has been provided, override the shaders defined usage slots to conform with the remapped layout
		if (semanticRemapTable && numElementsInSemanticRemapTable != 0)
		{
			// Override values defined in the shader binary header above
			SCE_GNM_ASSERT((uint32_t)usedApiSlotCount <= numElementsInSemanticRemapTable);
			usedApiSlotCount = remapVertexBufferOffsetsWithSemanticTable(&usedApiSlots[0], firstUsedApiSlot, lastUsedApiSlot, outTable->vertexBufferSlotCount,
				semanticRemapTable, numElementsInSemanticRemapTable);
		}

		// Generate the final dword offsets for the vertex buffer table
		lastUsedApiSlot = usedApiSlots[usedApiSlotCount - 1];
		for (uint8_t j = 0; j < usedApiSlotCount; j++)
		{
			int32_t currentApiSlot = usedApiSlots[j];
			outTable->vertexBufferDwOffset[currentApiSlot] = requiredMemorySizeInDwords + currentApiSlot * kDwordSizeVertexBuffer;
		}
		requiredMemorySizeInDwords += (lastUsedApiSlot + 1) * kDwordSizeVertexBuffer;
	}
*/

	// Final amount of memory the shader will use from the scratch and resource buffer
	outTable->requiredBufferSizeInDwords = requiredMemorySizeInDwords;
	

	// Checking for non handled input data
	for (int32_t i = 0; i < inputUsageSlotsCount; ++i)
	{
		switch (inputUsageSlots[i].m_usageType)
		{
		case kShaderInputUsageImmResource:
		case kShaderInputUsageImmRwResource:
		case kShaderInputUsageImmSampler:
		case kShaderInputUsageImmConstBuffer:
		case kShaderInputUsageImmVertexBuffer:
		case kShaderInputUsageImmShaderResourceTable:
		case kShaderInputUsageSubPtrFetchShader:
		case kShaderInputUsagePtrExtendedUserData:
		case kShaderInputUsagePtrResourceTable:
		case kShaderInputUsagePtrRwResourceTable:
		case kShaderInputUsagePtrConstBufferTable:
		case kShaderInputUsagePtrVertexBufferTable:
		case kShaderInputUsagePtrSamplerTable:
		case kShaderInputUsagePtrInternalGlobalTable:
		case kShaderInputUsageImmGdsCounterRange:
		case kShaderInputUsageImmGdsMemoryRange:
		case kShaderInputUsageImmLdsEsGsSize:
		case kShaderInputUsageImmGdsKickRingBufferOffset:
		case kShaderInputUsageImmVertexRingBufferOffset:
		case kShaderInputUsagePtrDispatchDraw:
		case kShaderInputUsageImmDispatchDrawInstances:
			//		case kShaderInputUsagePtrSoBufferTable:		// Only present in the VS copy-shader that doesn't have a footer
			break;
		default:
			// Not handled yet
			(false, "Input Usage Slot type %d is not supported by LCUE sce::generateInputResourceOffsetTable()", inputUsageSlots[i].m_usageType);
			break;
		}
	}
	
}

int generateVsFetchShaderBuildState(FetchShaderBuildState *fsbs, const VsStageRegisters *pregs, uint32_t numInputs, const FetchShaderInstancingMode *instancingData, const uint32_t numElementsInInstancingData, uint8_t vertexBaseUserSgpr, uint8_t instanceBaseUsgpr)
{
	uint32_t v7; // ebp
	uint32_t v8; // er14
	uint32_t v9; // ebp
	_BOOL4 v10; // ebx
	unsigned int v11; // er15
	int v12; // er14
	int v13; // esi
	__int16 result; // ax
	bool v15; // zf
	signed int v16; // ecx

	v7 = pregs->m_spiShaderPgmRsrc2Vs;
	v8 = pregs->m_spiShaderPgmRsrc1Vs;
	v9 = ((((unsigned __int8)(v7 >> 7) | (unsigned __int8)(v7 >> 12)) & 1)
		+ ((pregs->m_spiShaderPgmRsrc2Vs >> 24) & 1)
		+ ((pregs->m_spiShaderPgmRsrc2Vs >> 7) & 1)
		+ ((v7 >> 11) & 1)
		+ ((v7 >> 10) & 1)
		+ ((v7 >> 9) & 1)
		+ ((unsigned __int8)v7 >> 8)
		+ ((v7 >> 12) & 1)
		+ (v7 & 1)
		+ ((pregs->m_spiShaderPgmRsrc2Vs >> 1) & 0x1F)
		+ 3) & 0xFFFFFFFC;
	v10 = (((v8 >> 3) & 0x78) + 8 - v9) >> 2 == 0;
	v11 = ((((v8 >> 3) & 0x78) + 8 - v9) >> 2) + 2 * v10;
	v12 = v8 & 0x3000000;
	v13 = (((_BYTE)v10 + (unsigned __int8)(pregs->m_spiShaderPgmRsrc1Vs >> 6)) & 0xF) << 6;
	fsbs->m_fetchShaderBufferSize = 4
		* ((numInputs + v11 - 1) / v11
			+ (instanceBaseUsgpr != 0)
			+ numInputs
			- ((vertexBaseUserSgpr < 1u)
				- 1))
		+ 8 * numInputs
		+ 12;
	fsbs->m_fetchShaderFlags = 0;
	fsbs->m_firstFreeSgpr = v9;
	fsbs->m_vsharpSlotCount = v11;
	fsbs->m_fetchShaderInstancingData = instancingData;
	result = 0;
	v15 = instancingData == 0LL;
	if (instancingData)
		result = numElementsInInstancingData;
	v16 = 0x3000000;
	if (v15)
		v16 = v12;
	fsbs->m_numElementsInInstancingData = result;
	fsbs->m_vertexBaseUsgpr = vertexBaseUserSgpr;
	fsbs->m_instanceBaseUsgpr = instanceBaseUsgpr;
	fsbs->m_reserved1 = 0;
	fsbs->m_reserved2 = 0;
	fsbs->m_reserved0 = 0;
	fsbs->m_shaderModifier = v13 | v16;
	return result;
}


int generateFetchShader(uint32_t *fs, FetchShaderBuildState *fsbs)
{
	FetchShaderBuildState *fsbs_; // r11
	__int64 *v3; // rsi
	char vbtStartRegister; // r14
	uint16_t v5; // ax
	const InputUsageSlot *inputUsageSlot; // rcx
	unsigned __int64 i; // rdx
	signed __int64 idx; // r9
	unsigned __int64 inputSemaCount; // rdi
	uint8_t vsharpSlotCount; // bl
	_DWORD *pEleNumInRemap; // r15
	__int64 v12; // rdx
	unsigned int k; // er13
	int shiftVbtStartRegister; // er14
	unsigned __int64 v15; // r10
	unsigned int v16; // er8
	const uint32_t *v17; // rbx
	int v18; // er12
	int semaIdx; // edx
	__int64 j; // rax
	unsigned int v21; // eax
	char v22; // cl
	int v23; // edx
	__int64 v24; // rcx
	__int64 v25; // rax
	unsigned int v26; // esi
	__int64 v27; // rcx
	const FetchShaderInstancingMode *v28; // r12
	const VertexInputSemantic *v29; // rdx
	__int64 v30; // r14
	int v31; // er11
	signed __int64 v32; // r8
	unsigned int v33; // edx
	unsigned int v34; // er13
	unsigned int v35; // er15
	int v36; // edi
	int v37; // esi
	unsigned int v38; // ebx
	unsigned int v39; // ecx
	__int64 v40; // rbx
	__int64 v41; // rdi
	__int64 v42; // rcx
	__int64 v43; // rax
	__int64 v44; // r9
	unsigned __int64 v46; // [rsp+8h] [rbp-98h]
	__int64 v47; // [rsp+10h] [rbp-90h]
	signed __int64 v48; // [rsp+18h] [rbp-88h]
	__int64 v49; // [rsp+20h] [rbp-80h]
	FetchShaderBuildState *v50; // [rsp+28h] [rbp-78h]
	unsigned __int64 v51; // [rsp+30h] [rbp-70h]
	signed __int64 v52; // [rsp+38h] [rbp-68h]
	int v53; // [rsp+44h] [rbp-5Ch]
	int v54; // [rsp+48h] [rbp-58h]
	int v55; // [rsp+4Ch] [rbp-54h]
	uint32_t *fs_dst; // [rsp+50h] [rbp-50h]
	uint8_t v57; // [rsp+5Fh] [rbp-41h]
	__int64 v58; // [rsp+60h] [rbp-40h]
	int v59; // [rsp+68h] [rbp-38h]
	int v60; // [rsp+6Ch] [rbp-34h]
	__int64 v61; // [rsp+70h] [rbp-30h]

	fsbs_ = fsbs;
	//v3 = (__int64 *)loc_96B;
	fs_dst = fs;
	vbtStartRegister = -1;
	//v61 = *loc_96B;
	v5 = fsbs_->m_fetchShaderFlags;
	v58 = 0x300000000LL;
	v59 = (v5 & 1) + 1;
	v60 = 2;
	if (fsbs_->m_numInputUsageSlots)
	{
		inputUsageSlot = fsbs_->m_inputUsageSlots;
		vbtStartRegister = -1;
		i = 0LL;
		do
		{
			if (inputUsageSlot[i].m_usageType == kShaderInputUsagePtrVertexBufferTable)
				vbtStartRegister = inputUsageSlot[i].m_startRegister;
			++i;
		} while (i < fsbs_->m_numInputUsageSlots);
	}

	idx = 0LL;

	if (fsbs_->m_vertexBaseUsgpr)
	{
		idx = 1LL;
		*fs = fsbs_->m_vertexBaseUsgpr | 0x4A000000;
	}

	if (fsbs_->m_instanceBaseUsgpr)
	{
		fs[idx] = fsbs_->m_instanceBaseUsgpr | 0x4A060600;
		LODWORD(idx) = idx + 1;
	}

	inputSemaCount = fsbs_->m_numInputSemantics;
	if (fsbs_->m_numInputSemantics)
	{
		vsharpSlotCount = fsbs_->m_vsharpSlotCount;
		pEleNumInRemap = &fsbs_->m_numElementsInRemapTable;
		v12 = 0LL;
		k = 0;
		v50 = fsbs_;
		v51 = fsbs_->m_numInputSemantics;
		shiftVbtStartRegister = (vbtStartRegister & 0x7E) << 8;
		v52 = (signed __int64)&fsbs_->m_numElementsInRemapTable;
		v55 = shiftVbtStartRegister;
		v57 = fsbs_->m_vsharpSlotCount;
		do
		{
			if (vsharpSlotCount)
			{
				v15 = (unsigned int)v12;
				v16 = 0;
				while (1)
				{
					v12 = (unsigned int)v15;
					if (v15 >= inputSemaCount)
						break;
					v17 = fsbs_->m_semanticsRemapTable;
					v18 = v15;
					semaIdx = fsbs_->m_inputSemantics[v15].m_semantic;
					if (v17 && *pEleNumInRemap)
					{
						j = 0LL;
						do
						{
							if (v17[j] == semaIdx)
								break;
							++j;
						} while ((unsigned int)j < *pEleNumInRemap);
						LOBYTE(semaIdx) = j;
					}
					v21 = *(unsigned __int16 *)&fsbs_->m_firstFreeSgpr;
					++v15;
					v22 = v21 + 4 * v16++;
					v23 = ((v22 & 0x7F) << 15) | shiftVbtStartRegister | 4 * (semaIdx & 0x3F);
					v24 = (unsigned int)idx;
					LODWORD(idx) = idx + 1;
					fs_dst[v24] = v23 | 0xC0800100;
					if (v16 >= v21 >> 8)
					{
						v12 = (unsigned int)(v18 + 1);
						break;
					}
				}
				vsharpSlotCount = v57;
			}
			v25 = (unsigned int)idx;
			LODWORD(idx) = idx + 1;
			fs_dst[v25] = 0xBF8C007F;
			if ((unsigned int)v12 > k)
			{
				v26 = *(_DWORD *)&fsbs_->m_firstFreeSgpr;
				v27 = v12;
				v28 = fsbs_->m_fetchShaderInstancingData;
				v29 = fsbs_->m_inputSemantics;
				v30 = 0LL;
				v49 = v27;
				v53 = 2 * k;
				v54 = 2 * v27;
				v47 = k;
				v31 = k - v27;
				v32 = (signed __int64)&v29[k].m_sizeInElements;
				v33 = idx;
				v46 = v26 >> 16;
				v48 = (signed __int64)&v28[k];
				v34 = (v26 & 0xFFFFFFFC) << 14;
				do
				{
					v35 = 0;
					if (v28 && v47 + v30 < v46)
						v35 = *(_DWORD *)(v48 + 4 * v30);
					v36 = *(unsigned __int8 *)(v32 + 4 * v30 - 1);
					v37 = *(unsigned __int8 *)(v32 + 4 * v30);
					v38 = v34;
					++v30;
					v34 += 0x10000;
					v39 = (v38 & 0x1F0000) + (*((unsigned __int8 *)&v58 + 4 * v35) | (v36 << 8)) + 0x80000000;
					v40 = v33;
					v41 = v33 + 1;
					v33 += 2;
					fs_dst[v40] = ((v37 << 18) + 0x1FC0000) & 0x1FC0000 | 0xE0002000;
					fs_dst[v41] = v39;
				} while (v31 + (_DWORD)v30);
				v12 = v49;
				fsbs_ = v50;
				shiftVbtStartRegister = v55;
				inputSemaCount = v51;
				pEleNumInRemap = (_DWORD *)v52;
				vsharpSlotCount = v57;
				LODWORD(idx) = v54 + idx - v53;
				k = v49;
			}
		} while (k < (unsigned int)inputSemaCount);
		//v3 = (__int64 *)loc_C1E;
	}
	else
	{
		pEleNumInRemap = &fsbs_->m_numElementsInRemapTable;
	}
	v42 = (unsigned int)idx;
	v43 = (unsigned int)(idx + 1);
	v44 = (unsigned int)(idx + 2);
	fs_dst[v42] = 0xBF8C0000;
	fs_dst[v43] = 0xBE802000;
	if (*pEleNumInRemap)
		LODWORD(inputSemaCount) = *pEleNumInRemap;
	fs_dst[v44] = inputSemaCount;
	return 0;
}

int main(void)
{
	ShaderInfo shaderInfo;
	parseShader(&shaderInfo, s_vex_vv);

	void *shaderBinary = malloc(shaderInfo.m_gpuShaderCodeSize);
	void *shaderHeader = malloc(shaderInfo.m_vsShader->computeSize());

	memcpy(shaderBinary, shaderInfo.m_gpuShaderCode, shaderInfo.m_gpuShaderCodeSize);
	memcpy(shaderHeader, shaderInfo.m_vsShader, shaderInfo.m_vsShader->computeSize());

	ShaderBinaryInfo* binInfo = findShaderBinInfo((uint8_t*)shaderBinary);

	InputResourceOffsets table;
	generateInputResourceOffsetTable(&table, binInfo);

	size_t codeLength = binInfo->m_length + sizeof(ShaderBinaryInfo);

	VsShader* m_shader = static_cast<VsShader*>(shaderHeader);
	m_shader->patchShaderGpuAddress(shaderBinary);

	VsShader* vertexShader = m_shader;
	FetchShaderBuildState fb = { 0 };
	generateVsFetchShaderBuildState(&fb, (const VsStageRegisters*)&vertexShader->m_vsStageRegisters, vertexShader->m_numInputSemantics, nullptr, 0, fb.m_vertexBaseUsgpr, fb.m_instanceBaseUsgpr);

	const InputUsageSlot *inputUsageSlots = vertexShader->getInputUsageSlotTable();
	fb.m_numInputSemantics = vertexShader->m_numInputSemantics;
	fb.m_inputSemantics = vertexShader->getInputSemanticTable();
	fb.m_numInputUsageSlots = vertexShader->m_common.m_numInputUsageSlots;
	fb.m_inputUsageSlots = inputUsageSlots;
	fb.m_numElementsInRemapTable = 0;
	fb.m_semanticsRemapTable = 0;

	uint32_t *fs = (uint32_t *)malloc(fb.m_fetchShaderBufferSize);
	generateFetchShader(fs, &fb);

	const InputUsageSlot* slot = m_shader->getInputUsageSlotTable();
	const VertexInputSemantic* inputSema = m_shader->getInputSemanticTable();
	const VertexExportSemantic* expSema = m_shader->getExportSemanticTable();
	
	return binInfo->m_isSrt;
}