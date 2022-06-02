#pragma once

#include "GcnCommon.h"
#include "GcnCompilerDefs.h"
#include "GcnInstructionIterator.h"
#include "UtilVector.h"
#include "UtilHash.h"

#include <string>
#include <unordered_set>
#include <unordered_map>

namespace sce::gcn
{
	struct GcnCfgVertex;

	struct GcnBlockTerminator
	{
		enum Kind
		{
			Unconditional,
			Conditional,
			Sink
		};

		Kind kind;

		// successors[0] is true label
		// successors[1] is false label if exist
		util::static_vector<GcnCfgVertex*, 2> successors;
	};

	/**
	 * \brief CFG vertex
	 * 
	 * A basic block is a vertex
	 */
	struct GcnCfgVertex
	{
		uint32_t           id;  // ease debug
		uint32_t           pcBegin;
		uint32_t           pcEnd;  // point to next instruction right after this block
		GcnBlockTerminator terminator;
		GcnInstructionList insList;
	};

	struct GcnCfgEdge
	{
		GcnCfgEdge(GcnCfgVertex* src, GcnCfgVertex* dst):
			source(src), target(dst)
		{
		}
		GcnCfgVertex* source = nullptr;
		GcnCfgVertex* target = nullptr;
	};

	struct GcnCfgEdgeHash
	{
		size_t operator()(const GcnCfgEdge& edge) const
		{
			return std::hash<std::pair<GcnCfgVertex*, GcnCfgVertex*>>{}(
				std::make_pair(edge.source, edge.target));
		}
	};

	struct GcnCfgEdgeEqual
	{
	public:
		size_t operator()(const GcnCfgEdge& lhs, const GcnCfgEdge& rhs) const
		{
			return lhs.source == rhs.source &&
				   lhs.target == rhs.target;
		}
	};

	struct GcnControlFlowGraph
	{
		std::vector<GcnCfgVertex> vertices;
		std::unordered_set<
			GcnCfgEdge,
			GcnCfgEdgeHash,
			GcnCfgEdgeEqual>
			edges;
		// Stores adjacent vertex,
		// in our situation, we have at most 2 children
		// so static_vector is fine.
		std::unordered_map<
			GcnCfgVertex*,
			util::static_vector<GcnCfgVertex*, 2>>
			adjacentMap;

		// Because we store pointers of vertex within one instance,
		// we shouldn't make this copyable, or the pointers will
		// be invalid.
		GcnControlFlowGraph()                           = default;
		GcnControlFlowGraph(const GcnControlFlowGraph&) = delete;
		GcnControlFlowGraph& operator=(const GcnControlFlowGraph&) = delete;
	};

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
		void generateTerminators();

		GcnCfgVertex* findVertex(uint32_t pc);

		GcnBlockTerminator::Kind 
			getTerminatorKind(const GcnShaderInstruction& ins);
	private:
		std::unordered_set<uint32_t> m_labels;
		GcnControlFlowGraph          m_cfg;
	};
}  // namespace sce::gcn