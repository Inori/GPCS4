#pragma once

#include "GPCS4Common.h"
#include "PsslProgramInfo.h"
#include "PsslFetchShader.h"
#include "../Gve/GveShader.h"
#include <optional>

namespace pssl
{;

class GCNCompiler
{
public:
	GCNCompiler(const PsslProgramInfo& progInfo,
		std::optional<PsslFetchShader> fsShader = {});
	~GCNCompiler();

	void processInstruction(GCNInstruction& ins);

	RcPtr<gve::GveShader> finalize();
private:
	
};



} // namespace pssl
