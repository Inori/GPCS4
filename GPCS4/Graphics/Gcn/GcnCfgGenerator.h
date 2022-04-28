#pragma once

#include "GcnCommon.h"
#include "GcnCompilerDefs.h"

#include <set>

namespace sce::gcn
{
	struct GcnShaderInstruction;
	struct GcnAnalysisInfo;

	/**
	 * \brief Generate control flow graph of a shader
	 * 
	 * The generated result is consumed by the compiler
	 * to proper handle 
	 */
	class GcnCfgGenerator
	{
	public:
		GcnCfgGenerator(GcnAnalysisInfo& analysis);
		~GcnCfgGenerator();

		void collectLabel(
			const GcnShaderInstruction& ins);

		void collectEdge(
			const GcnShaderInstruction& ins);

	private:
		GcnControlGraph generateControlFlowGraph();

		void updateProgramCounter(
			const GcnShaderInstruction& ins);

		void generateVertices();

		uint32_t getBranchTarget(
			const GcnShaderInstruction& ins);

	private:
		GcnAnalysisInfo*   m_analysis;
		void*              m_cfg;
		std::set<uint32_t> m_branchLabels;
		uint32_t           m_programCounter = 0;
	};


}  // namespace sce::gcn