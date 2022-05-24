#include "GcnModule.h"
#include "GcnAnalysis.h"
#include "GcnCompiler.h"
#include "GcnDecoder.h"
#include "GcnControlFlowGraph.h"

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
		const uint32_t* start = reinterpret_cast<const uint32_t*>(m_code);
		const uint32_t* end   = reinterpret_cast<const uint32_t*>(m_code + m_header.length());
		GcnCodeSlice    slice(start, end);

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

		auto shaderName = util::str::formatex(m_programInfo.name(), "_", m_header.key().name());

		LOG_TRACE("compiling shader %s", shaderName.c_str());
		auto fileName = util::str::formatex("shaders/", shaderName, ".bin");
		auto dotName = util::str::formatex(shaderName, ".dot");

		////if (fileName.find("CS_SHDR_844598A0F388C19D") != std::string::npos)
		////{
		////	__debugbreak();
		////}

		//plat::StoreFile(fileName, start, m_header.length());

		//return nullptr;

		GcnCfgPass cfgPass;
		auto cfg = cfgPass.generateCfg(insList);
		auto       dot = GcnCfgPass::dumpDot(cfg);
		plat::StoreFile(dotName, dot.data(), dot.size());
		

		// TODO:
		// Generate module info from device.
		GcnModuleInfo moduleInfo;
		moduleInfo.options.separateSubgroup = true;

		GcnAnalysisInfo analysisInfo;

		GcnAnalyzer analyzer(
			m_programInfo,
			analysisInfo);

		this->runInstructionIterator(&analyzer, insList);

		GcnCompiler compiler(
			this->name(),
			moduleInfo,
			m_programInfo,
			m_header,
			meta,
			analysisInfo);

		this->runInstructionIterator(&compiler, insList);

		return compiler.finalize();
	}

	void GcnModule::runInstructionIterator(
		GcnInstructionIterator*   insIterator,
		const GcnInstructionList& insList) const
	{
		for (const auto& ins : insList)
		{
			insIterator->processInstruction(ins);
		}
	}

}  // namespace sce::gcn