#pragma once

#include "GPCS4Common.h"
#include "PsslDecoder.h"
#include "PsslProgramInfo.h"
#include "PsslFetchShader.h"
#include "../Gve/GveShader.h"
#include <optional>

namespace pssl
{;

class PsslCompiler
{
public:
	PsslCompiler(const PsslProgramInfo& progInfo,
		std::optional<PsslFetchShader> fsShader = {});
	~PsslCompiler();

	void processInstruction(const PsslShaderInstruction& ins);

	RcPtr<gve::GveShader> finalize();
private:
	
};



} // namespace pssl
