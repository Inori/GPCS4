#include "GcnCompiler.h"

#include "Violet/VltShader.h"

LOG_CHANNEL(Graphic.Gcn.GcnCompiler);

using namespace sce::vlt;

namespace sce::gcn
{
	GcnCompiler::GcnCompiler(
		const std::string&          fileName,
		const GcnProgramInfo&       programInfo,
		const GcnAnalysisInfo&      analysis,
		const std::vector<uint8_t>& code) :
		m_programInfo(programInfo),
		m_header(code),
		m_analysis(&analysis)
	{

	}

	GcnCompiler::~GcnCompiler()
	{
	}

	void GcnCompiler::processInstruction(
		const GcnShaderInstruction& ins)
	{
	}

	Rc<VltShader> GcnCompiler::finalize()
	{
	}

}  // namespace sce::gcn