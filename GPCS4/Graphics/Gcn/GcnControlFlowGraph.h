#pragma once

#include "GcnCommon.h"
#include "GcnCompilerDefs.h"
#include "GcnInstructionIterator.h"

#include <string>
#include <unordered_set>
#include <boost/graph/adjacency_list.hpp>

namespace sce::gcn
{
	struct GcnBasicBlock
	{
		uint32_t           pcBegin;
		uint32_t           pcEnd;  // point to next instruction right after this block
		GcnInstructionList insList;
	};

	typedef boost::adjacency_list<
		boost::hash_setS, boost::vecS,
		boost::directedS,
		GcnBasicBlock>
		GcnControlFlowGraph;

	typedef GcnControlFlowGraph::vertex_descriptor 
		GcnCfgVertex;

	typedef GcnControlFlowGraph::edge_descriptor
		GcnCfgEdge;

	typedef boost::graph_traits<
		GcnControlFlowGraph>::adjacency_iterator 
		GcnAdjacencyIterator;

	/**
	 * \brief Control flow graph pass
	 * 
	 * Generate the CFG of the shader module.
	 */
	class GcnCfgPass : public GcnInstructionIterator
	{
	public:
		GcnCfgPass();
		virtual ~GcnCfgPass();

		const GcnControlFlowGraph& generateCfg(
			const GcnInstructionList& insList);

		static std::string dumpDot(
			const GcnControlFlowGraph& cfg);

	private:
		void generateLabels(
			const GcnInstructionList& insList);

		void generateVertices(
			const GcnInstructionList& insList);

		void generateEdges();

		std::optional<GcnControlFlowGraph::vertex_descriptor>
			findVertex(uint32_t pc);
	private:
		std::unordered_set<uint32_t> m_labels;
		GcnControlFlowGraph          m_cfg;
	};
}  // namespace sce::gcn