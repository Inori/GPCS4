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

		generateTerminators();

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

		uint32_t      vtxId      = 0;
		GcnCfgVertex* currentVtx = nullptr;

		for (const auto& ins : insList)
		{
			if (m_labels.find(m_programCounter) != m_labels.end())
			{
				GcnCfgVertex vertex;
				vertex.id      = vtxId++;
				vertex.pcBegin = m_programCounter;
				vertex.pcEnd   = m_programCounter + ins.length;  // in case there is only one instruction in this block
				vertex.insList.push_back(ins);

				m_cfg.vertices.push_back(vertex);
				currentVtx = &m_cfg.vertices.back();
			}
			else
			{
				currentVtx->insList.push_back(ins);
				currentVtx->pcEnd = m_programCounter + ins.length;
			}

			advanceProgramCounter(ins);
		}
	}

	void GcnCfgPass::generateEdges()
	{
		for (auto& vtx : m_cfg.vertices)
		{
			auto lastInst = vtx.insList.back();
			if (isTerminateInstruction(lastInst))
			{
				if (isUnconditionalBranch(lastInst))
				{
					uint32_t branchInstPc = vtx.pcEnd - lastInst.length;
					uint32_t targetPc     = calculateBranchTarget(branchInstPc, lastInst);
					auto     successor    = findVertex(targetPc);
					LOG_ASSERT(successor != nullptr, "can't find successor for unconditional branch.");

					m_cfg.edges.emplace(&vtx, successor);
					m_cfg.adjacentMap[&vtx].push_back(successor);
				}
				else if (isConditionalBranch(lastInst))
				{
					uint32_t branchInstPc = vtx.pcEnd - lastInst.length;
					uint32_t truePc       = calculateBranchTarget(branchInstPc, lastInst);
					uint32_t falsePc      = vtx.pcEnd;

					auto succTrue  = findVertex(truePc);
					auto succFalse = findVertex(falsePc);

					LOG_ASSERT(succTrue != nullptr && succFalse != nullptr, "can't find successor for conditional branch.");

					m_cfg.edges.emplace(&vtx, succTrue);
					m_cfg.edges.emplace(&vtx, succFalse);
					m_cfg.adjacentMap[&vtx].push_back(succTrue);
					m_cfg.adjacentMap[&vtx].push_back(succFalse);
				}
				else
				{
					// Don't add out edge for sink block.
				}
			}
			else
			{
				auto successor = findVertex(vtx.pcEnd);
				LOG_ASSERT(successor != nullptr, "can't find successor for direct block.");
				m_cfg.edges.emplace(&vtx, successor);
				m_cfg.adjacentMap[&vtx].push_back(successor);
			}
		}
	}

	void GcnCfgPass::generateTerminators()
	{
		for (auto& vtx : m_cfg.vertices)
		{
			auto& lastInst   = vtx.insList.back();
			auto& terminator = vtx.terminator;
			terminator.kind  = getTerminatorKind(lastInst);

			if (terminator.kind == GcnBlockTerminator::Sink)
			{
				continue;
			}

			for (const auto& succ : m_cfg.adjacentMap[&vtx])
			{
				terminator.successors.push_back(succ);
			}

			// Make sure successors[0] is true label
			// and successors[1] is false label
			if (terminator.kind == GcnBlockTerminator::Conditional)
			{
				auto     succ0      = terminator.successors[0];
				uint32_t blockEnd   = vtx.pcEnd;
				uint32_t succ0Begin = succ0->pcBegin;
				if (blockEnd == succ0Begin)
				{
					// If parent's end is equal to successor's begin,
					// then this successor is a false label,
					// in such case we swap the successors
					std::swap(terminator.successors[0], terminator.successors[1]);
				}
			}
		}
	}

	GcnCfgVertex* GcnCfgPass::findVertex(uint32_t pc)
	{
		GcnCfgVertex* result = nullptr;
		for (auto& vtx : m_cfg.vertices)
		{
			if (pc >= vtx.pcBegin && pc < vtx.pcEnd)
			{
				result = &vtx;
				break;
			}
		}
		return result;
	}

	std::string GcnCfgPass::dumpDot(const GcnControlFlowGraph& cfg)
	{
		std::stringstream dot;
		dot << "digraph shader {\n";

		auto getNodeName = [](const GcnCfgVertex& vtx) 
		{
			return str::format("label_%04X", vtx.pcBegin);
		};

		for (const auto& vtx : cfg.vertices)
		{
			auto  nodeName   = getNodeName(vtx);
			auto  shape      = vtx.terminator.kind == GcnBlockTerminator::Sink ? "box" : "ellipse";
			auto  attrList   = str::format("[label = \"%s-V%d\" shape=\"%s\"]",
										   nodeName.c_str(),
										   vtx.id,
										   shape);
			dot << nodeName << " " << attrList << ";\n";

			const auto iter = cfg.adjacentMap.find(const_cast<GcnCfgVertex*>(&vtx));
			if (iter == cfg.adjacentMap.end())
			{
				continue;
			}

			dot << nodeName << "->"
				<< "{";
			
			const auto& adjacents = iter->second;
			for (const auto& succ : adjacents)
			{
				auto nodeName = getNodeName(*succ);
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