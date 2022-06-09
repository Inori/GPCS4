#include "GcnModule.h"
#include "GcnAnalysis.h"
#include "GcnCompiler.h"
#include "GcnDecoder.h"
#include "ControlFlowGraph/GcnStackifier.h"

#include "PlatFile.h"
#include "UtilString.h"

using namespace sce::vlt;

LOG_CHANNEL(Graphic.Gcn.GcnModule);

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
		// Decode shader binary
		const uint32_t* start = reinterpret_cast<const uint32_t*>(m_code);
		const uint32_t* end   = reinterpret_cast<const uint32_t*>(m_code + m_header.length());
		GcnCodeSlice    slice(start, end);

		auto insList = this->decodeShader(slice);
		
		// Generate global information
		GcnAnalysisInfo analysisInfo;

		GcnAnalyzer analyzer(
			m_programInfo,
			analysisInfo);

		this->runAnalyzer(analyzer, insList);

		// Do the compile
		// TODO:
		// Generate module info from device.
		GcnModuleInfo moduleInfo;
		moduleInfo.options.separateSubgroup = true;

		GcnCompiler compiler(
			this->name(),
			moduleInfo,
			m_programInfo,
			m_header,
			meta,
			analysisInfo);

		this->runCompiler(compiler, insList);

		return compiler.finalize();
	}
	
	GcnInstructionList GcnModule::decodeShader(GcnCodeSlice& slice) const
	{
		GcnInstructionList insList;
		GcnDecodeContext   decoder;

		// Decode and save instructions
		insList.reserve(m_header.length() / sizeof(uint32_t));
		while (!slice.atEnd())
		{
			decoder.decodeInstruction(slice);

			insList.emplace_back(
				decoder.getInstruction());
		}

		return insList;
	}

	void GcnModule::runAnalyzer(
		GcnAnalyzer& analyzer, const GcnInstructionList& insList) const
	{
		for (auto& ins : insList)
		{
			analyzer.processInstruction(ins);
		}
	}

	void GcnModule::runCompiler(
		GcnCompiler& compiler, const GcnInstructionList& insList) const
	{
		GcnCfgPass cfgPass;
		auto&      cfg = cfgPass.generateCfg(insList);

		GcnStackifier stackifier(cfg);
		auto          tokenList = stackifier.generate();

		compiler.compile(tokenList);
	}

}  // namespace sce::gcn