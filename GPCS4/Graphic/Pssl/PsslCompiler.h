#pragma once

#include "GPCS4Common.h"
#include "PsslDecoder.h"
#include "PsslProgramInfo.h"


namespace pssl
{;

class PsslCompiler
{
public:
	PsslCompiler(const PsslProgramInfo& progInfo);
	~PsslCompiler();

	void processInstruction(const PsslShaderInstruction& ins);
private:

};



} // namespace pssl
