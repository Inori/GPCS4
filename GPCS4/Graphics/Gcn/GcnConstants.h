#pragma once

namespace sce::gcn
{

	enum ShaderConstantDwordSize : uint32_t
	{
		kDwordSizeResource                  = 8,
		kDwordSizeRwResource                = 8,
		kDwordSizeSampler                   = 4,
		kDwordSizeVertexBuffer              = 4,
		kDwordSizeConstantBuffer            = 4,
		kDwordSizeBoolConstant              = 1,
		kDwordSizeFloatConstant             = 1,
		kDwordSizeAppendConsumeCounterRange = 1,
		kDwordSizeStreamoutBuffer           = 4,
		kDwordSizeExtendedUserData          = 128,
		kDwordSizeDispatchDrawData          = 32,
		kDwordSizeGdsMemoryRange            = 1,
	};

	enum GcnLimits : uint32_t
	{
		kMaxResourceCount        = 16,  ///< PSSL compiler limit is 128, Default value is 16
		kMaxRwResourceCount      = 16,  ///< PSSL compiler limit is 16, Default value is 16
		kMaxSamplerCount         = 16,  ///< PSSL compiler limit is 16, Default value is 16
		kMaxVertexBufferCount    = 32,  ///< PSSL compiler limit is 32, Default value is 16
		kMaxConstantBufferCount  = 20,  ///< PSSL compiler limit is 20, Default value is 20 Note: Because API-slots 15-19 are all reserved this value should remain 20
		kMaxStreamOutBufferCount = 4,   ///< PSSL compiler limit is 4, Default value is 4
		kMaxUserDataCount        = 16,  ///< PSSL compiler limit is 16, count not tracked by the InputResourceOffsets table
		kMaxSrtUserDataCount     = 16,  ///< PSSL compiler limit is 16, count not tracked by the InputResourceOffsets table
		kMaxResourceBufferCount  = 4,   ///< Maximum number for supported splits for the resource buffer per LCUE instance
		kMaxPsInputUsageCount    = 32,  ///< Maximum number of interpolants a PS Stage can receive
	};

}  // namespace sce::gcn
