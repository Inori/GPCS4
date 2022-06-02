#include "GcnControlFlowGraph.h"
#include "Gcn/GcnInstructionUtil.h"
#include "UtilString.h"

LOG_CHANNEL(Graphic.Gcn.GcnControlFlowGraph);

using namespace util;

namespace sce::gcn
{

	GcnCfgPass::GcnCfgPass()
	{
	}

	GcnCfgPass::~GcnCfgPass()
	{
	}

	const GcnControlFlowGraph& GcnCfgPass::generateCfg(
		const GcnInstructionList& insList)
	{
		generateLabels(insList);

		generateVertices(insList);

		generateEdges();

		generateTerminators();

		generatePredecessors();

		return m_cfg;
	}

	void GcnCfgPass::generateLabels(
		const GcnInstructionList& insList)
	{
		// Always set a label at entry point.
		m_labels.insert(0);

		for (const auto& ins : insList)
		{
			if (isUnconditionalBranch(ins))
			{
				uint32_t target = getBranchTarget(ins);
				m_labels.insert(target);
			}
			else if (isConditionalBranch(ins))
			{
				uint32_t trueLabel  = getBranchTarget(ins);
				uint32_t falseLabel = m_programCounter + ins.length;
				m_labels.insert(trueLabel);
				m_labels.insert(falseLabel);
			}
			else if (ins.opcode == GcnOpcode::S_ENDPGM)
			{
				uint32_t nextLabel = m_programCounter + ins.length;
				m_labels.insert(nextLabel);
			}

			advanceProgramCounter(ins);
		}
	}

	void GcnCfgPass::generateVertices(
		const GcnInstructionList& insList)
	{
		resetProgramCounter();

		GcnControlFlowGraph::vertex_descriptor currentVtx;

		for (const auto& ins : insList)
		{
			if (m_labels.find(m_programCounter) != m_labels.end())
			{
				GcnBasicBlock block;
				block.pcBegin = m_programCounter;
				block.pcEnd   = m_programCounter + ins.length; // in case there is only one instruction in this block
				block.insList.push_back(ins);
				currentVtx = boost::add_vertex(block, m_cfg);
			}
			else
			{
				m_cfg[currentVtx].insList.push_back(ins);
				m_cfg[currentVtx].pcEnd = m_programCounter + ins.length;
			}

			advanceProgramCounter(ins);
		}
	}

	void GcnCfgPass::generateEdges()
	{
		for (const auto& vtx : boost::make_iterator_range(boost::vertices(m_cfg)))
		{
			auto& basicBlock = m_cfg[vtx];
			auto  lastInst   = basicBlock.insList.back();
			if (isTerminateInstruction(lastInst))
			{
				if (isUnconditionalBranch(lastInst))
				{
					uint32_t branchInstPc = basicBlock.pcEnd - lastInst.length;
					uint32_t targetPc     = calculateBranchTarget(branchInstPc, lastInst);
					auto     successor    = findVertex(targetPc);
					LOG_ASSERT(successor.has_value(), "can't find successor for unconditional branch.");
					boost::add_edge(vtx, successor.value(), m_cfg);
				}
				else if (isConditionalBranch(lastInst))
				{
					uint32_t branchInstPc = basicBlock.pcEnd - lastInst.length;
					uint32_t truePc       = calculateBranchTarget(branchInstPc, lastInst);
					uint32_t falsePc      = basicBlock.pcEnd;

					auto succTrue  = findVertex(truePc);
					auto succFalse = findVertex(falsePc);

					LOG_ASSERT(succTrue.has_value() && succFalse.has_value(), "can't find successor for conditional branch.");

					boost::add_edge(vtx, succTrue.value(), m_cfg);
					boost::add_edge(vtx, succFalse.value(), m_cfg);
				}
				else
				{
					// Don't add out edge for sink block.
				}
			}
			else
			{
				auto successor = findVertex(basicBlock.pcEnd);
				LOG_ASSERT(successor.has_value(), "can't find successor for direct block.");
				boost::add_edge(vtx, successor.value(), m_cfg);
			}
		}
	}

	void GcnCfgPass::generateTerminators()
	{
		for (const auto& vtx : boost::make_iterator_range(boost::vertices(m_cfg)))
		{
			auto& basicBlock = m_cfg[vtx];
			auto& lastInst             = basicBlock.insList.back();
			auto& terminator           = basicBlock.terminator;
			terminator.kind            = getTerminatorKind(lastInst);

			if (terminator.kind == GcnBlockTerminator::Sink)
			{
				continue;
			}

			for (const auto& succ : boost::make_iterator_range(adjacent_vertices(vtx, m_cfg)))
			{
				terminator.successors.push_back(succ);
			}

			// Make sure successors[0] is true label
			// and successors[1] is false label
			if (terminator.kind == GcnBlockTerminator::Conditional)
			{
				auto     succ0      = terminator.successors[0];
				uint32_t blockEnd   = basicBlock.pcEnd;
				uint32_t succ0Begin = m_cfg[succ0].pcBegin;
				if (blockEnd == succ0Begin)
				{
					// If parent's end is equal to successor's begin,
					// then this successor is a false label,
					// in such case we swap the successors.
					// In a rare case which true and false label is the same, 
					// then it doesn't matter if we swap or not.
					std::swap(terminator.successors[0], terminator.successors[1]);
				}
			}
		}
	}

	void GcnCfgPass::generatePredecessors()
	{
		for (const auto& vtx : boost::make_iterator_range(boost::vertices(m_cfg)))
		{
			for (const auto& edge : boost::make_iterator_range(boost::in_edges(vtx, m_cfg)))
			{
				auto pred = boost::source(edge, m_cfg);
				m_cfg[vtx].predecessors.push_back(pred);
			}
		}
	}

	std::optional<sce::gcn::GcnControlFlowGraph::vertex_descriptor> 
		GcnCfgPass::findVertex(uint32_t pc)
	{
		std::optional<sce::gcn::GcnControlFlowGraph::vertex_descriptor> result;
		for (const auto& vtx : boost::make_iterator_range(boost::vertices(m_cfg)))
		{
			if (pc >= m_cfg[vtx].pcBegin && pc < m_cfg[vtx].pcEnd)
			{
				result.emplace(vtx);
				break;
			}
		}
		return result;
	}

	std::string GcnCfgPass::dumpDot(const GcnControlFlowGraph& cfg)
	{
		std::stringstream dot;
		dot << "digraph shader {\n";

		for (const auto& vtx : boost::make_iterator_range(boost::vertices(cfg)))
		{
			auto& basicBlock = cfg[vtx];
			auto  nodeName   = str::format("label_%04X", basicBlock.pcBegin);
			auto  shape      = basicBlock.terminator.kind == GcnBlockTerminator::Sink ? "box" : "ellipse";
			auto  attrList   = str::format("[label = \"%s-V%d\" shape=\"%s\"]",
										   nodeName.c_str(),
										   static_cast<uint32_t>(vtx),
										   shape);
			dot << nodeName << " " << attrList << ";\n";

			dot << nodeName << "->" << "{";
			for (const auto& succ : boost::make_iterator_range(adjacent_vertices(vtx, cfg)))
			{
				auto& basicBlock = cfg[succ];
				auto  nodeName   = str::format("label_%04X", basicBlock.pcBegin);
				dot << nodeName << ";";
			}
			dot << "}\n";
		}

		dot << "}\n";
		return dot.str();
	}

	GcnBlockTerminator::Kind GcnCfgPass::getTerminatorKind(const GcnShaderInstruction& ins)
	{
		GcnBlockTerminator::Kind kind;
		if (isTerminateInstruction(ins))
		{
			if (isConditionalBranch(ins))
			{
				kind = GcnBlockTerminator::Kind::Conditional;
			}
			else if (isUnconditionalBranch(ins))
			{
				kind = GcnBlockTerminator::Kind::Unconditional;
			}
			else
			{
				kind = GcnBlockTerminator::Kind::Sink;
			}
		}
		else
		{
			// Block will fall through to the next block directly,
			// as if a unconditional branch.
			kind = GcnBlockTerminator::Kind::Unconditional;
		}
		return kind;
	}



}  // namespace sce::gcn