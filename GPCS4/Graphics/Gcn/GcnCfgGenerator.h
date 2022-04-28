#pragma once

#include "GcnCommon.h"
#include "GcnCompilerDefs.h"

#include <set>

namespace sce::gcn
{
	struct GcnShaderInstruction;
	struct GcnAnalysisInfo;

	// TODO:
	// Unlike DXVK, which has complete structured control
	// flow information embedded in dxbc binary,
	// we don' have such information in GCN binary.
	// As such, we need to generate ourselves.
	// This is a big todo.
	//
	// But for current, lacking of CFG information
	// (which is identify to lack of merge instruction in spir-v module)
	// is not a big problem because the shaders can still
	// be compiled, and the result it also correct, at least
	// on my Nvidia GTX 1660Ti.
	// And I believe the final compiled machine code executed on
	// GPU has no difference with or without this information,
	// so the performance is not a problem too.
	// The only downside of this is that the reverse compiled
	// glsl code from spir-v is full of repeated code blocks
	// making it hard to debug by human.

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
		GcnControlFlowGraph generateControlFlowGraph();

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