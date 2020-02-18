#pragma once

#include "GnmCommon.h"

#include "../Pssl/PsslEnums.h"
#include "../Pssl/PsslShaderStructure.h"

namespace gve
{;
class GveShader;
class GveImageView;
class GveSharpResourceManager;
}  // namespace gve


struct GnmShaderContext
{
	using UDSTVector = std::vector<pssl::PsslShaderResource>;
	//
	const void*           code   = nullptr;
	RcPtr<gve::GveShader> shader = nullptr;
	UDSTVector            userDataSlotTable;
};
