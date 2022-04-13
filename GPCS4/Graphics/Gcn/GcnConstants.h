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
		kMaxUserDataCount = 16,
	};

}  // namespace sce::gcn
