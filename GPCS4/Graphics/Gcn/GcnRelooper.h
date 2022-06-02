#pragma once

#include "GcnCommon.h"
#include "binaryen-c.h"
#include "mixed_arena.h"

#include <unordered_map>

namespace sce::gcn
{
	struct GcnControlFlowGraph;
	struct GcnShaderInstruction;
	struct GcnCfgVertex;

	/**
	 * \brief Relooper algorithm
	 * 
	 * A wrapper of binaryen's relooper.
	 * 
	 */
	class GcnRelooper
	{
	public:
		GcnRelooper();
		~GcnRelooper();

		BinaryenExpressionRef 
			buildAst(const GcnControlFlowGraph& cfg);

	private:
		RelooperBlockRef addVertices(
			const GcnControlFlowGraph& cfg);
		void addEdges(
			const GcnControlFlowGraph& cfg);

		std::vector<const GcnShaderInstruction*> copyInstructions(
			const std::vector<GcnShaderInstruction>& src);

		BinaryenOp getBranchOp(const GcnShaderInstruction& ins);
	private:
		BinaryenModuleRef m_module;
		RelooperRef       m_relooper;

		std::unordered_map<
			const GcnCfgVertex*,
			RelooperBlockRef> m_vertexMap;
	};


}  // namespace sce::gcn