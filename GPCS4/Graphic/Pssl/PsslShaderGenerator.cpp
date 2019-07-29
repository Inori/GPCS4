#include "PsslShaderGenerator.h"


namespace pssl
{;

PsslShaderGenerator::PsslShaderGenerator()
{

}

PsslShaderGenerator::~PsslShaderGenerator()
{

}

RcPtr<gve::GveShader> PsslShaderGenerator::compile(const uint32_t* code)
{
	PsslProgramInfo programInfo((uint8_t*)code);

	const uint32_t* codeEnd = code + programInfo.getCodeSizeDwords();
	GCNCodeSlice codeSlice(code, codeEnd);

	GCNCompiler compiler(programInfo);
	runCompiler(compiler, codeSlice);

	return compiler.finalize();
}

RcPtr<gve::GveShader> PsslShaderGenerator::compile(const uint32_t* code, const uint32_t* fsCode)
{
	PsslProgramInfo programInfo((uint8_t*)code);

	const uint32_t* codeEnd = code + programInfo.getCodeSizeDwords();
	GCNCodeSlice codeSlice(code, codeEnd);

	PsslFetchShader fsShader(fsCode);
	const uint32_t* fsCodeEnd = fsCode + fsShader.m_codeLengthDw;
	GCNCodeSlice fsCodeSlice(fsCode, fsCodeEnd);

	decodeFetchShader(fsCodeSlice, fsShader);

	GCNCompiler compiler(programInfo, fsShader);
	runCompiler(compiler, codeSlice);
}

void PsslShaderGenerator::runCompiler(GCNCompiler& compiler, GCNCodeSlice slice)
{
	GCNDecodeContext decoder;

	while (!slice.atEnd()) 
	{
		decoder.decodeInstruction(slice);

		compiler.processInstruction(decoder.getInstruction());
	}
}

void PsslShaderGenerator::decodeFetchShader(GCNCodeSlice slice, PsslFetchShader& fsShader)
{
	GCNDecodeContext decoder;

	while (!slice.atEnd())
	{
		decoder.decodeInstruction(slice);

		fsShader.m_instructionList.push_back(decoder.getInstruction());
	}
}

}  // namespace pssl