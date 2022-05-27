#pragma once

#include "GcnCommon.h"
#include "GcnTokenList.h"
#include "GcnLoopInfo.h"
#include "GcnDominatorTree.h"

#include <vector>

namespace sce::gcn
{
	/**
	 * \brief The stackifier algorithm
	 * 
	 * Build structured control flow graph
	 * from unstructured CFG for the shader.
	 * 
	 * Borrowed from Cheerp, originally designed
	 * by WebAssembly backend from LLVM project.
	 * 
	 * https://medium.com/leaningtech/solving-the-structured-control-flow-problem-once-and-for-all-5123117b1ee2
	 * https://github.com/leaningtech/cheerp-compiler/blob/cheerp-2.7/llvm/lib/CheerpWriter/CFGStackifier.cpp
	 */

	class GcnStackifier
	{
	public:
		GcnStackifier(GcnControlFlowGraph& cfg);
		~GcnStackifier();

	private:
		GcnControlFlowGraph& m_cfg;

		util::ObjectBank<GcnToken> m_pool;
		GcnTokenList               m_tokens;
		GcnTokenFactory            m_factory;
		GcnLoopInfo                m_loopInfo;
		GcnDominatorTree           m_domTree;
	};

}  // namespace sce::gcn