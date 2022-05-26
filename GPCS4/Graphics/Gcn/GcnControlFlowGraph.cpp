#include "GcnControlFlowGraph.h"
#include "GcnInstructionUtil.h"
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

}  // namespace sce::gcn