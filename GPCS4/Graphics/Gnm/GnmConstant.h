#pragma once


#include <cstdint>
#include "GnmError.h"

namespace sce
{
    /** @brief Lowest-level interface to the PlayStation®4 GPU.
     *
     *   The Gnm namespace includes 
	 *   the lowest level APIs for the PlayStation®4 GPU. The boundary for forward compatibility in the PlayStation®4
	 *   graphics library is the Gnm-driver layer, but the Gnm interface is also considered a compatible layer because 
	 *   it has a one-to-one correspondence with the Gnm-driver layer. In the %Gnm library, each API provides an atomic
	 *   function, and the user should assemble these atomic functions in order to realize their objectives.
	 *
     *   For users who are just starting PlayStation®4 development, please begin by using the %Gnmx library. Once you get
	 *   used to how it works, start referring to the source code of Gnmx in order to learn how to work with Gnm functions.
	 *   From there you can begin to optimize your library. Please refer to the Gnmx namespace for further information.
	 *
     */
	namespace Gnm
	{

		/** @brief Memory alignment requirements for the various GPU memory objects.
			@note These values are the minimum alignment <em>required</em> by the hardware for each type. In many cases, alignment to larger
			      boundaries (for example, 64 byte cachelines) may be beneficial for performance reasons.
		*/
		typedef enum Alignment
		{
			kAlignmentOfShaderInBytes			= 256,  ///< Alignment of shader binaries, in bytes.
			kAlignmentOfTessFactorBufferInBytes = 256,  ///< Alignment of tessellation factor buffers, in bytes.
			kAlignmentOfMipStatsBufferInBytes   = 64,   ///< Alignment of MIP stats buffer, in bytes.
			kAlignmentOfOcclusionQueryInBytes	= 16,   ///< Alignment of OcclusionQueryResults objects, in bytes.
			kAlignmentOfIndirectArgsInBytes     = 8,    ///< Alignment of DrawIndirectArgs and DrawIndexIndirectArgs objects, in bytes.
			kAlignmentOfDispatchIndirectArgsInBytes = 32,    ///< Alignment of DispatchIndirectArgs, in bytes.
			kAlignmentOfBufferInBytes			= 4,    ///< Alignment of Buffer data, in bytes. Note that 8-byte alignment is required for any Buffer whose elements will be the target of 64-bit atomic operations.
			kAlignmentOfFetchShaderInBytes		= 4,    ///< Alignment of fetch shader binaries, in bytes.
			kAlignmentOfComputeQueueRingBufferInBytes = 256, ///< Alignment of Compute queue ring buffer, in bytes.
		} Alignment;

		/** @brief Various GPU hardware characteristics are documented here.
		*/
		typedef enum HardwareConstants
		{
			kThreadsPerWavefront			  = 64,       ///< There are 64 threads in a wavefront, which executes each instruction across all threads in parallel.
			kCacheLineSizeInBytes			  = 64,       ///< The GPU cache has lines that are 64 bytes wide.
			kIndirectBufferMaximumSizeInBytes = 0x3FFFFC, ///< (= 4*1024*1024 - 4) An indirect buffer may be almost four megabytes in size.
			kDmaMaximumSizeInBytes            = 0x1FFFFC, ///< (= (1<<21)-4) A DMA transfer may be almost two megabytes in size and must be a multiple of 4 bytes.
			kNumCusPerSe                      = 9,        ///< Each of the 2 shader engines (SE) has 9 compute units (CUs) enabled.
			kGdsAccessibleMemorySizeInBytes   = 0xBF00,   ///< Accessible GDS memory size in bytes.
			kGdsAccessibleOrderedAppendCounters = 8,      ///< Accessible count of GDS OA counters. Please see DispatchCommandBuffer or DrawCommandBuffer::enableOrderedAppendAllocationCounter().
			kCpramSizeInBytes                 = 0xC000,   ///< CPRAM size in bytes.
			kTextureMaximumWidth              = 16384,    ///< The maximum width of a texture in texels.
			kTextureMaximumHeight             = 16384,    ///< The maximum height of a texture in texels.
			kTextureMaximumDepth              = 8192,     ///< The maximum depth of a texture in texels.
			kPixelMaximumSamples              = 16,       ///< The maximum number of samples in a pixel.
			kPixelMaximumFragments            = 8,        ///< The maximum number of fragments in a pixel.
			kChannelTypeMinimumBitsPerChannel = 6,        ///< Below this number of bits per channel, ChannelType is disregarded. (Unorm is assumed.)
		} HardwareConstants;

		/** @brief Specifies the modes that the PlayStation®4 GPU supports. 

				Not all PlayStation®4 systems support all GPU modes. */
		typedef enum GpuMode
		{
			kGpuModeBase = 0, ///< GPU mode that Standard PlayStation®4 systems use.
			kGpuModeNeo  = 1, ///< GPU mode that PlayStation®4 Pro systems use.
		} GpuMode;
		
		/** @brief Defines the possible sizes of a Tessellation Factor Ring Buffer.
		*/
		typedef enum RingSizeInBytes
		{
			kTfRingSizeInBytes   = 0x20000,   ///< Fixed size for Tessellation Factor Ring Buffer (128KB)
		} RingSizeInBytes;

		/** @brief Valid sizes for the system-wide ESGS or GSVS buffers. 

					Large sizes are required only when geometry shader inputs and outputs have a very large number of parameters per vertex.
		    @see Gnm::setGsRingSizes()
		*/
		typedef enum GsRingSizeSetup
		{
			kGsRingSizeSetup4Mb  = 0x0400000, ///< Ring size of 4 MB.  (Default Settings for ES/GS and GS/VS.)
			kGsRingSizeSetup5Mb  = 0x0500000, ///< Ring size of 5 MB.
			kGsRingSizeSetup6Mb  = 0x0600000, ///< Ring size of 6 MB.
			kGsRingSizeSetup7Mb  = 0x0700000, ///< Ring size of 7 MB.
			kGsRingSizeSetup8Mb  = 0x0800000, ///< Ring size of 8 MB.
		} GsRingSizeSetup;

		/** @brief Specifies which mode should be used by a DrawCommandBuffer when writing: graphics or compute.
			@see DrawCommandBuffer::setShaderType(), DrawCommandBuffer::setBaseIndirectArgs()
		*/
		typedef enum ShaderType
		{
			kShaderTypeGraphics                         = 0x00000000, ///< Configures command buffer for graphics commands.
			kShaderTypeCompute                          = 0x00000001, ///< Configures command buffer for compute commands.
		} ShaderType;

		/** @brief Specifies a hardware shader engine.
			*/
		typedef enum ShaderEngine
		{
			kShaderEngine0                              = 0, ///< Shader Engine 0.
			kShaderEngine1                              = 1, ///< Shader Engine 1.
			kShaderEngine2                              = 2, ///< Shader Engine 2. (NEO mode only)
			kShaderEngine3                              = 3, ///< Shader Engine 3. (NEO mode only)
		} ShaderEngine;

		/** @brief Specifies how certain commands should be broadcast to the hardware shader engines.
		*/
		typedef enum ShaderEngineBroadcast
		{
			kShaderEngineBroadcastAll					=-1,	///< Broadcast to all hardware unit instances.
			kShaderEngineBroadcast0						= 0,	///< Broadcast to all hardware unit instances on shader engine (SE) 0.
			kShaderEngineBroadcast1						= 1,	///< Broadcast to all hardware unit instances on shader engine (SE) 1.
			kShaderEngineBroadcast2						= 2,	///< Broadcast to all hardware unit instances on shader engine (SE) 2.
			kShaderEngineBroadcast3						= 3,	///< Broadcast to all hardware unit instances on shader engine (SE) 3.
		} ShaderEngineBroadcast;

		typedef enum _EmbeddedTag
		{
			kMarkerEmbeddedData							= 0x68750000,
		} _EmbeddedTag;

		typedef enum EmbeddedDataType
		{
			kEmbeddedDataTypeUndefined                  = 0x000,

			kPerfMonMarkerPush                          = 0x001,
			kPerfMonMarkerPop                           = 0x002,
			kPerfMonMarkerSet                           = 0x003,
			kPerfMonVsharpUserData						= 0x004,
			kPerfMonTsharpUserData						= 0x005,
			kPerfMonSsharpUserData						= 0x006,
			kPerfMonMarkerPerfCounterSettingPush		= 0x007,
			kPerfMonMarkerPerfCounterSettingPop			= 0x008,
			kPerfMonMarkerPerfCounterReadPush			= 0x009,
			kPerfMonMarkerPerfCounterReadPop			= 0x00A,
			kPerfMonMarkerPushRttvFormat				= 0x00B,
			kPerfMonMarkerSetRttvFormat		    		= 0x00C,
			kPerfMonSetUserDataRegion	    		    = 0x00D,
			kPerfMonMarkerColoredPush                   = 0x00E,
			kPerfMonMarkerColoredSet                    = 0x00F,
			kPerfMonMarkerColoredPushRttvFormat			= 0x010,
			kPerfMonMarkerColoredSetRttvFormat		    = 0x011,
		} EmbeddedDataType;

		/** @brief Specifies the alignment of data within a command buffer.
			@see DrawCommandBuffer::allocateFromCommandBuffer(), DispatchCommandBuffer::allocateFromCommandBuffer()
		*/
		typedef enum EmbeddedDataAlignment
		{
			kEmbeddedDataAlignment4                     = 0x2, ///< Align to a 4-byte boundary.
			kEmbeddedDataAlignment8                     = 0x3, ///< Align to an 8-byte boundary.
			kEmbeddedDataAlignment16                    = 0x4, ///< Align to a 16-byte boundary.
			kEmbeddedDataAlignment32					= 0x5, ///< Align to a 32-byte boundary.
			kEmbeddedDataAlignment64                    = 0x6, ///< Align to a 64-byte boundary.
			kEmbeddedDataAlignment128                   = 0x7, ///< Align to a 128-byte boundary.
			kEmbeddedDataAlignment256                   = 0x8, ///< Align to a 256-byte boundary.
		} EmbeddedDataAlignment;

		/** @brief Specifies the index size of the indices passed to the draw calls.
		*/
		typedef enum IndexSize
		{
			kIndexSize16                                = 0x00000000, ///< 16 bit indices via DMA.
			kIndexSize32                                = 0x00000001, ///< 32 bit indices via DMA.
		} IndexSize;

		/** @brief Specifies the shader stage when setting shaders and their respective resources and constants.
		*/
		typedef enum ShaderStage
		{
			kShaderStageCs                              = 0x00000000,	///< Compute shader stage.
			kShaderStagePs                              = 0x00000001,	///< Pixel shader stage.
			kShaderStageVs                              = 0x00000002,	///< Vertex shader stage.
			kShaderStageGs                              = 0x00000003,	///< Geometry shader stage.
			kShaderStageEs                              = 0x00000004,	///< Export shader stage.
			kShaderStageHs                              = 0x00000005,	///< Hull shader stage.
			kShaderStageLs                              = 0x00000006,	///< LDS shader stage.

			kShaderStageCount											///< The number of shader stages.
		} ShaderStage;

		/** @brief Specifies which stages should be activated in the graphics shader pipeline.
		*/
		typedef enum ActiveShaderStages
		{
			kActiveShaderStagesVsPs                     = 0x00000000, ///< VS/PS only.
			kActiveShaderStagesEsGsVsPs                 = 0x000000B0, ///< Geometry shader followed by VS/PS.
			kActiveShaderStagesLsHsVsPs                 = 0x00000045, ///< Tessellation followed by VS/PS.
			kActiveShaderStagesOffChipLsHsVsPs			= 0x00000145, ///< Off-chip tessellation followed by VS/PS
			kActiveShaderStagesLsHsEsGsVsPs             = 0x000000AD, ///< Tessellation followed by the geometry shader followed by VS/PS.
			kActiveShaderStagesOffChipLsHsEsGsVsPs		= 0x000001AD, ///< Off-chip tessellation, followed by the geometry shader, followed by VS/PS.
		} ActiveShaderStages;

		/** @brief Defines the shader compiler predefined slot.
			@see Gnm::AaSampleInfo, Gnm::AaSampleLocation
		*/
		typedef enum ShaderCompilerPredefinedSlot
		{
			kShaderSlotEmbeddedConstantBuffer	  = 15, /// Description to be specified.
			kShaderSlotAaSampleInfoConstantBuffer	  = 16,	///< Slot for array of Gnm::AaSampleInfo structures.
			kShaderSlotAaSampleLocationConstantBuffer = 17,	///< Slot for array of Gnm::AaSampleLocation structures.
			kShaderSlotTessellationDataConstantBuffer = 19,	///< Slot for the tessellation data constant buffer
		} ShaderCompilerPredefinedSlot;

		/** @brief Describes a data resource expected by a shader.

			Each input must be bound by the application before the shader runs.
		*/
		typedef enum ShaderInputUsageType
		{
			kShaderInputUsageImmResource                = 0x00, ///< Immediate read-only buffer/texture descriptor.
			kShaderInputUsageImmSampler			        = 0x01, ///< Immediate sampler descriptor.
			kShaderInputUsageImmConstBuffer             = 0x02, ///< Immediate constant buffer descriptor.
			kShaderInputUsageImmVertexBuffer            = 0x03, ///< Immediate vertex buffer descriptor.
			kShaderInputUsageImmRwResource				= 0x04, ///< Immediate read/write buffer/texture descriptor.
			kShaderInputUsageImmAluFloatConst		    = 0x05, ///< Immediate float const (scalar or vector).
			kShaderInputUsageImmAluBool32Const		    = 0x06, ///< 32 immediate Booleans packed into one UINT.
			kShaderInputUsageImmGdsCounterRange	        = 0x07, ///< Immediate UINT with GDS address range for counters (used for append/consume buffers).
			kShaderInputUsageImmGdsMemoryRange		    = 0x08, ///< Immediate UINT with GDS address range for storage.
			kShaderInputUsageImmGwsBase                 = 0x09, ///< Immediate UINT with GWS resource base offset.
			kShaderInputUsageImmShaderResourceTable     = 0x0A, ///< Pointer to read/write resource indirection table.
			kShaderInputUsageImmLdsEsGsSize             = 0x0D, ///< Immediate LDS ESGS size used in on-chip GS
			// Skipped several items here...
			kShaderInputUsageSubPtrFetchShader		    = 0x12, ///< Immediate fetch shader subroutine pointer.
			kShaderInputUsagePtrResourceTable           = 0x13, ///< Flat resource table pointer.
			kShaderInputUsagePtrInternalResourceTable   = 0x14, ///< Flat internal resource table pointer.
			kShaderInputUsagePtrSamplerTable		    = 0x15, ///< Flat sampler table pointer.
			kShaderInputUsagePtrConstBufferTable	    = 0x16, ///< Flat const buffer table pointer.
			kShaderInputUsagePtrVertexBufferTable       = 0x17, ///< Flat vertex buffer table pointer.
			kShaderInputUsagePtrSoBufferTable		    = 0x18, ///< Flat stream-out buffer table pointer.
			kShaderInputUsagePtrRwResourceTable		    = 0x19, ///< Flat read/write resource table pointer.
			kShaderInputUsagePtrInternalGlobalTable     = 0x1A, ///< Internal driver table pointer.
			kShaderInputUsagePtrExtendedUserData        = 0x1B, ///< Extended user data pointer.
			kShaderInputUsagePtrIndirectResourceTable   = 0x1C, ///< Pointer to resource indirection table.
			kShaderInputUsagePtrIndirectInternalResourceTable = 0x1D, ///< Pointer to internal resource indirection table.
			kShaderInputUsagePtrIndirectRwResourceTable = 0x1E, ///< Pointer to read/write resource indirection table.
		} ShaderInputUsageType;


		/** @brief Indices used when <c>V#</c>'s are set for scratch buffers or ring buffers.	*/
		typedef enum ShaderGlobalResourceType
		{
			kShaderGlobalResourceScratchRingForGraphic = 0x00,
			kShaderGlobalResourceScratchRingForCompute = 0x01,
			kShaderGlobalResourceEsGsWriteDescriptor   = 0x02,
			kShaderGlobalResourceEsGsReadDescriptor	   = 0x03,
			kShaderGlobalResourceGsVsWriteDescriptor0  = 0x04,
			kShaderGlobalResourceGsVsWriteDescriptor1  = 0x05,
			kShaderGlobalResourceGsVsWriteDescriptor2  = 0x06,
			kShaderGlobalResourceGsVsWriteDescriptor3  = 0x07,
			kShaderGlobalResourceGsVsReadDescriptor	   = 0x08,
			kShaderGlobalResourceTessFactorBuffer	   = 0x09,
			kShaderGlobalResourceOffChipLds0		   = 0x0A,
			kShaderGlobalResourceOffChipLds1		   = 0x0B,
			
			kShaderGlobalResourceCount				   = 0x0C,
		} ShaderGlobalResourceType;

		/** @brief Specifies how a fetch shader should handle vertex instancing. 
			@note Step rates are not affected by the instance offset in draw commands. That is, the <c>kFetchShaderUseInstanceIdOverStepRate0</c>/<c>kFetchShaderUseInstanceIdOverStepRate1</c> indexed elements will be fetched as if the index offset is <c>0</c>.
		*/
		typedef enum FetchShaderInstancingMode
		{
			kFetchShaderUseVertexIndex              = 0x0, ///< No instancing; use Vertex Index. Default.
			kFetchShaderUseInstanceId               = 0x1, ///< Use the instance ID to index the data.
			kFetchShaderUseInstanceIdOverStepRate0  = 0x2, ///< Use the (instance ID / step rate 0) to index the data.
			kFetchShaderUseInstanceIdOverStepRate1  = 0x3, ///< Use the (instance ID / step rate 1) to index the data.
		} FetchShaderInstancingMode;

		/** @brief Defines the special fetch shader build state modes. */
		typedef enum FetchShaderBuildStateMode
		{
			kFetchShaderBuildStateIsLS				= 0x1,	///< The fetch shader is to be used on the LS stage.
		} FetchShaderBuildStateMode;

		/**
		* @brief Specifies the Geometry shader mode.
		* @sa DrawCommandBuffer::setGsMode()
		*/
		typedef enum GsMode
		{
			kGsModeDisable                           = 0x00000000, ///< Geometry shader is off.
			kGsModeEnable                            = 0x00180003, ///< Geometry shader is on. Data between stages goes through ring buffer in GDDR5. Optimized for write-combiner.
			kGsModeEnableOnChip                      = 0x00600003, ///< Geometry shader is on. Data between stages goes through LDS.
		} GsMode;

		/**
		* @brief Specifies the maximum amount of data per primitive that can be emitted by a single GS stage thread.
		* @sa DrawCommandBuffer::setGsMode()
		*/
		typedef enum GsMaxOutputPrimitiveDwordSize
		{
			kGsMaxOutputPrimitiveDwordSize1024                 = 0x00000000, ///< GS stage output is <= 1024 <c>DWORD</c>s per primitive.
			kGsMaxOutputPrimitiveDwordSize512                  = 0x00000001, ///< GS stage output is <=  512 <c>DWORD</c>s per primitive.
			kGsMaxOutputPrimitiveDwordSize256                  = 0x00000002, ///< GS stage output is <=  256 <c>DWORD</c>s per primitive.
			kGsMaxOutputPrimitiveDwordSize128                  = 0x00000003, ///< GS stage output is <=  128 <c>DWORD</c>s per primitive.
		} GsMaxOutputPrimitiveDwordSize;
		// typedef SCE_GNM_API_DEPRECATED_ENUM_MSG("Use GsMaxOutputPrimitiveDwordSize instead.") enum GsMaxOutputPrimitiveDwordSize GsMaxOutputVertexCount; // DEPRECATED
		
		/** @brief Flags that specify the GPU memory write targets for which a surface synchronization operation should wait.

			For example, if <c>kWaitTargetSlotCb0</c> is specified,
			surface synchronization will stall until all GPU jobs that write to Render Target 0 have completed. The writes themselves may still be cached at this time.

			These flags can be bitwise-OR'd together and passed to DrawCommandBuffer::waitForGraphicsWrites().

			@see DrawCommandBuffer::waitForGraphicsWrites()
			*/
		typedef enum WaitTargetSlot
		{
			kWaitTargetSlotCb0                             = 0x00000040, ///< Render Target 0.
			kWaitTargetSlotCb1                             = 0x00000080, ///< Render Target 1.
			kWaitTargetSlotCb2                             = 0x00000100, ///< Render Target 2.
			kWaitTargetSlotCb3                             = 0x00000200, ///< Render Target 3.
			kWaitTargetSlotCb4                             = 0x00000400, ///< Render Target 4.
			kWaitTargetSlotCb5                             = 0x00000800, ///< Render Target 5.
			kWaitTargetSlotCb6                             = 0x00001000, ///< Render Target 6.
			kWaitTargetSlotCb7                             = 0x00002000, ///< Render Target 7.
			kWaitTargetSlotDb                              = 0x00004000, ///< Depth Target.

			kWaitTargetSlotAll                             = 0x00007FC0  ///< All render targets and depth target. Should be used only for debugging purposes.
		} WaitTargetSlot;

		/** @brief Functions to use for comparing a specified value with a value in memory.
		*/
		typedef enum WaitCompareFunc
		{
			kWaitCompareFuncAlways                        = 0x00000000, ///< Do not wait.
			kWaitCompareFuncLess                          = 0x00000001, ///< Wait until the value in memory is less than the specified value.
			kWaitCompareFuncLessEqual                     = 0x00000002, ///< Wait until the value in memory is less than or equal to the specified value.
			kWaitCompareFuncEqual                         = 0x00000003, ///< Wait until the value in memory is equal to the specified value.
			kWaitCompareFuncNotEqual                      = 0x00000004, ///< Wait until the value in memory is not equal to the specified value.
			kWaitCompareFuncGreaterEqual                  = 0x00000005, ///< Wait until the value in memory is greater than or equal to the specified value.
			kWaitCompareFuncGreater                       = 0x00000006, ///< Wait until the value in memory is greater than the specified value.
		} WaitCompareFunc;

		/** @brief Specifies whether certain commands should stall command buffer parsing.
		
		    Use these values to enable or disable command buffer parsing.
			@note In addition to fetching command buffer data, the CP prefetches the index data referenced by subsequent draw calls.
			      Stalling the command buffer parsing will also prevent this index prefetching, which may be necessary if the index data is
			      dynamically generated or uploaded just-in-time by a previous command.
			@sa Gnm::DispatchCommandBuffer::flushShaderCachesAndWait(), Gnm::DrawCommandBuffer::flushShaderCachesAndWait(),
			    Gnmx::ComputeContext::flushShaderCachesAndWait(), Gnmx::GfxContext::flushShaderCachesAndWait(), 
				Gnm::DispatchCommandBuffer::waitForGraphicsWrites(), Gnm::DrawCommandBuffer::waitForGraphicsWrites(), 
			    Gnmx::ComputeContext::waitForGraphicsWrites(), Gnmx::GfxContext::waitForGraphicsWrites()
		*/
		typedef enum StallCommandBufferParserMode
		{
			kStallCommandBufferParserEnable               = 0x00000000,  ///< Enables stalling of command buffer parsing.
			kStallCommandBufferParserDisable              = 0x00000001,  ///< Disables stalling of command buffer parsing.
		} StallCommandBufferParserMode;

		/** @brief Specifies the event for which to write statistics.
			
			@see Gnm::DrawCommandBuffer::writeEventStats()
			*/
		typedef enum EventStats
		{
			kEventStatsZPassDone                        = 0x0,  ///< Description to be specified.
			kEventStatsSamplePipelinestat               = 0x1,	///< write out the accumulated pipeline statisics. See Gnm::PipelineStats structure.
			kEventStatsSampleStreamoutstats0            = 0x2,  ///< Description to be specified.
			kEventStatsSampleStreamoutstats1            = 0x3,	///< Description to be specified.
			kEventStatsSampleStreamoutstats2            = 0x4,	///< Description to be specified.
			kEventStatsSampleStreamoutstats3            = 0x5,	///< Description to be specified.
		} EventStats;

		/** @brief Defines reset forcing options for Mip statistics.
		 */
		typedef enum MipStatsResetForce
		{
			kMipStatsResetForceDisable = 0x00000000,	///< Disable forced resets for Mip statistics.	 
			kMipStatsResetForceEnable  = 0x00000001,	///< Enable forced resets for Mip statistics.	 
		} MipStatsResetForce;

		
		/** @brief Defines the types of Mip statistics errors that can occur.
		 */
		typedef enum MipStatsError
		{
			kMipStatsErrorNone                 = 0,			 ///< No error occurred. 				 
			kMipStatsErrorInvalidBufferAddress = 0x80000001, ///< The output buffer base address must have low six bits and high twenty-four bits set to zero.
			kMipStatsErrorInvalidBufferSize    = 0x80000002, ///< The size must be at least 4096 + 64 bytes and equals to N*2048 + 64 bytes.
			kMipStatsErrorInternalError        = 0x8000000F, ///< Internal error.
		} MipStatsError;

		/** @brief Describes the memory type used by a graphic resource (Buffer or Texture).

			For a more thorough description of each memory type, see "Gnm Library Overview - Cache Protocol”.

			@cond For a text-only, non-XML version of the following table, see the single-line comments in the declaration of the enum itself, below. @endcond

			<table>
			<tr><th>Value</th> <th>Memory Type</th> <th>L2$ Mode</th> <th>Memory Bus</th> <th>L1$ Default Behavior</th> <th>K$ Default Behavior</th></tr>
			<tr><td>kResourceMemoryTypePV = 0x60</td><td>Private</td><td>Non Volatile</td><td>Garlic Recommended</td><td>LRU</td><td> Bypass</td></tr>
			<tr><td>kResourceMemoryTypeGC = 0x6D</td><td>GPU Coherent</td><td>Non Volatile</td><td>Garlic Recommended</td><td>Bypass</td><td>Bypass</td></tr>
			<tr><td>kResourceMemoryTypeSC = 0x6E</td><td>System Coherent</td><td>Volatile</td><td>Onion Recommended</td><td>Bypass</td><td>Bypass</td></tr>
			<tr><td>kResourceMemoryTypeUC = 0x6F</td><td>Uncached</td><td>Not Applicable</td><td>Onion Only</td><td>Bypass</td><td>Bypass</td></tr>
			<tr><td>kResourceMemoryTypeRO = 0x10</td><td>Read Only</td><td>Non Volatile</td><td>Garlic Recommended</td><td>LRU</td><td>LRU</td></tr>
			</table>

			@note The L1 and K cache behaviors described in the table above are the default behaviors for the five different resource memory types.
			However, Buffer::setResourceMemoryType() does offer the ability to override both while Texture::setResourceMemoryType() offers the
			ability to override the L1 cache behavior. Regarding the memory bus column, “Garlic Recommended” and “Onion Recommended” is
			specified when this will result in the best performance. Where “Onion Only” is specified, ignoring this advice will result in undefined behavior.
			@see Buffer::setResourceMemoryType(), Texture::setResourceMemoryType()
		 */
		typedef enum ResourceMemoryType
		{
										  // Memory Type     | L2$ Mode       | Memory Bus         | L1$ Default Behavior | K$ Default Behavior
										  // ----------------+----------------+--------------------+----------------------+--------------------
			kResourceMemoryTypePV = 0x60, // Private         | Non Volatile   | Garlic Recommended | LRU                  | Bypass
			kResourceMemoryTypeGC = 0x6D, // GPU Coherent    | Non Volatile   | Garlic Recommended | Bypass               | Bypass
			kResourceMemoryTypeSC = 0x6E, // System Coherent | Volatile       | Onion Recommended  | Bypass               | Bypass
			kResourceMemoryTypeUC = 0x6F, // Uncached        | Not Applicable | Onion Only         | Bypass               | Bypass
			kResourceMemoryTypeRO = 0x10, // Read Only       | Non Volatile   | Garlic Recommended | LRU                  | LRU
		} ResourceMemoryType;

		/** @brief Specifies the number of samples per pixel in a render surface.

			For Texture objects, this is the actual number of values stored per pixel. For RenderTarget objects, this is the number of
			"virtual" samples into the target's fragments.
			@see NumFragments
			*/
		typedef enum NumSamples
		{
			kNumSamples1                                = 0x0, ///< 1 sample per pixel.
			kNumSamples2                                = 0x1, ///< 2 samples per pixel.
			kNumSamples4                                = 0x2, ///< 4 samples per pixel.
			kNumSamples8                                = 0x3, ///< 8 samples per pixel.
			kNumSamples16                               = 0x4, ///< 16 samples per pixel.
		} NumSamples;

		/** @brief Specifies the number of fragments per pixel in a render target.

			For RenderTarget and DepthRenderTarget objects, this is the actual number of values stored per pixel. Texture objects use the NumSamples enum for this purpose instead.
			@see NumSamples
			*/
		typedef enum NumFragments
		{
			kNumFragments1                              = 0x0, ///< 1 fragment per pixel.
			kNumFragments2                              = 0x1, ///< 2 fragments per pixel.
			kNumFragments4                              = 0x2, ///< 4 fragments per pixel.
			kNumFragments8                              = 0x3, ///< 8 fragments per pixel.
		} NumFragments;

		/** @brief Defines the possible pixels within a 2x2 pixel quad. */
		typedef enum QuadPixel
		{
			kQuadPixelUpperLeft  = 0, ///< The upper-left pixel in a 2x2 quad.
			kQuadPixelUpperRight = 1, ///< The upper-right pixel in a 2x2 quad.
			kQuadPixelLowerLeft  = 2, ///< The lower-left pixel in a 2x2 quad.
			kQuadPixelLowerRight = 3, ///< The lower-right pixel in a 2x2 quad.
		} QuadPixel;

		/** @brief Defines the factors which can be used to scale the effect of several Sampler performance/quality tradeoffs. For example:

				<ul>
				<li>Sampler::setMipClampRegionSize()</li>
				<li>Sampler::setZClampRegionSize()</li>
				<li>Sampler::setAnisotropyBias()</li>
				<li>Sampler::setAnisotropyThreshold()</li>
				<li>Sampler::setLodBias() - secondary bias only</li>
				</ul>
			
			@see Texture::setSamplerModulationFactor(), Texture::getSamplerModulationFactor()

			*/
		typedef enum SamplerModulationFactor
		{
			kSamplerModulationFactor0_0000           = 0x0, ///< Scale associated values by 0/16. This setting invalidates the parameter.
			kSamplerModulationFactor0_1250           = 0x1, ///< Scale associated values by 2/16.
			kSamplerModulationFactor0_3125           = 0x2, ///< Scale associated values by 5/16.
			kSamplerModulationFactor0_4375           = 0x3, ///< Scale associated values by 7/16.
			kSamplerModulationFactor0_5625           = 0x4, ///< Scale associated values by 9/16.
			kSamplerModulationFactor0_6875           = 0x5, ///< Scale associated values by 11/16.
			kSamplerModulationFactor0_8750           = 0x6, ///< Scale associated values by 14/16.
			kSamplerModulationFactor1_0000           = 0x7, ///< Scale associated values by 16/16. This setting effectively leaves the associated tweaks unscaled.
		} SamplerModulationFactor;


		/** @brief Enable or disable the sign flip of the gradient factor: factor01.
			@see setTextureGradientFactors
		 */
		typedef enum TextureGradientFactor01SignNegationBehavior
		{
			kTextureGradientFactor01SignNegationBehaviorDisabled = 0, ///< factor01 will remain unchanged.
			kTextureGradientFactor01SignNegationBehaviorEnabled  = 1, ///< factor01 will be negated, even if directed by the shader SampleAdjust instruction.
		} TextureGradientFactor01SignNegationBehavior;

		/** @brief Enable or disable the sign flip of the gradient factor: factor10.
			@see setTextureGradientFactors
		 */
		typedef enum TextureGradientFactor10SignNegationBehavior
		{
			kTextureGradientFactor10SignNegationBehaviorDisabled = 0, ///< factor10 will remain unchanged.
			kTextureGradientFactor10SignNegationBehaviorEnabled  = 1, ///< factor01 will be negated, even if directed by the shader SampleAdjust instruction.
		} TextureGradientFactor10SignNegationBehavior;

		/** @brief Specifies the number of contiguous buffer elements to swizzle as a single block, if buffer swizzling is enabled.
			@see Buffer::setSwizzle()
			*/
		typedef enum BufferSwizzleStride
		{
			kBufferSwizzleStride8                         = 0x0, ///< 8 records swizzled together.
			kBufferSwizzleStride16                        = 0x1, ///< 16 records swizzled together.
			kBufferSwizzleStride32                        = 0x2, ///< 32 records swizzled together.
			kBufferSwizzleStride64                        = 0x3, ///< 64 records swizzled together.
		} BufferSwizzleStride;

		/** @brief Specifies the amount of data to treat as an atomic chunk when swizzling a block of buffer data, if buffer swizzling is enabled.
		    
			@note Because there is no <c>ElementSize1</c> or <c>ElementSize12</c>, it is not possible to swizzle DataBuffers with a SurfaceFormat of those sizes.
			This is because the elements remain packed tightly in memory. SurfaceFormats with sizes <c>1</c> and <c>12</c> include #kSurfaceFormat8 and #kSurfaceFormat32_32_32.

			@see Buffer::setSwizzle()
			*/
		typedef enum BufferSwizzleElementSize
		{
			kBufferSwizzleElementSize2                    = 0x0, ///< 2-byte swizzle elements.
			kBufferSwizzleElementSize4                    = 0x1, ///< 4-byte swizzle elements.
			kBufferSwizzleElementSize8                    = 0x2, ///< 8-byte swizzle elements.
			kBufferSwizzleElementSize16                   = 0x3, ///< 16-byte swizzle elements.
		} BufferSwizzleElementSize;

		/** @brief Enables or disables the window offset mode when specifying a scissor rectangle.

			If enabled, the rectangle is offset by the window coordinates.
			@see DrawCommandBuffer::setWindowScissor(), DrawCommandBuffer::setGenericScissor(), DrawCommandBuffer::setViewportScissor()
			*/
		typedef enum WindowOffsetMode
		{
			kWindowOffsetDisable                        = 0, ///< Window offset is disabled.
			kWindowOffsetEnable                         = 1, ///< Window offset is enabled.
		} WindowOffsetMode;

		/** @brief Specifies how often a PS shader runs for each pixel.
			@see DrawCommandBuffer::setPsShaderRate()
			*/
		typedef enum PsShaderRate
		{
			kPsShaderRatePerPixel                       = 0, ///< PS shader is executed once per pixel, regardless of the sample count.
			kPsShaderRatePerSample                      = 1, ///< PS shader is executed once per sample.
		} PsShaderRate;

		/** @brief Enables or disables multisample antialiasing (MSAA).
			@see DrawCommandBuffer::setScanModeControl()
			*/
		typedef enum ScanModeControlAa
		{
			kScanModeControlAaDisable                   = 0, ///< Disables MSAA.
			kScanModeControlAaEnable                    = 1, ///< Enables MSAA.
		} ScanModeControlAa;

		/** @brief Enables or disables scissor rectangles for a given viewport.
			@see DrawCommandBuffer::setScanModeControl(), DrawCommandBuffer::setViewportScissor()
			*/
		typedef enum ScanModeControlViewportScissor
		{
			kScanModeControlViewportScissorDisable      = 0, ///< Disables the scissor rectangle for a viewport.
			kScanModeControlViewportScissorEnable       = 1, ///< Enables the scissor rectangle for a viewport.
		} ScanModeControlViewportScissor;

		/** @brief Specifies which primitives should be culled based on their facing.
			@see PrimitiveSetup::setCullFace()
			*/
		typedef enum PrimitiveSetupCullFaceMode
		{
			kPrimitiveSetupCullFaceNone              = 0, ///< Disable face culling.
			kPrimitiveSetupCullFaceFront             = 1, ///< Cull front-facing primitives only.
			kPrimitiveSetupCullFaceBack              = 2, ///< Cull back-facing primitives only.
			kPrimitiveSetupCullFaceFrontAndBack      = 3, ///< Cull front and back faces.
		} PrimitiveSetupCullFaceMode;

		/** @brief Specifies which winding order corresponds to front-facing primitives.
			@see PrimitiveSetup::setFrontFace()
			*/
		typedef enum PrimitiveSetupFrontFace
		{
			kPrimitiveSetupFrontFaceCw                     = 1, ///< Clockwise is front-facing.
			kPrimitiveSetupFrontFaceCcw                    = 0, ///< Counter-clockwise is front-facing.
		} PrimitiveSetupFrontFace;

		/** @brief Specifies how each polygon is rendered.
			@see PrimitiveSetup::setPolygonMode()
			*/
		typedef enum PrimitiveSetupPolygonMode
		{
			kPrimitiveSetupPolygonModePoint              = 0, ///< Render polygons as points.
			kPrimitiveSetupPolygonModeLine               = 1, ///< Render polygons in wireframe.
			kPrimitiveSetupPolygonModeFill               = 2, ///< Render polygons as solid/filled.
		} PrimitiveSetupPolygonMode;

		/** @brief Specifies whether polygon offsets are enabled for a given facedness.
			@see PrimitiveSetup::setPolygonOffsetEnable()
			*/
		typedef enum PrimitiveSetupPolygonOffsetMode
		{
			kPrimitiveSetupPolygonOffsetEnable           = 1, ///< Enable polygon offset.
			kPrimitiveSetupPolygonOffsetDisable          = 0, ///< Disable polygon offset.
		} PrimitiveSetupPolygonOffsetMode;

		/** @brief Specifies which vertex in a primitive should be the source of attributes when flat-shading mode is active.
			@see PrimitiveSetup::setProvokingVertex()
			*/
		typedef enum PrimitiveSetupProvokingVertexMode
		{
			kPrimitiveSetupProvokingVertexFirst       = 0, ///< First vertex of a primitive is used for attribute components when flat shading is enabled.
			kPrimitiveSetupProvokingVertexLast        = 1, ///< Last vertex of a primitive is used for attribute components when flat shading is enabled.
		} PrimitiveSetupProvokingVertexMode;

		/** @brief Specifies the clipping/culling behavior of user clip planes.
			@see ClipControl::setUserClipPlaneMode()
			*/
		typedef enum ClipControlUserClipPlaneMode
		{
			kClipControlUserClipPlaneCenterNoBias                  = 0, ///< Points are culled solely based on the center point of the point or vertex. No clipping occurs.
			kClipControlUserClipPlaneCenterRadiusBias              = 1, ///< Distance from the clip plane to the point is compared to the point radius to decide whether to cull. No clipping occurs.
			kClipControlUserClipPlaneCenterRadisBiasAndExpand      = 2, ///< Points are first tested to see if they would be culled. If not point is expanded if it needs to be clipped.
			kClipControlUserClipPlaneCenterAlwaysExpand            = 3, ///< Points are automatically expanded and run through the clip sequence.
		} ClipControlUserClipPlaneMode;

		/** @brief Specifies the clip space range for vertex Z values.
			@see ClipControl::setClipSpace()
			*/
		typedef enum ClipControlClipSpace
		{
			kClipControlClipSpaceDX                     = 1, ///< DirectX-style clip space: 0 <= Z <= W.
			kClipControlClipSpaceOGL                    = 0, ///< OpenGL-style clip space: -W <= Z <= W.
		} ClipControlClipSpace;

		/** @brief Specifies whether clipping is enabled for a Z plane.
			@see ClipControl::setZClipEnable()
			*/
		typedef enum ClipControlZClipMode
		{
			kClipControlZClipDisable                = 1, ///< Disables Z clipping against the specified Z plane.
			kClipControlZClipEnable                 = 0, ///< Enables clipping against the specified Z plane.
		} ClipControlZClipMode;

		/** @brief Specifies how many vertices must be killed by a vertex shader before the associated primitive is culled.
			@see ClipControl::setVertexKillMode()
			*/
		typedef enum ClipControlVertexKillMode
		{
			kClipControlVertexKillModeAny               = 1, ///< A triangle will be culled if any of its vertices are killed by the vertex shader.
			kClipControlVertexKillModeAll               = 0, ///< A triangle will only be culled if all of its vertices are killed by the vertex shader.
		} ClipControlVertexKillMode;

		/** @brief Enables or disables alpha-to-mask mode.
			@see DrawCommandBuffer::setAlphaToMaskControl(), AlphaToMaskControl::setEnabled()
			*/
		typedef enum AlphaToMaskMode
		{
			kAlphaToMaskDisable                         = 0, ///< Disables alpha-to-mask mode.
			kAlphaToMaskEnable                          = 1, ///< Enables alpha-to-mask mode.
		} AlphaToMaskMode;

		/** @brief Represents the possible dither thresholds for one pixel in a 2x2 quad when alpha-to-mask is enabled.
		 *  @see AlphaToMaskControl::setPixelDitherThresholds()
		 */
		typedef enum AlphaToMaskDitherThreshold
		{
			kAlphaToMaskDitherThreshold0                = 0, ///< The alpha-to-mask threshold for this pixel-in-quad is 0.00.
			kAlphaToMaskDitherThreshold1                = 1, ///< The alpha-to-mask threshold for this pixel-in-quad is 0.25.
			kAlphaToMaskDitherThreshold2                = 2, ///< The alpha-to-mask threshold for this pixel-in-quad is 0.50.
			kAlphaToMaskDitherThreshold3                = 3, ///< The alpha-to-mask threshold for this pixel-in-quad is 0.75.
			kAlphaToMaskDitherDefault                   = 2, ///< The alpha-to-mask threshold for this pixel-in-quad is 0.50.

			// kAlphaToMaskDitherDisabled SCE_GNM_API_DEPRECATED_ENUM_MSG("Use kAlphaToMaskDitherDefault instead.") = 2, ///< Alpha-to-mask dithering is disabled for this pixel-in-quad.
		} AlphaToMaskDitherThreshold;

		/** @brief Enables or disables alpha-to-mask dithering.
			@see AlphaToMaskControl::setDitherMode()
			*/
		typedef enum AlphaToMaskDitherMode
		{
			kAlphaToMaskDitherModeDisabled                = 0, ///< A non-dithered look.
			kAlphaToMaskDitherModeEnabled                 = 1, ///< A dithered look.
		} AlphaToMaskDitherMode;

		/** @brief Determines when the vertex geometry tessellator (VGT) should emit a new VS wave.
			@see DrawCommandBuffer::setVgtControl()
			*/
		typedef enum VgtPartialVsWaveMode
		{
			kVgtPartialVsWaveDisable                    = 0, ///< The VGT will continue a VS wave from one primitive group to next primitive group within a draw call.
			kVgtPartialVsWaveEnable                     = 1, ///< The VGT will emit a VS wave as soon as a primitive group is finished, even if the wave is not full.
		} VgtPartialVsWaveMode;

		/** @brief Determines when the input assembly (IA) unit should switch between the two vertex geometry tessellator (VGT) units.
			@see DrawCommandBuffer::setVgtControl()
			*/
		typedef enum WdSwitchOnlyOnEopMode
		{
			kWdSwitchOnlyOnEopDisable             = 0, ///< Disables switching between the VGT units at EOP time. 
			kWdSwitchOnlyOnEopEnable              = 1, ///< Enables switching between the VGT units at EOP time.
		} WdSwitchOnlyOnEopMode;

		/** @brief Controls how vertex coordinates are quantized to sub-pixel sample locations.
			@see DrawCommandBuffer::setVertexQuantization()
			*/
		typedef enum VertexQuantizationRoundMode
		{
			kVertexQuantizationRoundModeTruncate                    = 0, ///< Truncate vertex coordinates (effectively, round towards zero). OpenGL-style.
			kVertexQuantizationRoundModeRound                       = 1, ///< Round to the nearest sample location.
			kVertexQuantizationRoundModeRoundToEven                 = 2, ///< Round to the nearest sample location, breaking ties by choosing the sample location whose least significant bit is 0. Direct3D-style.
			kVertexQuantizationRoundModeRoundToOdd                  = 3, ///< Round to the nearest sample location, breaking ties by choosing the sample location whose least significant bit is 1.
		} VertexQuantizationRoundMode;

		/** @brief Controls the precision of sub-pixel vertex coordinates. 24 bits are available.
			@see DrawCommandBuffer::setVertexQuantization()
			*/
		typedef enum VertexQuantizationMode
		{
			kVertexQuantizationMode16_8                = 5, ///< 16.8 fixed-point format.
			kVertexQuantizationMode14_10               = 6, ///< 14.10 fixed-point format.
			kVertexQuantizationMode12_12               = 7, ///< 12.12 fixed-point format.
		} VertexQuantizationMode;

		/** @brief Defines the possible locations of the pixel center with respect to vertex quantitization.
		
		           Internally, the drawing engine has pixel centers at (0.5, 0.5), which affects how
				   quantization is implemented in the case of each enumerator (please see individual descriptions).
			
			@see DrawCommandBuffer::setVertexQuantization()
			
			*/
		typedef enum VertexQuantizationCenterMode
		{
			kVertexQuantizationCenterAtZero             = 0, ///< Quantize vertices to the internal subpixel coordinates (0, 0) by adding 0.5 to the incoming vertex X and Y coordinates. This follows the DirectX 9 style.
			kVertexQuantizationCenterAtHalf             = 1, ///< Quantize vertices to the internal subpixel coordinates (0.5, 0.5) by leaving the input vertex coordinates unchanged. This follows the OpenGL and DirectX 10+ style.
		} VertexQuantizationCenterMode;

		/** @brief Controls whether writes to the depth buffer are enabled.
			@see DepthStencilControl::setDepthControl()
			*/
		typedef enum DepthControlZWrite
		{
			kDepthControlZWriteDisable                  = 0, ///< Disables depth writes.
			kDepthControlZWriteEnable                   = 1, ///< Enables depth writes.
		} DepthControlZWrite;

		/** @brief Controls whether depth buffer is permitted to write back compressed tiles to a buffer.
			@see DbRenderControl::setDepthTileWriteBackPolicy(), DbRenderControl::setStencilTileWriteBackPolicy()
		    */
		typedef enum DbTileWriteBackPolicy
		{
			kDbTileWriteBackPolicyCompressionAllowed = 0, ///< DB is permitted to write back compressed tiles to a buffer.
			kDbTileWriteBackPolicyCompressionForbidden = 1, ///< DB is forbidden from writing back compressed tiles to a buffer.
		} DbTileWriteBackPolicy;

		/** @brief Controls the accuracy/performance tradeoff for Z-pass counts.
			@see DrawCommandBuffer::setDbCountControl(), DrawCommandBuffer::writeOcclusionQuery()
			*/
		typedef enum DbCountControlPerfectZPassCounts
		{
			kDbCountControlPerfectZPassCountsDisable    = 0, ///< Disables perfect Z-pass counts.
			kDbCountControlPerfectZPassCountsEnable     = 1, ///< Enables perfect Z-pass counts. This setting is usually used when writing an occlusion query.
		} DbCountControlPerfectZPassCounts;

		/** @brief Marks the beginning and end of an occlusion query.
			@see DrawCommandBuffer::writeOcclusionQuery()
			*/
		typedef enum OcclusionQueryOp
		{
			kOcclusionQueryOpClearAndBegin              = 0,		///< Beginning of an occlusion query. The query results will be cleared with a GPU DMA when this operation is submitted. This can lead to a race condition on the CPU if the results are not already cleared; consider manually clearing the query results and using <c>kOcclusionQueryOpBeginWithoutClear</c> instead.
			kOcclusionQueryOpEnd                        = 1,		///< End of an occlusion query.
			kOcclusionQueryOpBeginWithoutClear          = 2,		///< Beginning of an occlusion query. The query results are assumed to be already cleared; no GPU DMA is performed.
		} OcclusionQueryOp;

		/** @brief Specifies what to do when a predicated draw call is processed before its associated occlusion query results are ready.
			@see DrawCommandBuffer::setZPassPredicationEnable()
			*/
		typedef enum PredicationZPassHint
		{
			kPredicationZPassHintWait                   = 0, ///< Predicated draw calls will stall until occlusion query results are ready.
			kPredicationZPassHintDraw                   = 1, ///< Predicated draw calls will proceed to as if they were unpredicated.
		} PredicationZPassHint;

		/** @brief Specifies how a predicated draw call interpets the results of an occlusion query.
			@see DrawCommandBuffer::setZPassPredicationEnable()
			*/
		typedef enum PredicationZPassAction
		{
			kPredicationZPassActionDrawIfNotVisible          = 0, ///< Predicated draw calls will draw if no fragments passed the depth test.
			kPredicationZPassActionDrawIfVisible             = 1, ///< Predicated draw calls will draw if 1 or more fragments passed the depth test.
		} PredicationZPassAction;

		/** @brief Comparison functions used in depth/stencil operations,
				   comparing a reference value against the destination depth/stencil value.
			@see DepthStencilControl::setDepthControl(), DepthStencilControl::setStencilFunction(), DepthStencilControl::setStencilFunctionBack()
			*/
		typedef enum CompareFunc
		{
			kCompareFuncNever                        = 0x00000000, ///< Never passes.
			kCompareFuncLess                         = 0x00000001, ///< Passes if the reference is less than the destination.
			kCompareFuncEqual                        = 0x00000002, ///< Passes if the reference is equal to the destination.
			kCompareFuncLessEqual                    = 0x00000003, ///< Passes if the reference is less than or equal to the destination.
			kCompareFuncGreater                      = 0x00000004, ///< Passes if the reference is greater than the destination.
			kCompareFuncNotEqual                     = 0x00000005, ///< Passes if the reference is not equal to the destination.
			kCompareFuncGreaterEqual                 = 0x00000006, ///< Passes if the reference is greater than or equal to the destination.
			kCompareFuncAlways                       = 0x00000007, ///< Always passes.
		} CompareFunc;

		/** @brief Generates a stencil buffer value as a function of the current stencil buffer value, <c><i>testVal</i></c> and <c><i>opVal</i></c>.
			@see DrawCommandBuffer::setStencil(), DrawCommandBuffer::setStencilSeparate(), StencilOpControl::setStencilOps(), StencilOpControl::setStencilOpsBack()
			*/
		typedef enum StencilOp
		{
			kStencilOpKeep                           = 0x00000000, ///< <c>dest = src</c>.
			kStencilOpZero                           = 0x00000001, ///< <c>dest = 0x00</c>.
			kStencilOpOnes                           = 0x00000002, ///< <c>dest = 0xFF</c>.
			kStencilOpReplaceTest                    = 0x00000003, ///< <c>dest = testVal</c>.
			kStencilOpReplaceOp                      = 0x00000004, ///< <c>dest = opVal</c>.
			kStencilOpAddClamp                       = 0x00000005, ///< <c>dest = src + opVal (clamp)</c>.
			kStencilOpSubClamp                       = 0x00000006, ///< <c>dest = src - opVal (clamp)</c>.
			kStencilOpInvert                         = 0x00000007, ///< <c>dest = ~src</c>.
			kStencilOpAddWrap                        = 0x00000008, ///< <c>dest = src + opVal (wrap)</c>.
			kStencilOpSubWrap                        = 0x00000009, ///< <c>dest = src - opVal (wrap)</c>.
			kStencilOpAnd                            = 0x0000000a, ///< <c>dest = src \& opVal</c>.
			kStencilOpOr                             = 0x0000000b, ///< <c>dest = src | opVal</c>.
			kStencilOpXor                            = 0x0000000c, ///< <c>dest = src ^ opVal</c>.
			kStencilOpNand                           = 0x0000000d, ///< <c>dest = ~(src \& opVal)</c>.
			kStencilOpNor                            = 0x0000000e, ///< <c>dest = ~(src | opVal)</c>.
			kStencilOpXnor                           = 0x0000000f, ///< <c>dest = ~(src ^ opVal)</c>.
		} StencilOp;

		/** @brief Specifies color buffer modes.
			@see DrawCommandBuffer::setCbControl()
			*/
		typedef enum CbMode
		{
			kCbModeDisable                           = 0x00000000, ///< Disables drawing to color buffer.
			kCbModeNormal                            = 0x00000001, ///< Normal rendering mode.
			kCbModeEliminateFastClear                = 0x00000002, ///< Fill fast cleared color surface locations with fast-clear color. This mode can cause undefined behavior if used incorrectly. Please see <c>Gnmx::eliminateFastClear()</c> for the correct usage.
			kCbModeResolve                           = 0x00000003, ///< Read from MRT0, average all samples, and write to MRT1, which must be a single-sample target. This mode can cause undefined behavior if used incorrectly. Please see <c>Gnmx::hardwareMsaaResolve()</c> for the correct usage.
			kCbModeFmaskDecompress                   = 0x00000005, ///< Decompress the FMASK buffer into a texture-readable format.  A <c>kCbModeEliminateFastClear</c> pass before this is unnecessary. This mode can cause undefined behavior if used incorrectly. Please see <c>Gnmx::decompressFmaskSurface()</c> for the correct usage.
			kCbModeDccDecompress                     = 0x00000006, ///< Decompress any DCC-compressed surfaces and perform an implicit <c>kCbModeEliminateFastClear</c> and <c>kCbModeFmaskDecompress</c>.
		} CbMode;

		/** @brief Defines the possible raster operations that can be used. 
		
		These bitwise operations are performed on the source color (shader output) and
		           destination color (color buffer value).
			@see DrawCommandBuffer::setCbControl()
			*/
		typedef enum RasterOp
		{
			kRasterOpBlackness                       = 0x00000000, ///< Output: <c>0            </c>.
			kRasterOpNor                             = 0x00000005, ///< Output: <c>~(src |  dst)</c>.
			kRasterOpAndInverted                     = 0x0000000A, ///< Output: <c> ~src &  dst </c>.
			kRasterOpCopyInverted                    = 0x0000000F, ///< Output: <c> ~src        </c>.
			kRasterOpAndReverse                      = 0x00000044, ///< Output: <c>  src & ~dst </c>.
			kRasterOpInvert                          = 0x00000055, ///< Output: <c>        ~dst </c>.
			kRasterOpXor                             = 0x0000005A, ///< Output: <c>  src ^  dst </c>.
			kRasterOpNand                            = 0x0000005F, ///< Output: <c>~(src &  dst)</c>.
			kRasterOpAnd                             = 0x00000088, ///< Output: <c>  src &  dst </c>.
			kRasterOpEquiv                           = 0x00000099, ///< Output: <c>~(src ^  dst)</c>.
			kRasterOpNoop                            = 0x000000AA, ///< Output: <c>         dst </c>.
			kRasterOpOrInverted                      = 0x000000AF, ///< Output: <c> ~src |  dst </c>.
			kRasterOpCopy                            = 0x000000CC, ///< Output: <c>  src        </c>. Essentially, this mode disables the RasterOp functionality. It is the only mode that supports alpha blending.
			kRasterOpOrReverse                       = 0x000000DD, ///< Output: <c>  src | ~dst </c>.
			kRasterOpOr                              = 0x000000EE, ///< Output: <c>  src |  dst </c>.
			kRasterOpSet                             = 0x000000FF, ///< Output: <c>1            </c>.
		} RasterOp;

		/** @brief Specifies how the input assembly unit (IA) groups vertices into hardware-rasterizable primitives.
			@see DrawCommandBuffer::setPrimitiveType()
			*/
		typedef enum PrimitiveType
		{
			kPrimitiveTypeNone                            = 0x00000000, ///< No particular primitive type.
			kPrimitiveTypePointList                       = 0x00000001, ///< For N>=0, vertex N renders a point.
			kPrimitiveTypeLineList                        = 0x00000002, ///< For N>=0, vertices [N*2+0, N*2+1] render a line.
			kPrimitiveTypeLineStrip                       = 0x00000003, ///< For N>=0, vertices [N, N+1] render a line.
			kPrimitiveTypeTriList                         = 0x00000004, ///< For N>=0, vertices [N*3+0, N*3+1, N*3+2] render a triangle.
			kPrimitiveTypeTriFan                          = 0x00000005, ///< For N>=0, vertices [0, (N+1)%M, (N+2)%M] render a triangle, where M is the vertex count.  To prevent a GPU hang in NEO mode, you must call <c>setVgtControlForNeo(255, Gnm::kWdSwitchOnlyOnEopEnable, Gnm::kVgtPartialVsWaveDisable)</c> before using this mode.
			kPrimitiveTypeTriStrip                        = 0x00000006, ///< For N>=0, vertices [N*2+0, N*2+1, N*2+2] and [N*2+2, N*2+1, N*2+3] render triangles.
			kPrimitiveTypePatch                           = 0x00000009, ///< Used for tessellation.
			kPrimitiveTypeLineListAdjacency               = 0x0000000a, ///< For N>=0, vertices [N*4..N*4+3] render a line from [1, 2]. Lines [0, 1] and [2, 3] are adjacent to the rendered line.
			kPrimitiveTypeLineStripAdjacency              = 0x0000000b, ///< For N>=0, vertices [N+1, N+2] render a line. Lines [N, N+1] and [N+2, N+3] are adjacent to the rendered line.
			kPrimitiveTypeTriListAdjacency                = 0x0000000c, ///< For N>=0, vertices [N*6..N*6+5] render a triangle from [0, 2, 4]. Triangles [0, 1, 2] [4, 2, 3] and [5, 0, 4] are adjacent to the rendered triangle.
			kPrimitiveTypeTriStripAdjacency               = 0x0000000d, ///< For N>=0, vertices [N*4..N*4+6] render a triangle from [0, 2, 4] and [4, 2, 6]. Odd vertices Nodd form adjacent triangles with indices min(Nodd+1,Nlast) and max(Nodd-3,Nfirst). To prevent a GPU hang in NEO mode, you must call <c>setVgtControlForNeo(255, Gnm::kWdSwitchOnlyOnEopEnable, Gnm::kVgtPartialVsWaveDisable)</c> before using this mode.
			kPrimitiveTypeRectList                        = 0x00000011, ///< For N>=0, vertices [N*3+0, N*3+1, N*3+2] render a screen-aligned rectangle. 0 is upper-left, 1 is upper-right, and 2 is the lower-left corner.
			kPrimitiveTypeLineLoop                        = 0x00000012, ///< Like <c>kPrimitiveTypeLineStrip</c>, but the first and last vertices also render a line.  To prevent a GPU hang in NEO mode, you must call <c>setVgtControlForNeo(255, Gnm::kWdSwitchOnlyOnEopEnable, Gnm::kVgtPartialVsWaveDisable)</c> before using this mode.
			kPrimitiveTypeQuadList                        = 0x00000013, ///< For N>=0, vertices [N*4+0, N*4+1, N*4+2] and [N*4+0, N*4+2, N*4+3] render triangles.
			kPrimitiveTypeQuadStrip                       = 0x00000014, ///< For N>=0, vertices [N*2+0, N*2+1, N*2+3] and [N*2+0, N*2+3, N*2+2] render triangles.
			kPrimitiveTypePolygon                         = 0x00000015, ///< For N>=0, vertices [0, N+1, N+2] render a triangle.  To prevent a GPU hang in NEO mode, you must call <c>setVgtControlForNeo(255, Gnm::kWdSwitchOnlyOnEopEnable, Gnm::kVgtPartialVsWaveDisable)</c> before using this mode.
		} PrimitiveType;

		/** @brief Specifies a scale factor for a given blend equation input.
			@see BlendControl::setColorEquation(), BlendControl::setAlphaEquation, BlendFunc
		*/
		typedef enum BlendMultiplier
		{
			kBlendMultiplierZero                             = 0x00000000, ///< Multiply the associated input by zero.
			kBlendMultiplierOne                              = 0x00000001, ///< Multiply the associated input by one.
			kBlendMultiplierSrcColor                         = 0x00000002, ///< Multiply the associated input by the fragment color.
			kBlendMultiplierOneMinusSrcColor                 = 0x00000003, ///< Multiply the associated input by one minus the fragment color.
			kBlendMultiplierSrcAlpha                         = 0x00000004, ///< Multiply the associated input by the fragment alpha.
			kBlendMultiplierOneMinusSrcAlpha                 = 0x00000005, ///< Multiply the associated input by one minus the fragment alpha.
			kBlendMultiplierDestAlpha                        = 0x00000006, ///< Multiply the associated input by the render target alpha.
			kBlendMultiplierOneMinusDestAlpha                = 0x00000007, ///< Multiply the associated input by one minus the render target alpha.
			kBlendMultiplierDestColor                        = 0x00000008, ///< Multiply the associated input by the render target color.
			kBlendMultiplierOneMinusDestColor                = 0x00000009, ///< Multiply the associated input by one minus the render target color.
			kBlendMultiplierSrcAlphaSaturate                 = 0x0000000a, ///< Multiply the associated input by the minimum of 1 or fragment alpha.
			kBlendMultiplierConstantColor                    = 0x0000000d, ///< Multiply the associated input by the constant color. @see DrawCommandBuffer::setBlendColor()
			kBlendMultiplierOneMinusConstantColor            = 0x0000000e, ///< Multiply the associated input by one minus the constant color. @see DrawCommandBuffer::setBlendColor()
			kBlendMultiplierSrc1Color                        = 0x0000000f, ///< Multiply the associated input by a secondary fragment color.
			kBlendMultiplierInverseSrc1Color                 = 0x00000010, ///< Multiply the associated input by one minus a secondary fragment color.
			kBlendMultiplierSrc1Alpha                        = 0x00000011, ///< Multiply the associated input by a secondary fragment alpha.
			kBlendMultiplierInverseSrc1Alpha                 = 0x00000012, ///< Multiply the associated input by one minus a secondary fragment alpha.
			kBlendMultiplierConstantAlpha                    = 0x00000013, ///< Multiply the associated input by the constant color alpha. @see DrawCommandBuffer::setBlendColor()
			kBlendMultiplierOneMinusConstantAlpha            = 0x00000014, ///< Multiply the associated input by one minus the constant color alpha. @see DrawCommandBuffer::setBlendColor()
		} BlendMultiplier;

		/** @brief Functions which specify how the source (fragment) and destination (render target) blend inputs are to be combined
		           after a BlendMultiplier is applied to each.
			@see BlendControl::setColorEquation(), BlendControl::setAlphaEquation(), BlendMultiplier
		*/
		typedef enum BlendFunc
		{
			kBlendFuncAdd                        = 0x00000000, ///< The source value is added to the destination value.
			kBlendFuncSubtract                   = 0x00000001, ///< The destination value is subtracted from the source value.
			kBlendFuncMin                        = 0x00000002, ///< The minimum of the source and destination values is selected.
			kBlendFuncMax                        = 0x00000003, ///< The maximum of the source and destination values is selected.
			kBlendFuncReverseSubtract            = 0x00000004, ///< The source value is subtracted from the destination value.
		} BlendFunc;


		/** @brief Specifies how coordinates outside the range of <c>[0..1]</c> are handled by a texture sampler.
			@see Sampler::setWrapMode(), Sampler::getWrapModeX(), Sampler::getWrapModeY(), Sampler::getWrapModeZ()
		*/
		typedef enum WrapMode
		{
			kWrapModeWrap                            = 0x00000000, ///< The integer portion of the input coordinate is discarded, and the fractional portion is used instead. <c>U=U-floorf(U);</c>
			kWrapModeMirror                          = 0x00000001, ///< The input coordinate is "reflected" across the texture boundary. This reflection may occur multiple times until the coordinate falls within the <c>[0..1]</c> range. <c>U=isOdd(floorf(U)) ? 1-fracf(U) : fracf(U)</c>
			kWrapModeClampLastTexel                  = 0x00000002, ///< The input coordinate is clamped to the range <c>[0..1]</c>. <c>U=max(0,min(1,U));</c>
			kWrapModeMirrorOnceLastTexel             = 0x00000003, ///< The input coordinate is reflected at most one time and then clamped to the range <c>[0..1]</c>. <c>U=abs(max(-1,min(1,U));</c>
			kWrapModeClampHalfBorder                 = 0x00000004, ///< Similar to kWrapModeClampLastTexel, but if clamping is necessary, the output color will be the border color specified by the Sampler. For this mode, coordinates that are not within half a pixel of the border are considered clamped.
			kWrapModeMirrorOnceHalfBorder            = 0x00000005, ///< Similar to kWrapModeMirrorOnceLastTexel, but if clamping is necessary, the output color will be the border color specified by the Sampler. For this mode, coordinates that are not within half a pixel of the border are considered clamped.
			kWrapModeClampBorder                     = 0x00000006, ///< Similar to kWrapModeClampLastTexel, but if clamping is necessary, the output color will be the border color specified by the Sampler. For this mode, coordinates that are outside the range <c>[0..1]</c> are considered clamped.
			kWrapModeMirrorOnceBorder                = 0x00000007, ///< Similar to kWrapModeMirrorOnceLastTexel, but if clamping is necessary, the output color will be the border color specified by the Sampler. For this mode, coordinates that are outside the range <c>[0..1]</c> are considered clamped.
		} WrapMode;

		/** @brief Specifies how texels are filtered by a texture sampler in the X and Y axes. 

		For filtering along the Z axis, see #ZFilterMode.

			@see Sampler::setXyFilterMode(), Sampler::getMagFilterMode(), Sampler::getMinFilterMode(), ZFilterMode, AnisotropyRatio
		*/
		typedef enum FilterMode
		{
			kFilterModePoint                         = 0x00000000, ///< Sample the one texel nearest to the sample point.
			kFilterModeBilinear                      = 0x00000001, ///< Sample the four texels nearest the sample point, and blend linearly.
			kFilterModeAnisoPoint                    = 0x00000002, ///< Sample many texels in an anisotropic coverage pattern. This results in a similar appearance to kFilterModePoint, but with fewer aliasing artifacts.
			kFilterModeAnisoBilinear                 = 0x00000003, ///< Sample many texels in an anisotropic coverage pattern, and blend linearly. This results in a similar appearance to kFilterModeBilinear, but with fewer aliasing artifacts. This yields the highest quality results.
		} FilterMode;

		/** @brief Specifies how volume textures are filtered by a texture sampler in the Z axis. 
		
		For filtering along the X and Y axes, see #FilterMode.
			@see Sampler::setZFilterMode(), Sampler::getZFilterMode(), FilterMode
		*/
		typedef enum ZFilterMode
		{
			kZFilterModeNone                         = 0x00000000, ///< Do not sample.
			kZFilterModePoint                        = 0x00000001, ///< Sample the one texel from the slice nearest to the sample point.
			kZFilterModeLinear                       = 0x00000002, ///< Sample the two texels from the two slices nearest to the sample point, and blend linearly. This yields the highest quality results.
		} ZFilterMode;

		/** @brief Specifies how texels are filtered by a texture sampler between adjacent mip levels.
			@see Sampler::setMipFilterMode(), Sampler::getMipFilterMode()
			*/
		typedef enum MipFilterMode
		{
			kMipFilterModeNone                         = 0x00000000, ///< Always sample the base mip map.
			kMipFilterModePoint                        = 0x00000001, ///< Sample the one mip map nearest to the sample point.
			kMipFilterModeLinear                       = 0x00000002, ///< Sample the two mip maps nearest the sample point, and blend linearly.
		} MipFilterMode;

		/** @brief Specifies how a Sampler reduces one or more raw texel values to produce the final output value. 
			If anything but kFilterReductionModeWeightedAverage is chosen, only the first valid channel is returned.
			@see Sample::setFilterReductionMode()
			*/
		typedef enum FilterReductionMode
		{
			kFilterReductionModeWeightedAverage             = 0x00000000, ///< Compute the weighted average of the texel values. This is the default behavior.
			kFilterReductionModeMin                         = 0x00000001, ///< Select the minimum texel value per channel. Texels with a weight of zero will not be included in this computation.
			kFilterReductionModeMax                         = 0x00000002, ///< Select the maximum texel value per channel. Texels with a weight of zero will not be included in this computation.
		} FilterReductionMode;

		/** @brief Specifies which Z plane to use as the base for depth comparisons.
			@see DepthRenderTarget::setZCompareBase(), DepthRenderTarget::getZCompareBase()
			*/
		typedef enum ZCompareBase
		{
			kZCompareBaseZMin                = 0x00000000, ///< Use the near Z plane as the base for depth comparisons.
			kZCompareBaseZMax                = 0x00000001, ///< Use the far Z plan as the base for depth comparisons.
		} ZCompareBase;

		/** @brief Specifies the degree of anisotropy supported when an anisotropic texture filtering mode is selected.

			Higher ratios provide increased sharpness in oblique textures, at a slight performance cost.

			@see Sampler::setAnisotropyRatio(), Sampler::getAnisotropyRatio(), FilterMode
			*/
		typedef enum AnisotropyRatio
		{
			kAnisotropyRatio1                        = 0x00000000, ///< Sample one texel to approximate an anisotropic coverage pattern.
			kAnisotropyRatio2                        = 0x00000001, ///< Sample two texels to approximate an anisotropic coverage pattern.
			kAnisotropyRatio4                        = 0x00000002, ///< Sample four texels to approximate an anisotropic coverage pattern.
			kAnisotropyRatio8                        = 0x00000003, ///< Sample eight texels to approximate an anisotropic coverage pattern.
			kAnisotropyRatio16                       = 0x00000004, ///< Sample sixteen texels to approximate an anisotropic coverage pattern.
		} AnisotropyRatio;

		/** @brief Specifies the nature of the depth test to apply to each fragment.
			@see Sampler::setDepthCompareFunction(), Sampler::getDepthCompareFunction()
		*/
		typedef enum DepthCompare
		{
			kDepthCompareNever                       = 0x00000000, ///< The depth test never succeeds.
			kDepthCompareLess                        = 0x00000001, ///< The depth test succeeds if and only if the fragment depth is less than the stored depth.
			kDepthCompareEqual                       = 0x00000002, ///< The depth test succeeds if and only if the fragment depth is equal to the stored depth.
			kDepthCompareLessEqual                   = 0x00000003, ///< The depth test succeeds if and only if the fragment depth is less than or equal to the stored depth.
			kDepthCompareGreater                     = 0x00000004, ///< The depth test succeeds if and only if the fragment depth is greater than the stored depth.
			kDepthCompareNotEqual                    = 0x00000005, ///< The depth test succeeds if and only if the fragment depth is not equal to the stored depth.
			kDepthCompareGreaterEqual                = 0x00000006, ///< The depth test succeeds if and only if the fragment depth greater than or equal to the stored depth.
			kDepthCompareAlways                      = 0x00000007, ///< The depth test always succeeds.
		} DepthCompare;

		/** @brief Specifies one of three built-in texture border colors or a reference to a texture border color table in memory.
			@see Sampler::setBorderColor(), Sampler::getBorderColor()
		*/
		typedef enum BorderColor
		{
			kBorderColorTransBlack                   = 0x00000000, ///< The border color is R=0, G=0, B=0, A=0.
			kBorderColorOpaqueBlack                  = 0x00000001, ///< The border color is R=0, G=0, B=0, A=1.
			kBorderColorOpaqueWhite                  = 0x00000002, ///< The border color is R=1, G=1, B=1, A=1.
			kBorderColorFromTable                    = 0x00000003, ///< The border color is read from a table in memory. Do not use with texture samplers used by compute shaders. This usage is unsupported.
		} BorderColor;

		typedef enum EventSampleType
		{
			kEventSampleStreamoutStats1              = 0x00000001,
			kEventSampleStreamoutStats2              = 0x00000002,
			kEventSampleStreamoutStats3              = 0x00000003,
			kEventZPassDone                          = 0x00000015,
			kEventSamplePipelineStats                = 0x0000001e,
			kEventSampleStreamoutStats               = 0x00000020,
			kEventPixelPipeStatControl               = 0x00000038,
			kEventPixelPipeStatDump                  = 0x00000039,
			kEventPixelPipeStatReset                 = 0x0000003A,
		} EventSampleType;

		/** @brief Specifies an event to trigger on the GPU.
			@see DrawCommandBuffer::triggerEvent(), DispatchCommandBuffer::triggerEvent()
			*/
		typedef enum EventType
		{
			kEventTypeCacheFlush                         = 0x00000006, ///< Causes the color buffers, depth buffers, and shader export to flush all prior rendering in any destination cache to memory.
			kEventTypeCsPartialFlush                     = 0x00000007, ///< Flushes compute shader work after the command processor in the vertex geometry tessellator, shader pipe interpolation, and sequencer such that all compute shader work launched prior to this event will complete execution in the shader core and free all shader resources.
			kEventTypeVsPartialFlush                     = 0x0000000f, ///< Flushes all work between the command processor and the export, geometry and vertex shaders, including the vertex geometry tessellator.
			kEventTypePsPartialFlush                     = 0x00000010, ///< Flushes all work between the command processor and the export, geometry, vertex and pixel shaders, including scan conversion, primitive assembly and vertex geometry tessellator.
			kEventTypeFlushHsOutput                      = 0x00000011, ///< Sent by the VGT after an HS threadgroup. Used to make sure all HS threadgroup data is processed before the corresponding DS threadgroup begins.
			kEventTypeFlushLsOutput                      = 0x00000012, ///< Sent by the VGT after an LS threadgroup. Used to make sure all LS threadgroup data is processed before the corresponding HS threadgroup begins.
			kEventTypeCacheFlushAndInvEvent              = 0x00000016, ///< Causes the color buffers, depth buffer and shader export unit to flush to memory and invalidate all prior rendering to any destination cache.
			kEventTypePerfCounterStart                   = 0x00000017, ///< Starts any enabled event-based Performance counters.
			kEventTypePerfCounterStop                    = 0x00000018, ///< Stops any enabled event-based Performance counters that are event-enabled.
			kEventTypePipelineStatsStart                 = 0x00000019, ///< Starts pipeline / stream-out stats.
			kEventTypePipelineStatsStop                  = 0x0000001a, ///< Stops pipeline / stream-out stats.
			kEventTypePerfCounterSample                  = 0x0000001b, ///< Sample the performance counters of all blocks
			kEventTypeFlushEsOutput                      = 0x0000001c, ///< Instructs the SX to flush all the ES output to memory.
			kEventTypeFlushGsOutput                      = 0x0000001d, ///< Instructs the SX to flush all GS output to memory.
			kEventTypeSoVgtstreamoutFlush                = 0x0000001f, ///< Causes the VGT to update the read-only offsets registers and send a <c>VGT_CP_strmout_flushed</c> to instruct the CP to read the offsets.
			kEventTypeResetVertexCount                   = 0x00000021, ///< Resets the auto index count for vertex count.  There are two counters - one for GS and one for non-GS - and they should be reset separately.
			kEventTypeBlockContextDone                   = 0x00000022, ///< Internal hardware-generated event.
			kEventTypeVgtFlush                           = 0x00000024, ///< Causes the VGT to be flushed.  Used when GS ring buffer sizes are changed.
			kEventTypeSqNonEvent                         = 0x00000026, ///< Internal hardware-generated event reserved for SQ.
			kEventTypeScSendDbViewportZ                  = 0x00000027, ///< Internal hardware-generated event.
			kEventTypeDbCacheFlushAndInvalidate          = 0x0000002a, ///< Inserted when the depth surface is paged out to memory.
			kEventTypeFlushAndInvalidateDbMeta           = 0x0000002c, ///< Causes the depth buffer to flush and invalidate only its metadata (HTILE) cache.
			kEventTypeFlushAndInvalidateCbMeta           = 0x0000002e, ///< Causes the color buffer to flush and invalidate only its metadata (CMASK/FMASK) cache.
			kEventTypeFlushAndInvalidateCbPixelData      = 0x00000031, ///< Flushes and invalidates the color buffer's pixel (render target) data.  Does not guarantee <c><i>rwBuffers</i></c> or <c><i>rwTextures</i></c> flush-and-invalidate.  Does not flush CMASK/FMASK cache. Typically inserted before resolving/expanding an MSAA buffer. No wait-idle is necessary between this flush and subsequent resolve/expand commands.
		} EventType;

		/** @brief Specifies which shader stage to wait on before triggering an EOS event.
			@see DrawCommandBuffer::readDataFromGds(), DrawCommandBuffer::writeAtEndOfShader(), Gnmx::GfxContext::writeAtEndOfShader()
		*/
		typedef enum EndOfShaderEventType
		{
			kEosCsDone                             = 0x0000002f, ///< Causes the SQ to generate a signal to indicate that all CS work prior to this point has completed.
			kEosPsDone                             = 0x00000030, ///< Causes the SQ to generate a signal to indicate that all PS work prior to this point has completed.
		} EndOfShaderEventType;

		/** @brief Specifies the condition to wait on before triggering an EOP event.
			@see DrawCommandBuffer::writeEventStats(), DrawCommandBuffer::writeAtEndOfPipe(), Gnmx::GfxContext::writeImmediateAtEndOfPipe(), Gnmx::GfxContext::writeTimestampAtEndOfPipe(), ExtendedCacheAction
			*/
		typedef enum EndOfPipeEventType
		{
			kEopFlushCbDbCaches                       = 0x00000004, ///< Initiates a CB/DB cache flush, and then triggers an EOP event when the CB/DBs have finished writing data and have completed flushing their caches.
			kEopFlushAndInvalidateCbDbCaches          = 0x00000014, ///< Same as <c>kEopFlushCbDbCaches</c>, but also tells the CB/DBs to invalidate their caches before triggering.
			kEopCbDbReadsDone                         = 0x00000028, ///< Triggers when the CB/DBs have finished reading data (for example when blending). It explicitly does NOT guarantee that the CB/DBs have finished <i>writing</i> data! It is therefore primarily useful for releasing or overwriting shader resources.

			kEopCsDone                                = 0x00000028, ///< To be used when waiting for the CS shader to be done.

		} EndOfPipeEventType;

		/** @brief Defines the possible memory release events that can occur.
		*/
		typedef enum ReleaseMemEventType
		{
			// Shader Done:
			kReleaseMemEventCsDone                       = 0x2F, ///< Causes the SQ to generate a signal to indicate that all CS work prior to this point has completed.

			// End of Pipe Event:
			kReleaseMemEventFlushCbDbCaches				 = 0x04, ///< Initiates a CB/DB cache flush, then triggers an EOP event when the CB/DBs have finished writing data and have completed flushing their caches.
			kReleaseMemEventFlushAndInvalidateCbDbCaches = 0x14, ///< Same as <c>kEopFlushCbDbCaches</c>, but also tells the CB/DBs to invalidate their caches before triggering.
			kReleaseMemEventCbDbReadsDone			     = 0x28, ///< Triggers when the CB/DBs have finished reading data (for example when blending). It explicitly does NOT guarantee that the CB/DBs have finished <i>writing</i> data! It is therefore primarily useful for performance tests rather than synchronization.
			kReleaseMemEventFlushAndInvalidateDbCache    = 0x2B, ///< Causes the depth buffer to flush and invalidate only its data cache.
			kReleaseMemEventFlushAndInvalidateCbCache    = 0x2D, ///< Causes the color buffer to flush and invalidate only its data cache.
		} ReleaseMemEventType;

		/** @brief Defines the possible destinations EventWrite commands.
			*/
		typedef enum EventWriteDest
		{
			kEventWriteDestMemory       = 0x00,	///< Uncached write which bypasses the GPU’s L2 cache.
			kEventWriteDestTcL2         = 0x01, ///< Write to GPU L2 cache.
			kEventWriteDestTcL2Volatile = 0x11, ///< Write to the GPU L2 volatile cache.
		} EventWriteDest;

		/** @brief Defines the possible sources for EventWrite commands.
		 * 
		 *  @note Using <c>kEventWriteSourceGlobalClockCounter</c>: Because the global clock frequency is 100 MHz, a wraparound 
		 *  occurs on PlayStation®4 Pro systems at <c>2<sup>40</sup> * 10 <sup>-8</sup></c> seconds, which is approximately three hours 
 		 *  after system startup. Your code that uses this counter on a PlayStation®4 Pro system must account for the possibility of a 
		 *  wraparound. This constraint applies even when the application operates in Base mode on a PlayStation®4 Pro system or development kit.
		 *  @note We recommend coding to the PlayStation®4 Pro specification to avoid compatibility issues. 
		 *  @note This issue affects the following functions:
		 *  <ul>
		 *  	<li> DispatchCommandBuffer::writeReleaseMemEvent() </li>
		 *  	<li> DispatchCommandBuffer::writeReleaseMemEventWithInterrupt() </li>
		 *  	<li> DrawCommandBuffer::writeAtEndOfPipe() </li>
		 *  	<li> DrawCommandBuffer::writeAtEndOfPipeWithInterrupt() </li>
		 *  	<li> UnsafeDispatchCommandBuffer::writeReleaseMemEvent() </li>
		 *  	<li> UnsafeDispatchCommandBuffer::writeReleaseMemEventWithInterrupt() </li>
		 *  	<li> UnsafeDrawCommandBuffer::writeAtEndOfPipe() </li>
		 *  	<li> UnsafeDrawCommandBuffer::writeAtEndOfPipeWithInterrupt() </li>
		 *  </ul>
		 *  @see DispatchCommandBuffer::writeReleaseMemEvent(), DispatchCommandBuffer::writeReleaseMemEventWithInterrupt(), 
		 *  	 DrawCommandBuffer::writeAtEndOfPipe(), DrawCommandBuffer::writeAtEndOfPipeWithInterrupt(),
		 *		 UnsafeDispatchCommandBuffer::writeReleaseMemEvent(), UnsafeDispatchCommandBuffer::writeReleaseMemEventWithInterrupt(),
		 *  	 UnsafeDrawCommandBuffer::writeAtEndOfPipe(), UnsafeDrawCommandBuffer::writeAtEndOfPipeWithInterrupt()
			*/
		typedef enum EventWriteSource
		{
			kEventWriteSource32BitsImmediate     = 0x1, ///< Source is a 32-bit constant value provided as a separate function argument.
			kEventWriteSource64BitsImmediate     = 0x2, ///< Source is a 64-bit constant value provided as a separate function argument.
			kEventWriteSourceGlobalClockCounter  = 0x3, ///< Source is a 64-bit timestamp from the system’s 100Mhz global clock on PlayStation®4 systems; a 40-bit timestamp on PlayStation®4 Pro systems. Beware of wraparound issues; see Notes.
			kEventWriteSourceGpuCoreClockCounter = 0x4, ///< Source is a 64-bit timestamp from the GPU’s Core clock.
		} EventWriteSource;


		/** @brief Passed to various GPU event-writing functions to specify an action to take on the data cache when the corresponding event is triggered.
			@see DrawCommandBuffer::writeEventStats(), DrawCommandBuffer::writeAtEndOfPipe(), Gnmx::GfxContext::writeImmediateAtEndOfPipe(), Gnmx::GfxContext::writeTimestampAtEndOfPipe(), ExtendedCacheAction
			*/
		typedef enum CacheAction
		{
			kCacheActionNone                                             = 0x00, ///< Take no cache action.
			kCacheActionWriteBackAndInvalidateL1andL2					 = 0x38, ///< Flush all data in the GPU L1 and L2 caches to memory, and invalidate both caches.
			// kCacheActionWriteBackL2Volatile SCE_GNM_API_DEPRECATED_ENUM_MSG("Due to a hardware issue, using kCacheActionWriteBackL2Volatile may cause memory corruption; please use kCacheActionWriteBackAndInvalidateL2Volatile instead.") = 0x3B, ///< <b>Deprecated.</b> Use <c>kCacheActionWriteBackAndInvalidateL2Volatile</c> instead.
			kCacheActionWriteBackAndInvalidateL2Volatile				 = 0x3B, ///< Flush and invalidate only the GPU L2 volatile data.
			kCacheActionInvalidateL2Volatile							 = 0x33, ///< Invalidate only the GPU L2 volatile data.
			kCacheActionInvalidateL1									 = 0x10, ///< Invalidate the contents of the GPU L1 cache.
		} CacheAction;

		/** @brief Specifies additional caches which may be flushed and invalidated by certain commands.

		    These values may be combined into a bitmask, subject to the restrictions described for each enumerator.
			@see CacheAction, DrawCommandBuffer::waitForGraphicsWrites(), DrawCommandBuffer::flushShaderCachesAndWait(), DispatchCommandBuffer::waitForGraphicsWrites(), DispatchCommandBuffer::flushShaderCachesAndWait()
			*/
		typedef enum ExtendedCacheAction
		{
			kExtendedCacheActionFlushAndInvalidateCbCache                   = 0x02000000, ///< Flush and invalidate all data in the CB cache. Does not flush the CMASK or FMASK caches; use <c><i>triggerEvent</i>(kEventTypeFlushAndInvalidateCbMeta)</c> instead.
			kExtendedCacheActionFlushAndInvalidateDbCache                   = 0x04000000, ///< Flush and invalidate all data in the DB cache. Does not flush the HTILE cache; use <c><i>triggerEvent</i>(kEventTypeFlushAndInvalidateDbMeta)</c> instead.
			kExtendedCacheActionInvalidateKCache                            = 0x08000000, ///< Invalidate all data in the K-cache.
			kExtendedCacheActionInvalidateICache                            = 0x20000000, ///< Invalidate all data in the Instruction cache.
		} ExtendedCacheAction;

		/** @brief Cache Policy Settings for fences and timestamps when modifying the GPU’s L2 cache.
			@see DrawCommandBuffer::writeAtEndOfPipe(), DrawCommandBuffer::writeAtEndOfPipeWithInterrupt(), DispatchCommandBuffer::writeReleaseMemEvent()
			*/
		typedef enum CachePolicy
		{
			kCachePolicyLru    = 0x0,	///< Write Through L2$.
			kCachePolicyStream = 0x1,	///< Write Through L2$, but don't touch LRU timestamps.
			kCachePolicyBypass = 0x2,	///< Completely bypass the L2$ - with very low bandwidth.
		} CachePolicy;

		/** @brief Defines the possible cache policies for the GPU’s L1 cache.
			*/
		typedef enum L1CachePolicy
		{
			kL1CachePolicyLru    = 0x0,	///< Cached in L1$.
			kL1CachePolicyBypass = 0x1,	///< Bypass the L1$.
		} L1CachePolicy;

		/** @brief Defines the possible cache policies set for the GPU’s K$ cache.
			*/
		typedef enum KCachePolicy
		{
			kKCachePolicyLru    = 0x0,	///< Cached in K$.
			kKCachePolicyBypass = 0x1,	///< Bypass the K$.
		} KCachePolicy;

		/** @brief Specifies the format of depth data in a DepthRenderTarget.
			@see DepthRenderTarget::setZFormat(), DepthRenderTarget::getZFormat()
			*/
		typedef enum ZFormat
		{
			kZFormatInvalid                          = 0x00000000, ///< Invalid format.
			kZFormat16                               = 0x00000001, ///< 16-bit unsigned integer.
			kZFormat32Float                          = 0x00000003, ///< 32-bit float.
		} ZFormat;

		/** @brief Specifies the format of stencil data in a DepthRenderTarget.
			@see DepthRenderTarget::setStencilFormat(), DepthRenderTarget::getStencilFormat()
			*/
		typedef enum StencilFormat
		{
			kStencilInvalid                          = 0x00000000, ///< Invalid format.
			kStencil8                                = 0x00000001, ///< 8-bit integer.
		} StencilFormat;

		/** @brief Specifies the target for the wave limit multiplier setting. 
		
		The target is
		           specified as a bit mask; to specify multiple targets, you can bitwise-OR multiple enum values.
			@see setWaveLimitMultipliers()
			*/
		typedef enum WaveLimitTarget
		{
			kWaveLimitTargetComputePipe0 = 0x0001U, ///< If this bit is set, the wave limit multiplier settings will be applied to compute pipe 0.
			kWaveLimitTargetComputePipe1 = 0x0002U,	///< If this bit is set, the wave limit multiplier settings will be applied to compute pipe 1. 
			kWaveLimitTargetComputePipe2 = 0x0004U,	///< If this bit is set, the wave limit multiplier settings will be applied to compute pipe 2. 
			kWaveLimitTargetComputePipe3 = 0x0008U,	///< If this bit is set, the wave limit multiplier settings will be applied to compute pipe 3. 
			kWaveLimitTargetComputePipe4 = 0x0010U,	///< If this bit is set, the wave limit multiplier settings will be applied to compute pipe 4. 
			kWaveLimitTargetComputePipe5 = 0x0020U,	///< If this bit is set, the wave limit multiplier settings will be applied to compute pipe 5. 
			kWaveLimitTargetComputePipe6 = 0x0040U,	///< If this bit is set, the wave limit multiplier settings will be applied to compute pipe 6. 
			kWaveLimitTargetGfx          = 0x0100U,	///< If this bit is set, the wave limit multiplier settings will be applied to the graphics pipe. 

			kWaveLimitTargetAll          = 0x017FU  ///< If this bit is set, the wave limit multiplier settings will be applied to all compute and graphics pipes.
		} WaveLimitTarget;


		/**
		 * @brief Specifies the expected output format of a PS shader, based on the contents of its #PsStageRegisters object.
		 *
		 *		These values are provided for debugging and validation and are not used directly by the %Gnm API.
		 *		This format is specified in the PSSL shader code using the following compiler directive:
		 *
		 *		@code
		 *			#pragma PSSL_target_output_format ([target <mrt#>|default] <format>)
		 *		@endcode
		 *
		 * @see Gnm::PsStageRegisters::getTargetOutputMode(), "Wave Shader Compiler User's Guide - Compiler Options - Render Output Formats"
		 */
		typedef enum PsTargetOutputMode
		{
			kPsTargetOutputModeNoExports         = 0, ///< Corresponds to target slots to which no data is exported.
			kPsTargetOutputModeR32               = 1, ///< Corresponds to <c>FMT_32_R</c>. Can be <c>FP32</c>, <c>UINT32</c>, or <c>SINT32</c>. 
			kPsTargetOutputModeG32R32            = 2, ///< Corresponds to <c>FMT_32_GR</c>. Can be <c>FP32</c>, <c>UINT32</c>, or <c>SINT32</c>. 
			kPsTargetOutputModeA32R32            = 3, ///< Corresponds to <c>FMT_32_AR</c>. Can be <c>FP32</c>, <c>UINT32</c>, or <c>SINT32</c>. 
			kPsTargetOutputModeA16B16G16R16Float = 4, ///< Corresponds to <c>FMT_FP16_ABGR</c>. Default format if not specified in the shader code. 
			kPsTargetOutputModeA16B16G16R16Unorm = 5, ///< Corresponds to <c>FMT_UNORM16_ABGR</c>. 
			kPsTargetOutputModeA16B16G16R16Snorm = 6, ///< Corresponds to <c>FMT_SNORM16_ABGR</c>. 
			kPsTargetOutputModeA16B16G16R16Uint  = 7, ///< Corresponds to <c>UINT16_ABGR</c>. 
			kPsTargetOutputModeA16B16G16R16Sint  = 8, ///< Corresponds to <c>SINT16_ABGR</c>. 
			kPsTargetOutputModeA32B32G32R32      = 9, ///< Corresponds to <c>FMT_32_ABGR</c>. Can be <c>FP32</c>, <c>UINT32</c>, or <c>SINT32</c>. 
		} PsTargetOutputMode;

		/** @brief Defines the possible conservative Z values, which are used to provide a hint to the hardware.
				
				This is used to hint to the hardware about the expected exported Z value. Accept/Reject by an early Z test can be
				performed effectively when using with Late Z and Re Z.
	
		*/
		typedef enum PsConservativeZExport
		{
				kPsConservativeZExportAnyZ				 = 0,	///< The exported Z can be any value.
				kPsConservativeZExportLessThanSourceZ	 = 1,	///< The exported Z can be assumed to be less than the source Z value.
				kPsConservativeZExportGreaterThanSourceZ = 2,	///< The exported Z can be assumed to be greater than the source Z value.

		} PsConservativeZExport;

		/** @brief Defines Z behavior.
		*/
		typedef enum PsZBehavior
		{
			kPsZBehaviorLateZ	= 0,	///< Late Z.
			kPsZBehaviorEarlyZ	= 1,	///< Early Z.
			kPsZBehaviorReZ     = 2,	///< Re Z.
		} PsZBehavior;



        /** @brief Defines VsLateAllocWaveLimit values in the Gnm library.
        @sa GraphicsShaderControl::setVsWaveLimit()
        */
        typedef enum VsLateAllocWaveLimit
        {
            kVsLateAllocWaveLimitMax	 = 63,	///< Maximum value to specify 64 in VsLateAllocWaveLimit
            kVsLateAllocWaveLimitDefault = 28,	///< Default value to specify 29 in VsLateAllocWaveLimit
            kVsLateAllocWaveLimitMin	 = 0,	///< Minimum value to specify  1 in VsLateAllocWaveLimit
        } VsLateAllocWaveLimitMinusOne;

		/** @brief Specifies the format of data in a Buffer object.

		    This enum determines the number of channels in each buffer element and the bit depth of each channel. For specifying the mapping of
			source data to X/Y/Z/W channels, use Gnm::BufferChannel. For specifying the numerical interpretation of each channel's raw data, use Gnm::BufferChannelType.
			@note Only a subset of SurfaceFormat formats can be used as <c>BufferFormat</c> formats; use DataFormat::supportsBuffer()
			      to see if a given format is supported.
			*/
		typedef enum BufferFormat
		{
			kBufferFormatInvalid                               = 0x00000000, ///< Invalid Buffer format.
			kBufferFormat8                                     = 0x00000001, ///< One 8-bit channel. X=0xFF
			kBufferFormat16                                    = 0x00000002, ///< One 16-bit channel. X=0xFFFF
			kBufferFormat8_8                                   = 0x00000003, ///< Two 8-bit channels. X=0x00FF, Y=0xFF00
			kBufferFormat32                                    = 0x00000004, ///< One 32-bit channel. X=0xFFFFFFFF
			kBufferFormat16_16                                 = 0x00000005, ///< Two 16-bit channels. X=0x0000FFFF, Y=0xFFFF0000
			kBufferFormat10_11_11                              = 0x00000006, ///< One 10-bit channel (Z) and two 11-bit channels (Y,X). X=0x000007FF, Y=0x003FF800, Z=0xFFC00000 Interpreted only as floating-point by texture unit, but also as integer by rasterizer.
			kBufferFormat11_11_10                              = 0x00000007, ///< Two 11-bit channels (Z,Y) and one 10-bit channel (X). X=0x000003FF, Y=0x001FFC00, Z=0xFFE00000 Interpreted only as floating-point by texture unit, but also as integer by rasterizer.
			kBufferFormat10_10_10_2                            = 0x00000008, ///< Three 10-bit channels (W,Z,Y) and one 2-bit channel (X). X=0x00000003, Y=0x00000FFC, Z=0x003FF000, W=0xFFC00000. X is never negative, even when YZW are.
			kBufferFormat2_10_10_10                            = 0x00000009, ///< One 2-bit channel (W) and three 10-bit channels (Z,Y,X). X=0x000003FF, Y=0x000FFC00, Z=0x3FF00000, W=0xC0000000. W is never negative, even when XYZ are.
			kBufferFormat8_8_8_8                               = 0x0000000a, ///< Four 8-bit channels. X=0x000000FF, Y=0x0000FF00, Z=0x00FF0000, W=0xFF000000
			kBufferFormat32_32                                 = 0x0000000b, ///< Two 32-bit channels.
			kBufferFormat16_16_16_16                           = 0x0000000c, ///< Four 16-bit channels.
			kBufferFormat32_32_32                              = 0x0000000d, ///< Three 32-bit channels.
			kBufferFormat32_32_32_32                           = 0x0000000e, ///< Four 32-bit channels.
		} BufferFormat;

		/** @brief Specifies the format of data in a RenderTarget object.

			 This enum determines the number of channels in each RenderTarget element and the bit depth of each channel. For specifying the mapping of
			 components to X/Y/Z/W channels, use Gnm::RenderTargetChannelOrder. For specifying the numerical interpretation of each channel's raw data, use Gnm::RenderTargetChannelType.
			@note Only a subset of SurfaceFormat formats can be used as <c>RenderTargetFormat</c> formats; use DataFormat::supportsRenderTarget() to see if a given format is supported.
			*/
		typedef enum RenderTargetFormat
		{
			kRenderTargetFormatInvalid                               = 0x00000000, ///< Invalid RenderTarget format.
			kRenderTargetFormat8                                     = 0x00000001, ///< One 8-bit channel. X=0xFF
			kRenderTargetFormat16                                    = 0x00000002, ///< One 16-bit channel. X=0xFFFF
			kRenderTargetFormat8_8                                   = 0x00000003, ///< Two 8-bit channels. X=0x00FF, Y=0xFF00
			kRenderTargetFormat32                                    = 0x00000004, ///< One 32-bit channel. X=0xFFFFFFFF
			kRenderTargetFormat16_16                                 = 0x00000005, ///< Two 16-bit channels. X=0x0000FFFF, Y=0xFFFF0000
			kRenderTargetFormat10_11_11                              = 0x00000006, ///< One 10-bit channel (Z) and two 11-bit channels (Y,X). X=0x000007FF, Y=0x003FF800, Z=0xFFC00000 Interpreted only as floating-point by texture unit, but also as integer by rasterizer.
			kRenderTargetFormat11_11_10                              = 0x00000007, ///< Two 11-bit channels (Z,Y) and one 10-bit channel (X). X=0x000003FF, Y=0x001FFC00, Z=0xFFE00000 Interpreted only as floating-point by texture unit, but also as integer by rasterizer.
			kRenderTargetFormat10_10_10_2                            = 0x00000008, ///< Three 10-bit channels (W,Z,Y) and one 2-bit channel (X). X=0x00000003, Y=0x00000FFC, Z=0x003FF000, W=0xFFC00000 X is never negative, even when YZW are.
			kRenderTargetFormat2_10_10_10                            = 0x00000009, ///< One 2-bit channel (W) and three 10-bit channels (Z,Y,X). X=0x000003FF, Y=0x000FFC00, Z=0x3FF00000, W=0xC0000000 W is never negative, even when XYZ are.
			kRenderTargetFormat8_8_8_8                               = 0x0000000a, ///< Four 8-bit channels. X=0x000000FF, Y=0x0000FF00, Z=0x00FF0000, W=0xFF000000
			kRenderTargetFormat32_32                                 = 0x0000000b, ///< Two 32-bit channels.
			kRenderTargetFormat16_16_16_16                           = 0x0000000c, ///< Four 16-bit channels.
			kRenderTargetFormat32_32_32_32                           = 0x0000000e, ///< Four 32-bit channels.
			kRenderTargetFormat5_6_5                                 = 0x00000010, ///< One 5-bit channel (Z), one 6-bit channel (Y), and a second 5-bit channel (X). X=0x001F, Y=0x07E0, Z=0xF800
			kRenderTargetFormat1_5_5_5                               = 0x00000011, ///< One 1-bit channel (W) and three 5-bit channels (Z,Y,X). X=0x001F, Y=0x03E0, Z=0x7C00, W=0x8000
			kRenderTargetFormat5_5_5_1                               = 0x00000012, ///< Three 5-bit channels (W,Z,Y) and one 1-bit channel (X). X=0x0001, Y=0x003E, Z=0x07C0, W=0xF800
			kRenderTargetFormat4_4_4_4                               = 0x00000013, ///< Four 4-bit channels. X=0x000F, Y=0x00F0, Z=0x0F00, W=0xF000
			kRenderTargetFormat8_24                                  = 0x00000014, ///< One 8-bit channel and one 24-bit channel. X=0x00FFFFFF, Y=0xFF000000
			kRenderTargetFormat24_8                                  = 0x00000015, ///< One 24-bit channel and one 8-bit channel. X=0x000000FF, Y=0xFFFFFF00
			kRenderTargetFormatX24_8_32                              = 0x00000016, ///< One 24-bit channel, one 8-bit channel, and one 32-bit channel.
		} RenderTargetFormat;

		/** @brief Specifies the format of data in a Texture object.
			*/
		typedef enum SurfaceFormat
		{
			kSurfaceFormatInvalid                               = 0x00000000, ///< Invalid surface format.
			kSurfaceFormat8                                     = 0x00000001, ///< One 8-bit channel. X=0xFF
			kSurfaceFormat16                                    = 0x00000002, ///< One 16-bit channel. X=0xFFFF
			kSurfaceFormat8_8                                   = 0x00000003, ///< Two 8-bit channels. X=0x00FF, Y=0xFF00
			kSurfaceFormat32                                    = 0x00000004, ///< One 32-bit channel. X=0xFFFFFFFF
			kSurfaceFormat16_16                                 = 0x00000005, ///< Two 16-bit channels. X=0x0000FFFF, Y=0xFFFF0000
			kSurfaceFormat10_11_11                              = 0x00000006, ///< One 10-bit channel (Z) and two 11-bit channels (Y,X). X=0x000007FF, Y=0x003FF800, Z=0xFFC00000 Interpreted only as floating-point by texture unit, but also as integer by rasterizer.
			kSurfaceFormat11_11_10                              = 0x00000007, ///< Two 11-bit channels (Z,Y) and one 10-bit channel (X). X=0x000003FF, Y=0x001FFC00, Z=0xFFE00000 Interpreted only as floating-point by texture unit, but also as integer by rasterizer.
			kSurfaceFormat10_10_10_2                            = 0x00000008, ///< Three 10-bit channels (W,Z,Y) and one 2-bit channel (X). X=0x00000003, Y=0x00000FFC, Z=0x003FF000, W=0xFFC00000 X is never negative, even when YZW are.
			kSurfaceFormat2_10_10_10                            = 0x00000009, ///< One 2-bit channel (W) and three 10-bit channels (Z,Y,X). X=0x000003FF, Y=0x000FFC00, Z=0x3FF00000, W=0xC0000000 W is never negative, even when XYZ are.
			kSurfaceFormat8_8_8_8                               = 0x0000000a, ///< Four 8-bit channels. X=0x000000FF, Y=0x0000FF00, Z=0x00FF0000, W=0xFF000000
			kSurfaceFormat32_32                                 = 0x0000000b, ///< Two 32-bit channels.
			kSurfaceFormat16_16_16_16                           = 0x0000000c, ///< Four 16-bit channels.
			kSurfaceFormat32_32_32                              = 0x0000000d, ///< Three 32-bit channels.
			kSurfaceFormat32_32_32_32                           = 0x0000000e, ///< Four 32-bit channels.
			kSurfaceFormat5_6_5                                 = 0x00000010, ///< One 5-bit channel (Z), one 6-bit channel (Y), and a second 5-bit channel (X). X=0x001F, Y=0x07E0, Z=0xF800
			kSurfaceFormat1_5_5_5                               = 0x00000011, ///< One 1-bit channel (W) and three 5-bit channels (Z,Y,X). X=0x001F, Y=0x03E0, Z=0x7C00, W=0x8000
			kSurfaceFormat5_5_5_1                               = 0x00000012, ///< Three 5-bit channels (W,Z,Y) and one 1-bit channel (X). X=0x0001, Y=0x003E, Z=0x07C0, W=0xF800
			kSurfaceFormat4_4_4_4                               = 0x00000013, ///< Four 4-bit channels. X=0x000F, Y=0x00F0, Z=0x0F00, W=0xF000
			kSurfaceFormat8_24                                  = 0x00000014, ///< One 8-bit channel and one 24-bit channel.
			kSurfaceFormat24_8                                  = 0x00000015, ///< One 24-bit channel and one 8-bit channel.
			kSurfaceFormatX24_8_32                              = 0x00000016, ///< One 24-bit channel, one 8-bit channel, and one 32-bit channel.
			kSurfaceFormatGB_GR                                 = 0x00000020, ///< To be documented.
			kSurfaceFormatBG_RG                                 = 0x00000021, ///< To be documented.
			kSurfaceFormat5_9_9_9                               = 0x00000022, ///< One 5-bit channel (W) and three 9-bit channels (Z,Y,X). X=0x000001FF, Y=0x0003FE00, Z=0x07FC0000, W=0xF8000000. Interpreted only as three 9-bit denormalized mantissas, and one shared 5-bit exponent.
			kSurfaceFormatBc1                                   = 0x00000023, ///< BC1 block-compressed surface.
			kSurfaceFormatBc2                                   = 0x00000024, ///< BC2 block-compressed surface.
			kSurfaceFormatBc3                                   = 0x00000025, ///< BC3 block-compressed surface.
			kSurfaceFormatBc4                                   = 0x00000026, ///< BC4 block-compressed surface.
			kSurfaceFormatBc5                                   = 0x00000027, ///< BC5 block-compressed surface.
			kSurfaceFormatBc6                                   = 0x00000028, ///< BC6 block-compressed surface.
			kSurfaceFormatBc7                                   = 0x00000029, ///< BC7 block-compressed surface.
			kSurfaceFormatFmask8_S2_F1                          = 0x0000002C, ///< 8 bits-per-element FMASK surface (2 samples, 1 fragment).
			kSurfaceFormatFmask8_S4_F1                          = 0x0000002D, ///< 8 bits-per-element FMASK surface (4 samples, 1 fragment).
			kSurfaceFormatFmask8_S8_F1                          = 0x0000002E, ///< 8 bits-per-element FMASK surface (8 samples, 1 fragment).
			kSurfaceFormatFmask8_S2_F2                          = 0x0000002F, ///< 8 bits-per-element FMASK surface (2 samples, 2 fragments).
			kSurfaceFormatFmask8_S4_F2                          = 0x00000030, ///< 8 bits-per-element FMASK surface (8 samples, 2 fragments).
			kSurfaceFormatFmask8_S4_F4                          = 0x00000031, ///< 8 bits-per-element FMASK surface (4 samples, 4 fragments).
			kSurfaceFormatFmask16_S16_F1                        = 0x00000032, ///< 16 bits-per-element FMASK surface (16 samples, 1 fragment).
			kSurfaceFormatFmask16_S8_F2                         = 0x00000033, ///< 16 bits-per-element FMASK surface (8 samples, 2 fragments).
			kSurfaceFormatFmask32_S16_F2                        = 0x00000034, ///< 32 bits-per-element FMASK surface (16 samples, 2 fragments).
			kSurfaceFormatFmask32_S8_F4                         = 0x00000035, ///< 32 bits-per-element FMASK surface (8 samples, 4 fragments).
			kSurfaceFormatFmask32_S8_F8                         = 0x00000036, ///< 32 bits-per-element FMASK surface (8 samples, 8 fragments).
			kSurfaceFormatFmask64_S16_F4                        = 0x00000037, ///< 64 bits-per-element FMASK surface (16 samples, 4 fragments).
			kSurfaceFormatFmask64_S16_F8                        = 0x00000038, ///< 64 bits-per-element FMASK surface (16 samples, 8 fragments).
			kSurfaceFormat4_4                                   = 0x00000039, ///< Two 4-bit channels (Y,X). X=0x0F, Y=0xF0
			kSurfaceFormat6_5_5                                 = 0x0000003A, ///< One 6-bit channel (Z) and two 5-bit channels (Y,X). X=0x001F, Y=0x03E0, Z=0xFC00
			kSurfaceFormat1										= 0x0000003B, ///< One 1-bit channel. 8 pixels per byte, with pixel index increasing from LSB to MSB.
			kSurfaceFormat1Reversed								= 0x0000003C, ///< One 1-bit channel. 8 pixels per byte, with pixel index increasing from MSB to LSB.
		} SurfaceFormat;

		/** @brief Specifies the pixel ordering within each 8x8-pixel micro-tile of a render surface. */
		typedef enum MicroTileMode
		{
			kMicroTileModeDisplay                    = 0x00000000, ///< Only for 64 bpp and below.
			kMicroTileModeThin                       = 0x00000001, ///< Non-displayable. Can be used for thin, thick, or X thick.
			kMicroTileModeDepth                      = 0x00000002, ///< Only mode supported by DB.
			kMicroTileModeRotated                    = 0x00000003, ///< Rotated. Not supported by Gnm.
			kMicroTileModeThick                      = 0x00000004, ///< Thick and X thick, non-AA only.
		} MicroTileMode;

		/** @brief Specifies on a broad scale how a given TileMode tiles a surface.
			@see TileMode
			*/
		typedef enum ArrayMode
		{
			kArrayModeLinearGeneral                      = 0x00000000, ///< Linear pixel storage; no alignment or padding restrictions. DEPRECATED - Do not use!
			kArrayModeLinearAligned                      = 0x00000001, ///< Linear pixel storage with some minor alignment requirements and internal padding.
			kArrayMode1dTiledThin                        = 0x00000002, ///< Micro-tile-only tiling for non-volume surfaces. Not valid for AA modes.
			kArrayMode1dTiledThick                       = 0x00000003, ///< Micro-tile-only tiling for volume surfaces (8x8x4 pixel micro-tiles). Not valid for AA modes.
			kArrayMode2dTiledThin                        = 0x00000004, ///< Macro-tile tiling for non-volume surfaces.
			kArrayModeTiledThinPrt                       = 0x00000005, ///< Macro-tile tiling for non-volume partially-resident texture (PRT) surfaces. Supports aliasing multiple virtual texture pages to the same physical page.
			kArrayMode2dTiledThinPrt                     = 0x00000006, ///< Macro-tile tiling for non-volume partially-resident texture (PRT) surfaces. Does not support aliasing multiple virtual texture pages to the same physical page.
			kArrayMode2dTiledThick                       = 0x00000007, ///< Macro-tile tiling for volume surfaces (8x8x4 pixel micro-tiles).
			kArrayMode2dTiledXThick                      = 0x00000008, ///< Macro-tile tiling for volume surfaces (8x8x8 pixel micro-tiles).
			kArrayModeTiledThickPrt                      = 0x00000009, ///< Micro-tile-only tiling for partially-resident texture (PRT) volume surfaces (8x8x4 pixel micro-tiles). Supports aliasing multiple virtual texture pages to the same physical page.
			kArrayMode2dTiledThickPrt                    = 0x0000000a, ///< Macro-tile tiling for partially-resident texture (PRT) volume surfaces (8x8x4 pixel micro-tiles). Does not support aliasing multiple virtual texture pages to the same physical page.
			kArrayMode3dTiledThinPrt                     = 0x0000000b, ///< Macro-tile tiling for partially-resident texture (PRT) non-volume surfaces. Z slices are rotated by pipe. Does not support aliasing multiple virtual texture pages to the same physical page.
			kArrayMode3dTiledThin                        = 0x0000000c, ///< Macro-tile tiling for non-volume surfaces. Z slices are rotated by pipe.
			kArrayMode3dTiledThick                       = 0x0000000d, ///< Macro-tile tiling for volume surfaces (8x8x4 pixel micro-tiles). Z slices are rotated by pipe.
			kArrayMode3dTiledXThick                      = 0x0000000e, ///< Macro-tile tiling for volume surfaces (8x8x8 pixel micro-tiles). Z slices are rotated by pipe.
			kArrayMode3dTiledThickPrt                    = 0x0000000f, ///< Macro-tile tiling for partially-resident texture (PRT) volume surfaces (8x8x4 pixel micro-tiles). Z slices are rotated by pipe. Does not support aliasing multiple virtual texture pages to the same physical page.
		} ArrayMode;

		// Internal GpuAddress enum
		typedef enum PipeConfig
		{
			kPipeConfigP8_32x32_8x16                 = 0x0000000a,
			kPipeConfigP8_32x32_16x16                = 0x0000000c,
			kPipeConfigP16                           = 0x00000012,
		} PipeConfig;

		// Internal GpuAddress enum
		typedef enum TileSplit
		{
			kTileSplit64B                 = 0x00000000,
			kTileSplit128B                = 0x00000001,
			kTileSplit256B                = 0x00000002,
			kTileSplit512B                = 0x00000003,
			kTileSplit1KB                 = 0x00000004,
			kTileSplit2KB                 = 0x00000005,
			kTileSplit4KB                 = 0x00000006,
		} TileSplit;

		// Internal GpuAddress enum
		typedef enum SampleSplit
		{
			kSampleSplit1                 = 0x00000000,
			kSampleSplit2                 = 0x00000001,
			kSampleSplit4                 = 0x00000002,
			kSampleSplit8                 = 0x00000003,
		} SampleSplit;

		// Internal GpuAddress enum
		typedef enum NumBanks
		{
			kNumBanks2                         = 0x00000000,
			kNumBanks4                         = 0x00000001,
			kNumBanks8                         = 0x00000002,
			kNumBanks16                        = 0x00000003,
		} NumBanks;

		// Internal GpuAddress enum
		typedef enum BankWidth
		{
			kBankWidth1                   = 0x00000000,
			kBankWidth2                   = 0x00000001,
			kBankWidth4                   = 0x00000002,
			kBankWidth8                   = 0x00000003,
		} BankWidth;

		// Internal GpuAddress enum
		typedef enum BankHeight
		{
			kBankHeight1                  = 0x00000000,
			kBankHeight2                  = 0x00000001,
			kBankHeight4                  = 0x00000002,
			kBankHeight8                  = 0x00000003,
		} BankHeight;

		// Internal GpuAddress enum
		typedef enum MacroTileAspect
		{
			kMacroTileAspect1                 = 0x00000000,
			kMacroTileAspect2                 = 0x00000001,
			kMacroTileAspect4                 = 0x00000002,
			kMacroTileAspect8                 = 0x00000003,
		} MacroTileAspect;

		/** @brief Specifies how the pixels of a surface are ordered in memory.

		  Refer to the "GpuAddress Library Overview" for a detailed discussion of tiling modes. Although the documentation for each
		  enum entry describes briefly when that tile mode would be useful, we recommend strongly that you call GpuAddress::computeSurfaceTileMode()
		  to select the appropriate tile mode for each surface.
		*/
		typedef enum TileMode
		{
			// Depth modes (for depth buffers)
			kTileModeDepth_2dThin_64                   = 0x00000000, ///< Recommended for depth targets with one fragment per pixel.
			kTileModeDepth_2dThin_128                  = 0x00000001, ///< Recommended for depth targets with two or four fragments per pixel, or texture-readable.
			kTileModeDepth_2dThin_256                  = 0x00000002, ///< Recommended for depth targets with eight fragments per pixel.
			kTileModeDepth_2dThin_512                  = 0x00000003, ///< Recommended for depth targets with 512-byte tiles.
			kTileModeDepth_2dThin_1K                   = 0x00000004, ///< Recommended for depth targets with 1024-byte tiled.
			kTileModeDepth_1dThin                      = 0x00000005, ///< Not used; included only for completeness.
			kTileModeDepth_2dThinPrt_256               = 0x00000006, ///< Recommended for partially-resident depth surfaces. Does not support aliasing multiple virtual texture pages to the same physical page.
			kTileModeDepth_2dThinPrt_1K                = 0x00000007, ///< Not used; included only for completeness.
			// Display modes
			kTileModeDisplay_LinearAligned             = 0x00000008, ///< Recommended for any surface to be easily accessed on the CPU.
			kTileModeDisplay_1dThin                    = 0x00000009, ///< Not used; included only for completeness.
			kTileModeDisplay_2dThin                    = 0x0000000A, ///< Recommended mode for displayable render targets.
			kTileModeDisplay_ThinPrt                   = 0x0000000B, ///< Supports aliasing multiple virtual texture pages to the same physical page.
			kTileModeDisplay_2dThinPrt                 = 0x0000000C, ///< Does not support aliasing multiple virtual texture pages to the same physical page.
			// Thin modes (for non-displayable 1D/2D/3D surfaces)
			kTileModeThin_1dThin                       = 0x0000000D, ///< Recommended for read-only non-volume textures.
			kTileModeThin_2dThin                       = 0x0000000E, ///< Recommended for non-displayable intermediate render targets and read/write non-volume textures.
			kTileModeThin_3dThin                       = 0x0000000F, ///< Not used; included only for completeness.
			kTileModeThin_ThinPrt                      = 0x00000010, ///< Recommended for partially-resident textures (PRTs). Supports aliasing multiple virtual texture pages to the same physical page.
			kTileModeThin_2dThinPrt                    = 0x00000011, ///< Does not support aliasing multiple virtual texture pages to the same physical page.
			kTileModeThin_3dThinPrt                    = 0x00000012, ///< Does not support aliasing multiple virtual texture pages to the same physical page.
			// Thick modes (for 3D textures)
			kTileModeThick_1dThick                     = 0x00000013, ///< Recommended for read-only volume textures.
			kTileModeThick_2dThick                     = 0x00000014, ///< Recommended for volume textures to which pixel shaders will write.
			kTileModeThick_3dThick                     = 0x00000015, ///< Not used; included only for completeness.
			kTileModeThick_ThickPrt                    = 0x00000016, ///< Supports aliasing multiple virtual texture pages to the same physical page.
			kTileModeThick_2dThickPrt                  = 0x00000017, ///< Does not support aliasing multiple virtual texture pages to the same physical page.
			kTileModeThick_3dThickPrt                  = 0x00000018, ///< Does not support aliasing multiple virtual texture pages to the same physical page.
			kTileModeThick_2dXThick                    = 0x00000019, ///< Recommended for volume textures to which pixel shaders will write.
			kTileModeThick_3dXThick                    = 0x0000001A, ///< Not used; included only for completeness.
			// Hugely inefficient linear display mode - do not use!
			kTileModeDisplay_LinearGeneral             = 0x0000001F, ///< Unsupported; do not use!
		} TileMode;

		/** @brief Specifies the pixel ordering within each macro-tile of a render surface.
		*/
		typedef enum MacroTileMode
		{
			kMacroTileMode_1x4_16                      = 0x00000000, ///< bankWidth=1 bankHeight=4 macroTileAspect=4 numBanks=16 altBankHeight=4 altNumBanks= 8 altMacroTileAspect=2
			kMacroTileMode_1x2_16                      = 0x00000001, ///< bankWidth=1 bankHeight=2 macroTileAspect=2 numBanks=16 altBankHeight=4 altNumBanks= 8 altMacroTileAspect=2
			kMacroTileMode_1x1_16                      = 0x00000002, ///< bankWidth=1 bankHeight=1 macroTileAspect=2 numBanks=16 altBankHeight=2 altNumBanks= 8 altMacroTileAspect=1
			kMacroTileMode_1x1_16_dup                  = 0x00000003, ///< bankWidth=1 bankHeight=1 macroTileAspect=2 numBanks=16 altBankHeight=2 altNumBanks= 8 altMacroTileAspect=1
			kMacroTileMode_1x1_8                       = 0x00000004, ///< bankWidth=1 bankHeight=1 macroTileAspect=1 numBanks= 8 altBankHeight=1 altNumBanks= 8 altMacroTileAspect=1
			kMacroTileMode_1x1_4                       = 0x00000005, ///< bankWidth=1 bankHeight=1 macroTileAspect=1 numBanks= 4 altBankHeight=1 altNumBanks= 8 altMacroTileAspect=1
			kMacroTileMode_1x1_2                       = 0x00000006, ///< bankWidth=1 bankHeight=1 macroTileAspect=1 numBanks= 2 altBankHeight=1 altNumBanks= 4 altMacroTileAspect=1
			kMacroTileMode_1x1_2_dup                   = 0x00000007, ///< bankWidth=1 bankHeight=1 macroTileAspect=1 numBanks= 2 altBankHeight=1 altNumBanks= 2 altMacroTileAspect=1
			kMacroTileMode_1x8_16                      = 0x00000008, ///< bankWidth=1 bankHeight=8 macroTileAspect=4 numBanks=16 altBankHeight=4 altNumBanks=16 altMacroTileAspect=2
			kMacroTileMode_1x4_16_dup                  = 0x00000009, ///< bankWidth=1 bankHeight=4 macroTileAspect=4 numBanks=16 altBankHeight=4 altNumBanks= 8 altMacroTileAspect=2
			kMacroTileMode_1x2_16_dup                  = 0x0000000A, ///< bankWidth=1 bankHeight=2 macroTileAspect=2 numBanks=16 altBankHeight=2 altNumBanks= 8 altMacroTileAspect=1
			kMacroTileMode_1x1_16_dup2                 = 0x0000000B, ///< bankWidth=1 bankHeight=1 macroTileAspect=2 numBanks=16 altBankHeight=1 altNumBanks= 8 altMacroTileAspect=1
			kMacroTileMode_1x1_8_dup                   = 0x0000000C, ///< bankWidth=1 bankHeight=1 macroTileAspect=1 numBanks= 8 altBankHeight=1 altNumBanks= 4 altMacroTileAspect=1
			kMacroTileMode_1x1_4_dup                   = 0x0000000D, ///< bankWidth=1 bankHeight=1 macroTileAspect=1 numBanks= 4 altBankHeight=1 altNumBanks= 4 altMacroTileAspect=1
			kMacroTileMode_1x1_2_dup2                  = 0x0000000E, ///< bankWidth=1 bankHeight=1 macroTileAspect=1 numBanks= 2 altBankHeight=1 altNumBanks= 2 altMacroTileAspect=1
			kMacroTileMode_1x1_2_dup3                  = 0x0000000F, ///< bankWidth=1 bankHeight=1 macroTileAspect=1 numBanks= 2 altBankHeight=1 altNumBanks= 2 altMacroTileAspect=1
		} MacroTileMode;

		/** @brief Specifies the tiling mode that the CMASK metadata buffer of a RenderTarget uses.
			*/
		typedef enum CmaskTileMode
		{
			kCmaskTileModeTiled                        = 0, ///< CMASK data is stored in optimal tiled order. This is the default mode. Optimized for CB rendering.
			kCmaskTileModeLinear                       = 1, ///< CMASK data is stored in a linear order. This is less efficient to access, but may result in a smaller CMASK buffer (for improved cache occupancy.)
			kCmaskTileModeTextureCompatible            = 2, ///< CMASK data is stored in a non-optimal tiled order. You must use this mode when sampling from associated color/FMASK surfaces as textures without decompressing them first. Supported in NEO mode only.
		} CmaskTileMode;

		/** @brief Specifies the tiling mode that the CMASK metadata buffer of a RenderTarget uses.
			*/
		typedef enum FmaskCompressionMode
		{
			kFmaskCompressionModeEnabled		 = 0,	///< Default setting, will required a decompression pass before sampling from it.
			kFmaskCompressionModeUncompressed	 = 1,	///< Works in both NEO and Base modes.  Allows the texture unit to sample the Fmask as texture without a decompression pass.
			kFmaskCompressionModeOneFragmentOnly = 2,	///< NEO mode only! Allows fast clears and single fragment tiles to skip writing any FMASK, but decompresses FMASK afterward so that texture may read it in the compressed FMask mode. (Requires CMask buffer as metadata.)
		} FmaskCompressionMode;

		/** @brief Specifies the size of compressed or uncompressed data blocks when configuring the DCC compression feature of a RenderTarget.
			*/
		typedef enum DccBlockSize
		{
			kDccBlockSize64                            = 0, ///<  64-byte blocks.
			kDccBlockSize128                           = 1, ///< 128-byte blocks.
			kDccBlockSize256                           = 2, ///< 256-byte blocks.
		} DccBlockSize;

		/** @brief Specifies the minimum size of compressed data blocks when configuring the DCC compression feature of a RenderTarget.
			*/
		typedef enum DccMinCompressedBlockSize
		{
			kDccMinCompressedBlockSize32               = 0, ///< 32-byte compressed blocks.
			kDccMinCompressedBlockSize64               = 1, ///< 64-byte compressed blocks.
		} DccMinCompressedBlockSize;

		/** @brief Specifies the transformation applied to color values in a RenderTarget before they are compressed using DCC.
		*/
		typedef enum DccColorTransform
		{
			kDccColorTransformAuto                     = 0, ///< Auto-select appropriate DCC color channel transform according to the format of the RenderTarget.
			kDccColorTransformNone                     = 1, ///< Disable the DCC color channel transform. Use this when the RenderTarget stores data unrelated to its actual format (such as storing a 128-bit integer in <c>kDataFormatR32G32B32A32Uint</c>.)
			kDccColorTransformAbgr                     = 2, ///< Transform color channels [A,G,B,R] to [A, B-G, R-G, G]. Use when the least significant three channels are correlated. This mode is not compatible with targets that can be sampled as Textures.
			kDccColorTransformBgra                     = 3, ///< Transform color channels [B,G,R,A] to [G, B-G, R-G, A]. Use when the most significant three channels are correlated. This mode is not compatible with targets that can be sampled as Textures.
		} DccColorTransform;

		//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


		/** @brief Specifies the swizzle pattern in a Buffer.

		    This enumeration specifies the source of a single data channel (when reading data from a Buffer),
		    and/or the destination channel (when writing to an <c>RW_Buffer</c>).
			@note This enum is interchangeable with TextureChannel.
			@note RenderTarget swizzle patterns are more restricted. To specify the
			      swizzle pattern of a RenderTarget, use the Gnm::RenderTargetChannelOrder enum.
		*/
		typedef enum BufferChannel
		{
			kBufferChannelConstant0                 = 0x00000000, ///< Destination channel will contain a constant 0.0.
			kBufferChannelConstant1                 = 0x00000001, ///< Destination channel will contain a constant 1.0.
			kBufferChannelX                         = 0x00000004, ///< Destination channel will contain the source's X channel.
			kBufferChannelY                         = 0x00000005, ///< Destination channel will contain the source's Y channel.
			kBufferChannelZ                         = 0x00000006, ///< Destination channel will contain the source's Z channel.
			kBufferChannelW                         = 0x00000007, ///< Destination channel will contain the source's W channel.
		} BufferChannel;

		/** @brief Specifies the swizzle pattern in a Texture.

		    This enumeration specifies the source of a single data channel (when sampling from a Texture),
		    and/or the destination channel (when writing to an <c>RW_Texture</c>).
		    @note This enum is interchangeable with Gnm::BufferChannel.
			@note Render target swizzle patterns are more restricted. To specify the
			      swizzle pattern of a render target, use the Gnm::RenderTargetChannelOrder enum.
		*/
		typedef enum TextureChannel
		{
			kTextureChannelConstant0                 = 0x00000000, ///< Destination channel will contain a constant 0.0.
			kTextureChannelConstant1                 = 0x00000001, ///< Destination channel will contain a constant 1.0.
			kTextureChannelX                         = 0x00000004, ///< Destination channel will contain the source's X channel.
			kTextureChannelY                         = 0x00000005, ///< Destination channel will contain the source's Y channel.
			kTextureChannelZ                         = 0x00000006, ///< Destination channel will contain the source's Z channel.
			kTextureChannelW                         = 0x00000007, ///< Destination channel will contain the source's W channel.
		} TextureChannel;

		/** @brief Defines the channel order of the data within a render target.

		 The ordering also depends on the number of color components per pixel.
		*/
		typedef enum RenderTargetChannelOrder
		{
			kRenderTargetChannelOrderStandard               = 0x00000000, ///< 1 channel: R; 2 channel: RG; 3 channel: RGB; 4 channels: RGBA.
			kRenderTargetChannelOrderAlt                    = 0x00000001, ///< 1 channel: G; 2 channel: RA; 3 channel: RGA; 4 channels: BGRA.
			kRenderTargetChannelOrderReversed               = 0x00000002, ///< 1 channel: B; 2 channel: GR; 3 channel: BGR; 4 channels: ABGR.
			kRenderTargetChannelOrderAltReversed            = 0x00000003, ///< 1 channel: A; 2 channel: AR; 3 channel: AGR; 4 channels: ARGB.
		} RenderTargetChannelOrder;

		/** @brief Channel format used by Buffers.

		There are three types of integer encoding: U, S, and UB. U means "unsigned" and indicates that the number
		is stored as an unsigned integer. S means "signed" and indicates that the number is stored as a signed
		integer. UB means "unsigned biased" and indicates that the integer is stored as unsigned, but the lowest
		encodable value of a signed integer with the same number of bits is added when decoding. That is to say,
		8-bit UBNorm encodings for float -1 and 1 are the bit patterns 00000000 (0) and 11111111 (255,) while 8-bit
		SNorm encodings for float -1 and 1 are the bit patterns 10000000 (-128) and 01111111 (127).

		There are three means for linear scale and offset of a decoded integer during conversion to and from float: "Norm",
		"Scaled", and "NormNoZero". "Norm" indicates that the integer range is normalized to the floating-point range
		0..1 or -1..1 inclusive. In the case of SNorm, the lowest two negative integer values both decode as float -1,
		so that the float value 0 is also represented. "Scaled" indicates that an integer is converted to a float with
		the same value as the integer. "NormNoZero" indicates that the integer range is normalized to -1..1, but with no
		duplication of the lowest two encodings as float -1, and no representation of float zero either.

		There are two means to indicate that there is no integer-float conversion: "Int" and "Float". Both indicate that
		the encoded bits are to be copied directly to or from a shader register, with extension to or from the register's 32 bits.

		Two-bit channels never decode to negative values, even when ten-bit channels in the same element do.

		@note This enum is interchangeable with #TextureChannelType.
 		*/
		typedef enum BufferChannelType
		{
			kBufferChannelTypeUNorm                 = 0x00000000, ///< Stored as <c>uint X\<N</c>, interpreted as <c>float X/(N-1)</c>.
			kBufferChannelTypeSNorm                 = 0x00000001, ///< Stored as <c>int -N/2\<=X\<N/2</c>, interpreted as <c>float MAX(-1,X/(N/2-1))</c>.
			kBufferChannelTypeUScaled               = 0x00000002, ///< Stored as <c>uint X\<N</c>, interpreted as <c>float X</c>.
			kBufferChannelTypeSScaled               = 0x00000003, ///< Stored as <c>int -N/2\<=X\<N/2</c>, interpreted as <c>float X</c>.
			kBufferChannelTypeUInt                  = 0x00000004, ///< Stored as <c>uint X\<N</c>, interpreted as <c>uint X</c>. Not filterable.
			kBufferChannelTypeSInt                  = 0x00000005, ///< Stored as <c>int -N/2\<=X\<N/2</c>, interpreted as <c>int X</c>. Not filterable.
			kBufferChannelTypeSNormNoZero           = 0x00000006, ///< Stored as <c>int -N/2<=X\<N/2</c>, interpreted as <c>float ((X+N/2)/(N-1))*2-1</c>.
			kBufferChannelTypeFloat                 = 0x00000007, ///< Stored as <c>float</c>, interpreted as <c>float</c>.
																	///<  – 32-bit: SE8M23, bias 127, range <c>(-2^129..2^129)</c>
																	///<  – 16-bit: SE5M10, bias 15, range <c>(-2^17..2^17)</c>
																	///<  – 11-bit: E5M6 bias 15, range <c>[0..2^17)</c>
																	///<  – 10-bit: E5M5 bias 15, range <c>[0..2^17)</c>
		} BufferChannelType;

		/** @brief Channel format used by Textures.
		*
		* There are three types of integer encoding: U, S, and UB. U means "unsigned" and indicates that the number
		* is stored as an unsigned integer. S means "signed" and indicates that the number is stored as a signed
		* integer. UB means "unsigned biased" and indicates that the integer is stored as unsigned, but the lowest
		* encodable value of a signed integer with the same number of bits is added when decoding. That is to say,
		* 8-bit UBNorm encodings for float -1 and 1 are the bit patterns 00000000 (0) and 11111111 (255,) while 8-bit
		* SNorm encodings for float -1 and 1 are the bit patterns 10000000 (-128) and 01111111 (127).
		*
		* There are three means for linear scale and offset of a decoded integer during conversion to and from float: "Norm",
		* "Scaled", and "NormNoZero". "Norm" indicates that the integer range is normalized to the floating-point range
		* 0..1 or -1..1 inclusive. In the case of SNorm, the lowest two negative integer values both decode as float -1,
		* so that the float value 0 is also represented. "Scaled" indicates that an integer is converted to a float with
		* the same value as the integer. "NormNoZero" indicates that the integer range is normalized to -1..1, but with no
		* duplication of the lowest two encodings as float -1, and no representation of float zero either.
		*
		* There are two means to indicate that there is no integer-float conversion: "Int" and "Float." Both indicate that
		* the encoded bits are to be copied directly to or from a shader register, with extension to or from the register's 32 bits.
		*
		* "Srgb" encoding indicates that an unsigned integer encodes the float range 0..1 with a function applied
		* in an attempt to approximate the gamma response of an sRGB image. 
		*
		* Two-bit channels never decode to negative values, even when ten-bit channels in the same element do.
		*
		* @note This enum is interchangeable with #BufferChannelType.
 		*/
		typedef enum TextureChannelType
		{
			kTextureChannelTypeUNorm                 = 0x00000000, ///< Stored as <c>uint X\<N</c>, interpreted as <c>float X/(N-1)</c>.
			kTextureChannelTypeSNorm                 = 0x00000001, ///< Stored as <c>int -N/2\<=X\<N/2</c>, interpreted as <c>float MAX(-1,X/(N/2-1))</c>.
			kTextureChannelTypeUScaled               = 0x00000002, ///< Stored as <c>uint X\<N</c>, interpreted as <c>float X</c>.
			kTextureChannelTypeSScaled               = 0x00000003, ///< Stored as <c>int -N/2\<=X\<N/2</c>, interpreted as <c>float X</c>.
			kTextureChannelTypeUInt                  = 0x00000004, ///< Stored as <c>uint X\<N</c>, interpreted as <c>uint X</c>. Not filterable.
			kTextureChannelTypeSInt                  = 0x00000005, ///< Stored as <c>int -N/2\<=X\<N/2</c>, interpreted as <c>int X</c>. Not filterable.
			kTextureChannelTypeSNormNoZero           = 0x00000006, ///< Stored as <c>int -N/2\<=X\<N/2</c>, interpreted as <c>float ((X+N/2)/(N-1))*2-1</c>.
			kTextureChannelTypeFloat                 = 0x00000007, ///< Stored as <c>float</c>, interpreted as <c>float</c>.
																	///<  – 32-bit: SE8M23, bias 127, range <c>(-2^129..2^129)</c>
																	///<  – 16-bit: SE5M10, bias 15, range <c>(-2^17..2^17)</c>
																	///<  – 11-bit: E5M6 bias 15, range <c>[0..2^17)</c>
																	///<  – 10-bit: E5M5 bias 15, range <c>[0..2^17)</c>
			kTextureChannelTypeSrgb                  = 0x00000009, ///< Stored as <c>uint X\<N</c>, interpreted as <c>float sRGB(X/(N-1))</c>. Srgb only applies to the XYZ channels of the texture; the W channel is always linear.
			kTextureChannelTypeUBNorm                = 0x0000000A, ///< Stored as <c>uint X\<N</c>, interpreted as <c>float MAX(-1,(X-N/2)/(N/2-1))</c>.
			kTextureChannelTypeUBNormNoZero          = 0x0000000B, ///< Stored as <c>uint X\<N</c>, interpreted as <c>float (X/(N-1))*2-1</c>.
			kTextureChannelTypeUBInt                 = 0x0000000C, ///< Stored as <c>uint X\<N</c>, interpreted as <c>int X-N/2</c>. Not blendable or filterable.
			kTextureChannelTypeUBScaled              = 0x0000000D, ///< Stored as <c>uint X\<N</c>, interpreted as <c>float X-N/2</c>.
		} TextureChannelType;

		/** @brief Number format used by render targets.
		*
		* There are two types of integer encoding: U and S. U means "unsigned" and indicates that the number
		* is stored as an unsigned integer. S means "signed" and indicates that the number is stored as a signed
		* integer.
		*
		* There are two means for linear scale and offset of a decoded integer during conversion to and from float: "Norm"
		* and "Scaled." "Norm" indicates that the integer range is normalized to the floating-point range
		* 0..1 or -1..1 inclusive. In the case of SNorm, the lowest two negative integer values both decode as float -1,
		* so that the float value 0 is also represented. "Scaled" indicates that an integer is converted to a float with
		* the same value as the integer.
		*
		* There are two means to indicate that there is no integer-float conversion: "Int" and "Float." Both indicate that
		* the encoded bits are to be copied directly to or from a shader register, with extension to or from the register's 32 bits.
		*
		* "Srgb" encoding indicates that an unsigned integer encodes the float range 0..1 with a function applied
		* in an attempt to approximate the gamma response of an sRGB image.
		*
		* Two-bit channels never decode to negative values, even when ten-bit channels in the same element do.
 		*/
		typedef enum RenderTargetChannelType
		{
			kRenderTargetChannelTypeUNorm            = 0x00000000, ///< Stored as <c>uint X\<N</c>, interpreted as <c>float X/(N-1)</c>.
			kRenderTargetChannelTypeSNorm            = 0x00000001, ///< Stored as <c>int -N/2\<=X\<N/2</c>, interpreted as <c>float MAX(-1,X/(N/2-1))</c>.
			kRenderTargetChannelTypeUInt             = 0x00000004, ///< Stored as <c>uint X\<N</c>, interpreted as <c>uint X</c>. Not blendable.
			kRenderTargetChannelTypeSInt             = 0x00000005, ///< Stored as <c>int -N/2\<=X\<N/2</c>, interpreted as <c>int X</c>. Not blendable.
			kRenderTargetChannelTypeSrgb             = 0x00000006, ///< Stored as <c>uint X\<N</c>, interpreted as <c>float sRGB(X/(N-1))</c>. Srgb only applies to the XYZ channels of the texture; the W channel is always linear.
			kRenderTargetChannelTypeFloat            = 0x00000007, ///< Stored as <c>float</c>, interpreted as <c>float</c>.
																	///<  – 32-bit: SE8M23, bias 127, range <c>(-2^129..2^129)</c>
																	///<  – 16-bit: SE5M10, bias 15, range <c>(-2^17..2^17)</c>
																	///<  – 11-bit: E5M6 bias 15, range <c>[0..2^17)</c>
																	///<  – 10-bit: E5M5 bias 15, range <c>[0..2^17)</c>
		} RenderTargetChannelType;

		/** @brief Specifies the dimensionality of a Texture.
			@see Texture::getTextureType()
			*/
		typedef enum TextureType
		{
			kTextureType1d                           = 0x00000008, ///< One-dimensional texture.
			kTextureType2d                           = 0x00000009, ///< Two-dimensional texture.
			kTextureType3d                           = 0x0000000A, ///< Three-dimensional volume texture.
			kTextureTypeCubemap                      = 0x0000000B, ///< Cubic environment map texture (six 2D textures arranged in a cube and indexed by a 3D direction vector). This TextureType is also used for cubemap arrays.
			kTextureType1dArray                      = 0x0000000C, ///< Array of 1D textures.
			kTextureType2dArray                      = 0x0000000D, ///< Array of 2D textures.
			kTextureType2dMsaa                       = 0x0000000E, ///< Two-dimensional texture with multiple samples per pixel. Usually an alias into an MSAA render target.
			kTextureType2dArrayMsaa                  = 0x0000000F, ///< Array of 2D MSAA textures.
		} TextureType;

		/** @brief Maps cubemap faces to array slice indices. 

				Useful as a convenience when indexing into the faces of a cubemap, which are represented internally as a 2D texture array.
			*/
		typedef enum CubemapFace
		{
			kCubemapFaceNone                         = 0x00000000, ///< N/A.
			kCubemapFacePositiveX                    = 0x00000000, ///< Positive X face.
			kCubemapFaceNegativeX                    = 0x00000001, ///< Negative X face.
			kCubemapFacePositiveY                    = 0x00000002, ///< Positive Y face.
			kCubemapFaceNegativeY                    = 0x00000003, ///< Negative Y face.
			kCubemapFacePositiveZ                    = 0x00000004, ///< Positive Z face.
			kCubemapFaceNegativeZ                    = 0x00000005, ///< Negative Z face.
		} CubemapFace;

		/** @brief Indicates whether a writeDataInline() command should block until the transfer completes.
			@see DrawCommandBuffer::writeDataInline(), DispatchCommandBuffer::writeDataInline()
			*/
		typedef enum WriteDataConfirmMode
		{
			kWriteDataConfirmDisable                    = 0, ///< Transfer is non-blocking. CP will continue execution immediately.
			kWriteDataConfirmEnable                     = 1, ///< Transfer is blocking. CP will stall until the transfer completes.
		} WriteDataConfirmMode;

		/** @brief Selects the source of a DMA operation.
			@see DrawCommandBuffer::dmaData(), DispatchCommandBuffer::dmaData()
			*/
		typedef enum DmaDataSrc
		{
			kDmaDataSrcMemory	           = 0x0, ///< Source is a GPU-visible memory address.
			kDmaDataSrcGds	               = 0x1, ///< Source is an offset into Global Data Store (GDS).
			kDmaDataSrcData	               = 0x2, ///< Source is a 32-bit data constant.
			kDmaDataSrcMemoryUsingL2       = 0x3, ///< Source is a GPU-visible memory address, but should be read directly from the L2 cache.
			kDmaDataSrcRegister		       = 0x4, ///< Source is a GPU register offset (auto-increment enabled for multi-register DMAs).
			kDmaDataSrcRegisterNoIncrement = 0xC, ///< Source is a GPU register offset (auto-increment disabled for multi-register DMAs).
		} DmaDataSrc;

		/** @brief Selects the destination of a DMA operation.
			@see DrawCommandBuffer::dmaData(), DispatchCommandBuffer::dmaData()
			*/
		typedef enum DmaDataDst
		{
			kDmaDataDstMemory	           = 0x0, ///< Destination is a GPU-visible memory address.
			kDmaDataDstGds	               = 0x1, ///< Destination is an offset into Global Data Store (GDS).
			kDmaDataDstRegister		       = 0x4, ///< Destination is a GPU register offset (auto-increment enabled for multi-register DMAs).
			kDmaDataDstRegisterNoIncrement = 0xC, ///< Destination is a GPU register offset (auto-increment disabled for multi-register DMAs).
		} DmaDataDst;

		/** @brief Specifies whether the command processor should block further processing until a DMA operation completes
			@see DrawCommandBuffer::dmaData(), DispatchCommandBuffer::dmaData()
			*/
		typedef enum DmaDataBlockingMode
		{
			kDmaDataBlockingDisable                  = 0, ///< Non-blocking DMA operation.
			kDmaDataBlockingEnable                   = 1, ///< Blocking DMA operation.
		} DmaDataBlockingMode;


		/** @brief Specifies the value to write into the buffer's offset.
			@see DrawCommandBuffer::writeStreamoutBufferUpdate()
			*/
		typedef enum StreamoutBufferUpdateWrite
		{
			kStreamoutBufferUpdateWriteImmediate			= 0,	///< Write immediate value from the packet.
			kStreamoutBufferUpdateWriteBufferFilledSize		= 1,	///< Write the current filled size.
			kStreamoutBufferUpdateWriteIndirect				= 2,	///< Write value referenced by the source address in the packet.
			kStreamoutBufferUpdateWriteNone					= 3		///< Do nothing
		} StreamoutBufferUpdateWrite;

		/** @brief Specifies whether to store the current filled size of a streamout buffer.
			@see DrawCommandBuffer::writeStreamoutBufferUpdate()
			*/
		typedef enum StreamoutBufferUpdateSaveFilledSize
		{
			kStreamoutBufferUpdateDontSaveFilledSize		= 0,	///< Do not write filled size.
			kStreamoutBufferUpdateSaveFilledSize			= 1		///< Write the filled size at the destination address in the packet.
		} StreamoutBufferUpdateSaveFilledSize;

		/** @brief Selects buffer for the streamout update.
			@see DrawCommandBuffer::writeStreamoutBufferUpdate()
			*/
		typedef enum StreamoutBufferId
		{
			kStreamoutBuffer0 = 0, ///< Update streamout buffer 0.
			kStreamoutBuffer1 = 1, ///< Update streamout buffer 1.
			kStreamoutBuffer2 = 2, ///< Update streamout buffer 2.
			kStreamoutBuffer3 = 3, ///< Update streamout buffer 3.
		} StreamoutBufferId;


		/** @brief Options for semaphore signaling behavior.
			@see DispatchCommandBuffer::signalSemaphore()
			*/
		typedef enum SemaphoreSignalBehavior
		{
			kSemaphoreSignalBehaviorIncrement	= 0, ///< Increments the value in the mailbox.
			kSemaphoreSignalBehaviorSet			= 1, ///< Writes 1 into the mailbox.
		} SemaphoreSignalBehavior;

		/** @brief Options for semaphore waiting behavior.
			@see DispatchCommandBuffer::waitSemaphore()
			*/
		typedef enum SemaphoreWaitBehavior
		{
			kSemaphoreWaitBehaviorDecrement		= 0, ///< Waits until the value in the mailbox is not 0 and then decrements it.
			kSemaphoreWaitBehaviorNone			= 1, ///< Waits until the value in the mailbox is not 0 and does not modify it.
		} SemaphoreWaitBehavior;


		/** @brief Options to indicate whether to wait for the mailbox update in a semaphore command.
			@see DispatchCommandBuffer::signalSemaphore()
			*/
		typedef enum SemaphoreUpdateConfirmMode
		{
			kSemaphoreUpdateConfirmDisabled                     = 0, ///< Don't wait for the mailbox update.
			kSemaphoreUpdateConfirmEnabled                      = 1, ///< Wait for the mailbox update.
		} SemaphoreUpdateConfirmMode;


		/** @brief Specifies queue quantum timer's unit scale.
			*/
		typedef enum QuantumScale
		{
			kQuantumScale5us		= 0, ///< 5 us unit (~5000 clocks)
			kQuantumScale1ms		= 1, ///< 1 ms unit (~1M clocks)
		} QuantumScale;

		/** @brief Total number of slots available for each resource, per shader stage.
			@see Gnmx::ConstantUpdateEngine
			*/
		typedef enum ShaderConstantSlotCount
		{
			kSlotCountResource		                 = 128,	///< Number of resources (Textures/Buffers) per shader stage. Each entry can be either a <c>T#</c> or a <c>V#</c>.
			kSlotCountRwResource                     = 16,  ///< Number of read/write resources (such as rwTexture or rwBuffers) per shader stage. Each entry can be either a <c>T#</c> or a <c>V#</c>.
			kSlotCountSampler		                 = 16,	///< Number of Samplers per shader stage. Each entry must be an <c>S#</c>.
			kSlotCountVertexBuffer	                 = 32,	///< Number of vertex buffers per shader stage. Each entry will be a <c>V#</c>.
			kSlotCountConstantBuffer                 = 20,	///< Number of constant buffers per shader stage. Each entry will be a <c>V#</c>.
			kSlotCountBoolConstant		             = 16,	///< Number of bool constants per shader stage. Each entry is 32 bools packed into a <c>DWORD</c>.
			kSlotCountFloatConstant	                 = 16,	///< Number of float constants per shader stage. Each entry is one 32-bit single-precision IEEE float.
			kSlotCountAppendConsumeCounterRange      = 1,   ///< Number of Append/Consume counter ranges per shader stage. Each entry is a 32-bit value where the high 16 bits give a byte offset into GDS, and the low 16 bits give the size of the range (in bytes).
			kSlotCountStreamoutBuffer                = 4,   ///< Number of streamout buffers per shader stage. Each entry will be a <c>V#</c>.
			kSlotCountExtendedUserData               = 1,   ///< Number of extended user data slots (internal use only; here for consistency).
			kSlotCountGdsMemoryRange                 = 1,   ///< Number of GDS memory range data slots. Each entry is a 32-bit value where the high 16 bits give a byte offset into GDS, and the low 16 bits give the size of the range (in bytes).
		} ShaderConstantSlotCount;

		/** @brief Size of each fundamental element type, in <c>DWORD</c>s.
			@see Gnmx::ConstantUpdateEngine
			*/
		typedef enum ShaderConstantDwordSize
		{
			kDwordSizeResource	                = 8,	///< <c>T#</c> or padded <c>V#</c>.
			kDwordSizeRwResource	            = 8,	///< <c>T#</c> or padded <c>V#</c>.
			kDwordSizeSampler	                = 4,	///< <c>S#</c>.
			kDwordSizeVertexBuffer              = 4,	///< <c>V#</c>.
			kDwordSizeConstantBuffer            = 4,	///< <c>V#</c>.
			kDwordSizeBoolConstant              = 1,	///< 32 packed bits.
			kDwordSizeFloatConstant             = 1,	///< IEEE single-precision float.
			kDwordSizeAppendConsumeCounterRange = 1,    ///< Two 16-bit integers packed together.
			kDwordSizeStreamoutBuffer           = 4,    ///< <c>V#</c>.
			kDwordSizeExtendedUserData          = 128,  ///< Space for extra shader user data.
			kDwordSizeGdsMemoryRange            = 1,    ///< Two 16-bit integers packed together.
		} ShaderConstantDwordSize;

		/** @brief Number of API slots per shader constant chunk.

		    Chunking is disabled, so all slots for each resource map to a single chunk.
		    @see Gnmx::ConstantUpdateEngine
			*/
		typedef enum ShaderConstantChunkSlotCount
		{
			kChunkSlotCountResource         = 128, ///< Slots per chunk of read-only texture/buffer resources.
			kChunkSlotCountRwResource       = 16,  ///< Slots per chunk of read/write texture/buffer resources.
			kChunkSlotCountSampler          = 16,  ///< Slots per chunk of samplers.
			kChunkSlotCountVertexBuffer     = 32,  ///< Slots per chunk of vertex buffers.
			kChunkSlotCountConstantBuffer   = 20,  ///< Slots per chunk of constant buffers.
			kChunkSlotCountStreamoutBuffer  = 4,   ///< Slots per chunk of streamout buffers.
			kChunkSlotCountExtendedUserData = 1,   ///< Slots per chunk of Extended User Data blocks.
		} ShaderConstantChunkSlotCount;


		/** @brief Defines the possible pipe priorities. 
		
				Used by the dispatcher to select a pipeline to execute a task from after the highest-priority queue has been selected.
				The pipe priority is primarily used to specify a compute queue's priority relative to tasks in the GFX pipeline.
			@see Gnm::mapComputeQueueWithPriority
			*/
		typedef enum PipePriority
		{
			kPipePriorityLow    = 0, ///< The default pipe priority. Tasks in compute queues with low priority will execute at a lower priority than GFX pipeline tasks.
			kPipePriorityMedium = 1, ///< Tasks in compute queues with medium priority will execute at a higher priority than GFX pipeline tasks.
		} PipePriority;

		/** @brief Bitwise masks used for classifying %Gnm command buffer submission return values.
			@see Gnm::submitCommandBuffers(), ValidationStatus, SubmissionStatus
			*/
		typedef enum StatusMask
		{
			kStatusMaskError				= 0x00001000,	///< Validation or submission error. Usage: <c>(state & kStatusErrorMask != 0)</c>
			kStatusMaskValidationIssue		= 0x00002000,	///< Validation warning or error detected. Usage: <c>(state & kStatusValidationIssueMask != 0)</c>

			kStatusMaskValidationError		= 0x80D13000,	///< Validation error. Usage: <c>(state & kStatusMaskValidationError == kStatusMaskValidationError)</c>
			kStatusMaskValidationWarning	= 0x80D12000,	///< Validation warning. Usage: <c>(state & kStatusMaskValidationWarning == kStatusMaskValidationWarning)</c>
			kStatusMaskSubmissionError		= 0x80D11000,	///< Submission error. Usage: <c>(state & kStatusMaskSubmissionError == kStatusMaskSubmissionError)</c>
		} StatusMask;

		/** @brief Error codes returned by the %Gnm compute queue functions.
			*/
		typedef enum ComputeQueueMappingStatus
		{
			kComputeQueueMappingStatusInvalidPipeId		  = SCE_GNM_ERROR_COMPUTEQUEUE_INVALID_PIPE_ID, ///< Pipe ID must between 0 or 3 (inclusive).
			kComputeQueueMappingStatusInvalidQueueId	  = SCE_GNM_ERROR_COMPUTEQUEUE_INVALID_QUEUE_ID, ///< Queue ID must between 0 or 7 (inclusive).
			kComputeQueueMappingStatusInvalidRingSize     = SCE_GNM_ERROR_COMPUTEQUEUE_INVALID_RING_SIZE, ///< Invalid Ring Size (must be multiple of DW and a power of 2).
			kComputeQueueMappingStatusInvalidRingBaseAddr = SCE_GNM_ERROR_COMPUTEQUEUE_INVALID_RING_BASE_ADDR, ///< Ring Base Address must be 256 bytes aligned.
			kComputeQueueMappingStatusInvalidReadPtrAddr  = SCE_GNM_ERROR_COMPUTEQUEUE_INVALID_READ_PTR_ADDR, ///< Read Ptr Address must be 4 bytes aligned.
			kComputeQueueMappingStatusInvalidPipePriority = SCE_GNM_ERROR_COMPUTEQUEUE_INVALID_PIPE_PRIORITY, ///< Pipe Priority must be either kPipePriorityLow or kPipePriorityMedium

			kComputeQueueMappingStatusInternalError		  = SCE_GNM_ERROR_COMPUTEQUEUE_INTERNAL, ///< Internal Error.
		} ComputeQueueMappingStatus;

		/** @brief Problem codes that the Gnm Validation Module returns.
		*/
		typedef enum ValidationStatus
		{
			kValidationStatusInvalidArgument	 = SCE_GNM_ERROR_VALIDATION_INVALID_ARGUMENT,		///< An invalid argument was passed to the Gnm Validation Module.
			kValidationStatusFailedInternalError = SCE_GNM_ERROR_VALIDATION_FAILED_INTERNAL_ERROR,	///< An internal error occurred in Gnm Validation Module.
			kValidationStatusNotEnabled			 = SCE_GNM_ERROR_VALIDATION_NOT_ENABLED,			///< Validation not enabled. Please enable "PA Debug" in target manager.
		} ValidationStatus;

		/** @brief Codes returned by the %Gnm command buffer submit functions.
			@see Gnm::submitCommandBuffers(), Gnm::submitAndFlipCommandBuffers()
			*/
		typedef enum SubmissionStatus
		{
			kSubmissionSuccess								= 0,												///< The submission was successful.	

			kSubmissionFailedInvalidArgument				= SCE_GNM_ERROR_SUBMISSION_FAILED_INVALID_ARGUMENT, ///< The submission failed because invalid arguments were passed to the submit function.
			kSubmissionFailedNotEnoughResources				= SCE_GNM_ERROR_SUBMISSION_NOT_ENOUGH_RESOURCES,	///< The submission failed because there were not enough resources to submit the command buffers; try to reduce the number of DCB/CCB being submitted at once.

			kSubmissionAndFlipFailedInvalidCommandBuffer	= SCE_GNM_ERROR_SUBMISSION_AND_FLIP_FAILED_INVALID_COMMAND_BUFFER, ///< The submission failed because the <c>DrawCommandBuffer::prepareFlip()</c> hasn't been called.
			kSubmissionAndFlipFailedInvalidQueueFull		= SCE_GNM_ERROR_SUBMISSION_AND_FLIP_FAILED_INVALID_QUEUE_FULL, ///< The submission failed because the flip queue is full.
			kSubmissionAndFlipFailedRequestFailed			= SCE_GNM_ERROR_SUBMISSION_AND_FLIP_FAILED_REQUEST_FAILED, ///< The submission failed because the flip request failed.

			kSubmissionFailedInternalError					= SCE_GNM_ERROR_SUBMISSION_FAILED_INTERNAL_ERROR, ///< Internal error occurred.
		} SubmissionStatus;

		/** @brief Codes returned by the %Gnm command capture functions.
			@see triggerCapture()
			*/
		typedef enum CaptureStatus
		{
			kCaptureSuccess									= 0, ///< The capture was successful.	

			kCaptureFileIOError								= SCE_GNM_ERROR_CAPTURE_FILE_IO, ///< The capture failed because the specified file could not be opened for writing.
			kCaptureRazorNotLoadedError						= SCE_GNM_ERROR_CAPTURE_RAZOR_NOT_LOADED, ///< The capture failed because Razor for PlayStation®4 (Razor) is not enabled.
			kCaptureNothingToCapture						= SCE_GNM_ERROR_CAPTURE_NOTHING_TO_CAPTURE, ///< The capture failed because the command buffer being captured is empty.

			kCaptureFailedInternalError						= SCE_GNM_ERROR_CAPTURE_FAILED_INTERNAL, ///< The capture failed because of an internal error.
		} CaptureStatus;

		/** @brief Error codes returned by the %Gnm resource-registration functions.
			*/
		typedef enum ResourceRegistrationStatus
		{
			kResourceRegistrationStatusSuccess						= 0,								///< The operation was successful.

			kResourceRegistrationStatusInvalidArgument				= SCE_GNM_ERROR_RESOURCE_REGISTRATION_INVALID_ARGUMENT,					///< Invalid argument.
			kResourceRegistrationStatusInvalidShader				= SCE_GNM_ERROR_RESOURCE_REGISTRATION_INVALID_SHADER,					///< The shader is invalid.
			kResourceRegistrationStatusStaleHandle					= SCE_GNM_ERROR_RESOURCE_REGISTRATION_STALE_HANDLE,						///< The handle has been used after freeing.
			kResourceRegistrationStatusFull							= SCE_GNM_ERROR_RESOURCE_REGISTRATION_FULL,								///< No space available.
			kResourceRegistrationStatusNotPermitted					= SCE_GNM_ERROR_RESOURCE_REGISTRATION_NOT_PERMITTED,					///< The operation is not permitted.
			kResourceRegistrationStatusOutputArgumentIsNull			= SCE_GNM_ERROR_RESOURCE_REGISTRATION_OUTPUT_ARGUMENT_IS_NULL,			///< The output argument is <c>NULL</c>.
			kResourceRegistrationStatusOwnerHandleInvalid			= SCE_GNM_ERROR_RESOURCE_REGISTRATION_OWNER_HANDLE_INVALID,				///< The owner handle is invalid.
			kResourceRegistrationStatusResourceHandleInvalid		= SCE_GNM_ERROR_RESOURCE_REGISTRATION_RESOURCE_HANDLE_INVALID,			///< The resource handle is invalid.
			kResourceRegistrationStatusResourceTypeInvalid			= SCE_GNM_ERROR_RESOURCE_REGISTRATION_RESOURCE_TYPE_INVALID,			///< The resource type is invalid.
			kResourceRegistrationStatusGdsResourceTypeInvalid		= SCE_GNM_ERROR_RESOURCE_REGISTRATION_GDS_RESOURCE_TYPE_INVALID,		///< The GDS resource type is invalid.
			kResourceRegistrationStatusResourceSizeInvalid			= SCE_GNM_ERROR_RESOURCE_REGISTRATION_RESOURCE_SIZE_INVALID,			///< The resource size is invalid.
			kResourceRegistrationStatusResourceAddressInvalid		= SCE_GNM_ERROR_RESOURCE_REGISTRATION_RESOURCE_ADDRESS_IS_NULL,			///< The resource address is <c>NULL</c>.
			kResourceRegistrationStatusGdsOffsetInvalid				= SCE_GNM_ERROR_RESOURCE_REGISTRATION_GDS_OFFSET_INVALID,				///< The GDS offset is invalid.
			kResourceRegistrationStatusResourceNameIsNull			= SCE_GNM_ERROR_RESOURCE_REGISTRATION_RESOURCE_NAME_IS_NULL,			///< The resource name is <c>NULL</c>.
			kResourceRegistrationStatusOwnerNameIsNull				= SCE_GNM_ERROR_RESOURCE_REGISTRATION_OWNER_NAME_IS_NULL,				///< The owner name is <c>NULL</c>.
			kResourceRegistrationStatusFindCallbackIsNull			= SCE_GNM_ERROR_RESOURCE_REGISTRATION_FIND_CALLBACK_IS_NULL,			///< The find callback is <c>NULL</c>.
			kResourceRegistrationStatusResourceIsNotShader			= SCE_GNM_ERROR_RESOURCE_REGISTRATION_RESOURCE_IS_NOT_SHADER,			///< The resource is not a shader.
			kResourceRegistrationStatusUserMemoryParamIsNull		= SCE_GNM_ERROR_RESOURCE_REGISTRATION_USER_MEMORY_PARAM_IS_NULL,		///< The user memory parameter is <c>NULL</c>.
			kResourceRegistrationStatusUserMemoryParamNotAligned	= SCE_GNM_ERROR_RESOURCE_REGISTRATION_USER_MEMORY_PARAM_NOT_ALIGNED,	///< The user memory parameter is not aligned.
			kResourceRegistrationStatusInvalidNameLengthParam		= SCE_GNM_ERROR_RESOURCE_REGISTRATION_INVALID_NAME_LENGTH_PARAM,		///< The name length parameter is invalid.
			kResourceRegistrationStatusInvalidSizeParam			= SCE_GNM_ERROR_RESOURCE_REGISTRATION_INVALID_SIZE_PARAM,			///< The size parameter is invalid.
			kResourceRegistrationStatusInvalidNumResourcesParam		= SCE_GNM_ERROR_RESOURCE_REGISTRATION_INVALID_NUM_RESOURCES_PARAM,		///< The number of resources param is invalid.

			kResourceRegistrationStatusInternalError			= SCE_GNM_ERROR_RESOURCE_REGISTRATION_INTERNAL,					///< Internal Error.
		} ResourceRegistrationStatus;

		/** @brief Defines the <c>SceKernelEqueue</c> event IDs.	*/
		typedef enum EqEventType
		{
			kEqEventCompute0RelMem   = 0x00,	///< ReleaseMem event from the compute pipe 0.
			kEqEventCompute1RelMem   = 0x01,	///< ReleaseMem event from the compute pipe 1.
			kEqEventCompute2RelMem   = 0x02,	///< ReleaseMem event from the compute pipe 2.
			kEqEventCompute3RelMem   = 0x03,	///< ReleaseMem event from the compute pipe 3.
			kEqEventCompute4RelMem   = 0x04,	///< ReleaseMem event from the compute pipe 4.
			kEqEventCompute5RelMem   = 0x05,	///< ReleaseMem event from the compute pipe 5.
			kEqEventCompute6RelMem   = 0x06,	///< ReleaseMem event from the compute pipe 6.
			kEqEventGfxEop           = 0x40,	///< EOP event from the Gfx pipe.
		} EqEventType;

		/** @brief Specifies the mode for assigning ordered append indices to dispatched CS wavefronts.

			Compute shaders which use the GDS ordered-append unit via the <c>ds_ordered_count</c> instruction have two options for ordering wavefronts:
			- Rely on the unit to order all wavefronts in dispatch order (by calling the instruction once for every wavefront).
			- Use the unit only to order all threadgroups in dispatch order (by calling the instruction once for every threadgroup) and handle ordering of
			waves within the threadgroup internally.  
			
			The compute shader technique used must agree with the hardware setting passed
			to the dispatch function in order for the ordered append functionality to work.

			@sa DrawCommandBuffer::dispatchWithOrderedAppend(), DrawCommandBuffer::dispatchIndirectWithOrderedAppend()
			*/
		typedef enum DispatchOrderedAppendMode
		{
			kDispatchOrderedAppendModeDisabled =				0,	///< CS dispatches will not assign a sequential ordered append index.
			kDispatchOrderedAppendModeIndexPerWavefront =		1,	///< CS dispatches will assign a sequential ordered append index per wavefront.
			kDispatchOrderedAppendModeIndexPerThreadgroup =		3,	///< CS dispatches will assign a sequential ordered append index per thread group.
		} DispatchOrderedAppendMode;

		/** @brief Defines the hardware debugging status flags. */
		typedef enum HardwareStatus
		{
			kHardwareStatusDump,		///< Dump the hardware status.	
		} HardwareStatus;


		/** @brief Defines which VS shaders which are embedded in the Gnm library.
			@sa DrawCommandBuffer::setEmbeddedVsShader()
			*/
		typedef enum EmbeddedVsShader
		{
			kEmbeddedVsShaderFullScreen          = 0, ///< A full screen VS shader.

			kNumEmbeddedVsShaders					///< The number of embedded VS shaders.	
		} EmbeddedVsShader;

		/** @brief Defines the set of PS shaders that are embedded in the Gnm library.
			@sa DrawCommandBuffer::setEmbeddedPsShader()
			*/
		typedef enum EmbeddedPsShader
		{
			kEmbeddedPsShaderDummy          = 0, ///< A dummy PS shader.
			kEmbeddedPsShaderDummyG32R32    = 1, ///< A dummy PS shader that exports R32 and G32.

			kNumEmbeddedPsShaders				///< The number of embedded PS shaders.
		} EmbeddedPsShader;

		/** @brief Encodes the ID of a workload stream.  
		
		PA Debug tools can use workload streams to identify "GPU work units."
			@see	Gnm::createWorkloadStream, Gnm::beginWorkload(), Gnm::destroyWorkloadStream()
		*/
		typedef uint32_t WorkloadStream;

		/** @brief Codes returned by the %Gnm workload creation functions.

		    @see Gnm::createWorkloadStream(), Gnm::beginWorkload(),
		    	 Gnm::endWorkload(), Gnm::destroyWorkloadStream()
		    */
		typedef enum WorkloadStatus
		{
			kWorkloadStatusOk					= 0,	///< The operation was performed successfully.
			kWorkloadStatusInvalidStream		= 1,	///< The stream parameter was invalid.
			kWorkloadStatusInvalidWorkload		= 2,	///< The workload parameter was invalid.
			kWorkloadStatusInvalidPointer		= 3, 	///< The required pointer argument was not set.
			kWorkloadStatusTooManyStreams		= 4, 	///< The maximum number of streams has already been allocated.
			kWorkloadStatusTooManyWorkloads		= 5,	///< The maximum number of active workloads for a stream has been reached.
			kWorkloadStatusStreamNotAllocated	= 6,	///< The stream to be destroyed was not created.	
			kWorkloadStatusInternalError		= 7,	///< The system was unable to perform the work.
		} WorkloadStatus;

		/** @brief Controls the render target index override by the draw call's render target index.
			@sa DrawPayloadControl
			*/
		typedef enum RenderTargetIndexOverrideMode
		{
			kRenderTargetIndexOverrideDisable			= 0,	///< The render target index from the draw call is suppressed by the render target index from the shader.
			kRenderTargetIndexOverrideWithDrawEnable	= 1		///< The render target index from the draw call will override the render target index from the shader.
		} RenderTargetIndexOverrideMode;

		/** @brief Controls the object and/or primitive ID propagation.
			@sa DrawPayloadControl
		 */
		typedef enum ObjectPrimIdMode
		{
			kObjectPrimIdDisable	= 0,	///< Disables propagation of the obj/prim ID.
			kObjectPrimIdEnable		= 1		///< Enables propagation of the obj/prim ID.
		} ObjectPrimIdMode;

		/** @brief Specifies whether objectIDs are per-instance or per-draw.
		*/
		typedef enum ObjectIdInstanceMode
		{
			kObjectIdInstanceDisable	= 0,	///< Disables per-instance object ID; the object ID is per-draw.
			kObjectIdInstanceEnable		= 1,	///< Enables per-instance object ID; the object ID is per-instance.
		} ObjectIdInstanceMode;


		/** @brief Controls the distribution of tessellated geometry between SEs.
			@sa HsStageRegisters::setTessellationDistributionMode()
		*/
		typedef enum TessellationDistributionMode
		{
			kTessellationDistributionNone	   = 0,		///< Legacy mode; the geometry stays on the same SE.
			kTessellationDistributionPatches   = 1,		///< Entire patches are sent to different SEs based on the threshold value.
			kTessellationDistributionDonuts	   = 2,		///< In addition to the <c>kTessellationDistributionPatches</c>, a patch can be split into donuts based on the threshold value.
			kTessellationDistributionTrapezoid = 3		///< In addition to the <c>kTessellationDistributionDonuts</c>, a donut can be split into trapezoids based on the threshold value.
		} TessellationDistributionMode;

		/** @brief Specifies the source of the Object ID data of a mesh.
			@sa DrawCommandBuffer::setObjectIdMode()
			*/
		typedef enum ObjectIdSource
		{
			kObjectIdSourceRegister = 0,///< Object IDs are sourced from a GPU register and are constant across an entire draw call.
			kObjectIdSourceShader   = 1	///< Object IDs are sourced from the vertex shader output, using the <c>S_OBJPRIM_ID</c> semantic.
		} ObjectIdSource;
		
		/** @brief Specifies whether to add the pipelined primitive ID to the object ID.
			@sa DrawCommandBuffer::setObjectIdMode()
			*/
		typedef enum AddPrimitiveId
		{
			kAddPrimitiveIdDisable = 0, ///< Do not add the pipelined primitive ID to the object ID.
			kAddPrimitiveIdEnable  = 1, ///< Add the pipelined primitive ID to the object ID.
		} AddPrimitiveId;

		/** @brief Specifies the type of metadata associated with a Texture object, if some sort of advanced texel compression is in use.

			       This happens only when the Texture is aliasing a compressed depth or color target.
				   NEO mode only.
			*/
		typedef enum TextureMetadataType
		{
			kTextureMetadataTypeDcc     = 0, ///< The metadata buffer contains DCC metadata. This can occur if the Texture aliases the color buffer of a RenderTarget.
			kTextureMetadataTypeCmask   = 1, ///< The metadata buffer contains CMASK metadata. This can occur if the Texture aliases the FMASK buffer of a RenderTarget.
			kTextureMetadataTypeHtile   = 2, ///< The metadata buffer contains HTILE metadata. This can occur if the Texture aliases a depth or stencil buffer of a DepthRenderTarget.
            kTextureMetadataTypeNone    = 3, ///< This texture does not have a metadata buffer.            
		} TextureMetadataType;

		/** @brief Specifies the meaning of each bit for <c>GPU_INFO</c> status.
		@see Gnm::getGpuInfoStatus()
		*/
		typedef enum GpuInfoStatus
		{
			kGpuInfoValid                  = 0x1 << 0,
			kProtectionFaultInfoValid      = 0x1 << 1,
			kBadPacketErrorInfoValid       = 0x1 << 2,
			kGpuInfoTriggerGpuError        = 0x1 << 8,
			kGpuInfoTriggerSubmitDone      = 0x1 << 9,
			kGpuInfoTriggerOther           = 0x1 << 10,
		} GpuInfoStatus;

		/** @brief Specifies the meaning of each bit for GPU block status.
			@see Gnm::getGpuBlockStatus()
		*/
		typedef enum GpuBlockName
		{
			kGpuBlockGraphicsPipe          = 0x1 << 0,
			kGpuBlockCp                    = 0x1 << 1,
			kGpuBlockCpf                   = 0x1 << 2,
			kGpuBlockCpg                   = 0x1 << 3,
			kGpuBlockCpc                   = 0x1 << 4,
			kGpuBlockCpCoherency           = 0x1 << 5,
			kGpuBlockWd                    = 0x1 << 6,
			kGpuBlockWdNoDma               = 0x1 << 7,
			kGpuBlockIa                    = 0x1 << 8,
			kGpuBlockIaNoDma               = 0x1 << 9,
			kGpuBlockVgt                   = 0x1 << 10,
			kGpuBlockPa                    = 0x1 << 11,
			kGpuBlockSc                    = 0x1 << 12,
			kGpuBlockSpi                   = 0x1 << 13,
			kGpuBlockTa                    = 0x1 << 14,
			kGpuBlockGds                   = 0x1 << 15,
			kGpuBlockSx                    = 0x1 << 16,
			kGpuBlockCb                    = 0x1 << 17,
			kGpuBlockDb                    = 0x1 << 18,
			kGpuBlockCbDirty               = 0x1 << 19,
			kGpuBlockDbDirty               = 0x1 << 20
		} GpuBlockName;

		/** @brief Specifies the meaning of each value for GPU block status.
			@see Gnm::getGpuBlockStatus()
			*/
		typedef enum GpuBlockStatus
		{
			kGpuBlockStatusIdle = 0,
			kGpuBlockStatusBusy = 1,
			kGpuBlockStatusClean = 0,
			kGpuBlockStatusDirty = 1
		} GpuBlockStatus;

		/** @brief Specifies the meaning of each bit for shader status. 
		
		These values are also used to
			get shader base address.
			@see Gnm::getShaderStatus(), Gnm::getShaderProgramBaseAddress()
		*/
		typedef enum ShaderProgramType
		{
			kShaderProgramTypeLs                = 0x1 << 0,
			kShaderProgramTypeHs                = 0x1 << 1,
			kShaderProgramTypeEs                = 0x1 << 2,
			kShaderProgramTypeGs                = 0x1 << 3,
			kShaderProgramTypeVs                = 0x1 << 4,
			kShaderProgramTypePs                = 0x1 << 5,

			kShaderProgramTypeCsG               = 0x1 << 10,

			kShaderProgramTypeCs0               = 0x1 << 15,
			kShaderProgramTypeCs1               = 0x1 << 16,
			kShaderProgramTypeCs2               = 0x1 << 17,
			kShaderProgramTypeCs3               = 0x1 << 18,
			kShaderProgramTypeCs4               = 0x1 << 19,
			kShaderProgramTypeCs5               = 0x1 << 20,
			kShaderProgramTypeCs6               = 0x1 << 21,
			kShaderProgramTypeCs7               = 0x1 << 22
		} ShaderProgramType;

        /** @brief Specifies which of MinZ and MaxZ in HTILE are force-summarized.
        */
        typedef enum ForceSummarization
        {
            kForceSummarizationOff,
            kForceSummarizationMinZ,
            kForceSummarizationMaxZ,
            kForceSummarizationBoth,
        } ForceSummarization;

	}
}
