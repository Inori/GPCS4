#include "PsslShaderModule.h"


namespace pssl
{;


PsslShaderModule::PsslShaderModule(uint32_t* code)
{
	initModule(code);
}

PsslShaderModule::PsslShaderModule(uint32_t* code, uint32_t* fsCode):
	m_fsCode(fsCode)
{
	initModule(code);
}

PsslShaderModule::~PsslShaderModule()
{

}


void PsslShaderModule::initModule(uint32_t* code)
{
	PsslProgramInfo programInfo((uint8_t*)code);
	
	uint32_t* codeEnd = code + programInfo.getCodeSizeDwords();
	PsslCodeSlice codeSlice(code, codeEnd);

	PsslCompiler compiler(programInfo);
	runCompiler(compiler, codeSlice);
}


RcPtr<gve::GveShader> PsslShaderModule::compile(uint32_t* code)
{

}

void PsslShaderModule::runCompiler(PsslCompiler& compiler, PsslCodeSlice slice)
{
	PsslDecoder decoder;

	while (!slice.atEnd()) 
	{
		decoder.decodeInstruction(slice);

		compiler.processInstruction(decoder.getInstruction());
	}
}

}  // namespace pssl