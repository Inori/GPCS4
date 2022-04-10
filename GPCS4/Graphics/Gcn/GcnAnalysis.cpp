#include "GcnAnalysis.h"

namespace sce::gcn
{
	GcnAnalyzer::GcnAnalyzer(GcnAnalysisInfo& analysis):
		m_analysis(&analysis)
	{
	}

	GcnAnalyzer::~GcnAnalyzer()
	{
	}

	void GcnAnalyzer::processInstruction(
		const GcnShaderInstruction& ins)
	{
	}

}  // namespace sce::gcn