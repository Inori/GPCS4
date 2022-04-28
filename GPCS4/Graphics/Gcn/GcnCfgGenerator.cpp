#include "GcnCfgGenerator.h"
#include "GcnInstruction.h"
#include "GcnDecoder.h"
#include "GcnAnalysis.h"

// I don't want boost headers spread everywhere,
// so I include them only in this cpp file.
// This will introduce some inconvenience.

#include <boost/utility.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/visitors.hpp>

using namespace boost;

namespace sce::gcn
{
	typedef adjacency_list<
		vecS,
		vecS,
		bidirectionalS> ControlFlowGraph;

	GcnCfgGenerator::GcnCfgGenerator(
		GcnAnalysisInfo& analysis):
		m_analysis(&analysis),
		m_cfg(new ControlFlowGraph())
	{
	}

	GcnCfgGenerator::~GcnCfgGenerator()
	{
		ControlFlowGraph* graph = reinterpret_cast<ControlFlowGraph*>(m_cfg);
		delete graph;
	}

	void GcnCfgGenerator::collectLabel(const GcnShaderInstruction& ins)
	{
		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::S_BRANCH:
			case GcnOpcode::S_CBRANCH_SCC0:
			case GcnOpcode::S_CBRANCH_SCC1:
			case GcnOpcode::S_CBRANCH_VCCZ:
			case GcnOpcode::S_CBRANCH_VCCNZ:
			case GcnOpcode::S_CBRANCH_EXECZ:
			case GcnOpcode::S_CBRANCH_EXECNZ:
			case GcnOpcode::S_CBRANCH_CDBGSYS:
			case GcnOpcode::S_CBRANCH_CDBGUSER:
			case GcnOpcode::S_CBRANCH_CDBGSYS_OR_USER:
			case GcnOpcode::S_CBRANCH_CDBGSYS_AND_USER:
			{
				uint32_t target = getBranchTarget(ins);
				m_branchLabels.insert(target);
			}
				break;
			case GcnOpcode::S_ENDPGM:
				// This is the first pass of the last instruction of a shader
				// we generate vertices here.
				generateVertices();
				break;
			default:
				break;
		}

		updateProgramCounter(ins);
	}

	void GcnCfgGenerator::collectEdge(const GcnShaderInstruction& ins)
	{
		ControlFlowGraph* graph = reinterpret_cast<ControlFlowGraph*>(m_cfg);

		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::S_ENDPGM:
				// This is the second pass of the last instruction of a shader
				// we generate the final result here.
				m_analysis->controlFlow = generateControlFlowGraph();
				break;
			default:
				break;
		}

		updateProgramCounter(ins);
	}

	void GcnCfgGenerator::generateVertices()
	{
		ControlFlowGraph* graph = reinterpret_cast<ControlFlowGraph*>(m_cfg);
	}

	uint32_t GcnCfgGenerator::getBranchTarget(const GcnShaderInstruction& ins)
	{
		auto     sopp   = gcnInstructionAs<GcnShaderInstSOPP>(ins);
		uint32_t target = m_programCounter + sopp.control.simm * 4 + 4;
		return target;
	}

	void GcnCfgGenerator::updateProgramCounter(const GcnShaderInstruction& ins)
	{
		m_programCounter += ins.length;
	}

	GcnControlFlowGraph GcnCfgGenerator::generateControlFlowGraph()
	{
		// Generate the final result for compiler use.
		ControlFlowGraph* graph = reinterpret_cast<ControlFlowGraph*>(m_cfg);

		return GcnControlFlowGraph();
	}

}  // namespace sce::gcn