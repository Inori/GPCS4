#include "GcnModule.h"
#include "GcnAnalysis.h"
#include "GcnCompiler.h"
#include "GcnDecoder.h"

#include "PlatFile.h"
#include "UtilString.h"


using namespace sce::vlt;

namespace sce::gcn
{
	GcnModule::GcnModule(
		GcnProgramType type,
		const uint8_t* code) :
		m_programInfo(type),
		m_header(code),
		m_code(code)
	{
	}

	GcnModule::~GcnModule()
	{
	}

	Rc<VltShader> GcnModule::compile(
		const GcnShaderMeta& meta) const
	{
		const uint32_t* start = reinterpret_cast<const uint32_t*>(m_code);
		const uint32_t* end   = reinterpret_cast<const uint32_t*>(m_code + m_header.length());
		GcnCodeSlice    codeSlice(start, end);

		auto fileName = util::str::formatex(
			"shaders/", 
			m_programInfo.name(), 
			"_",
			m_header.key().name(),
			".bin");

		if (fileName.find("CS_SHDR_844598A0F388C19D") != std::string::npos)
		{
			__debugbreak();
		}

		plat::StoreFile(fileName, start, m_header.length());

		GcnAnalysisInfo analysisInfo;

		GcnAnalyzer analyzer(
			m_programInfo,
			analysisInfo);

		this->runInstructionIterator(&analyzer, codeSlice);

		GcnCompiler compiler(
			this->name(),
			m_programInfo,
			m_header,
			meta,
			analysisInfo);

		this->runInstructionIterator(&compiler, codeSlice);

		return compiler.finalize();
	}


	void GcnModule::runInstructionIterator(
		GcnInstructionIterator* insIterator,
		GcnCodeSlice            slice) const
	{
		GcnDecodeContext decoder;

		while (!slice.atEnd())
		{
			decoder.decodeInstruction(slice);

			insIterator->processInstruction(
				decoder.getInstruction());
		}
	}

}  // namespace sce::gcn