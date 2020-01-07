#pragma once

namespace pssl
{;
	
enum ShaderConstantDwordSize
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


}  // namespace pssl
