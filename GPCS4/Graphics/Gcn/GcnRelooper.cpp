#include "GcnRelooper.h"
#include "GcnInstructionUtil.h"
#include "GcnControlFlowGraph.h"

LOG_CHANNEL(Graphic.Gcn.GcnRelooper);

namespace sce::gcn
{
	GcnRelooper::GcnRelooper()
	{
		m_module   = BinaryenModuleCreate();
		m_relooper = RelooperCreate(m_module);
	}

	GcnRelooper::~GcnRelooper()
	{
		if (m_relooper)
		{
			RelooperDispose(m_relooper);
			m_relooper = nullptr;
		}

		if (m_module)
		{
			BinaryenModuleDispose(m_module);
			m_module = nullptr;
		}
	}

	BinaryenExpressionRef GcnRelooper::buildAst(
		const GcnControlFlowGraph& cfg)
	{
		auto entry = addVertices(cfg);

		addEdges(cfg);

		// Build the AST
		auto ast = RelooperRenderAndDispose(m_relooper, entry, 0);
		// Relooper is deleted internally, mark it with null.
		m_relooper = nullptr;

		BinaryenType        localTypes[] = { BinaryenTypeInt32() };  // state, free-for-label
		BinaryenFunctionRef theMain      = BinaryenAddFunction(
				 m_module, "main", BinaryenTypeNone(), BinaryenTypeNone(), localTypes, 1, ast);
		BinaryenSetStart(m_module, theMain);
	
		BinaryenModuleOptimizeGcn(m_module);
		assert(BinaryenModuleValidate(m_module));
		BinaryenModulePrint(m_module);

		return ast;
	}

	RelooperBlockRef GcnRelooper::addVertices(const GcnControlFlowGraph& cfg)
	{
		RelooperBlockRef entry = nullptr;
		for (auto& vtx : cfg.vertices)
		{
			auto insList = copyInstructions(vtx.insList);
			auto gcnCode = BinaryenGcnCode(m_module,
										   reinterpret_cast<BinaryenGcnInstructionRef>(insList.data()),
										   insList.size());
			auto block   = RelooperAddBlock(
				  m_relooper, BinaryenBlock(
								  m_module, nullptr, &gcnCode, 1, BinaryenTypeAuto()));

			// Recode gcn vertex to relooper block map,
			// this will be used when adding branches
			m_vertexMap[&vtx] = block;

			// Record the entry block
			if (entry == nullptr)
			{
				entry = block;
			}
		}
		return entry;
	}

	void GcnRelooper::addEdges(const GcnControlFlowGraph& cfg)
	{
		for (const auto& vtx : cfg.vertices)
		{
			auto  parent     = m_vertexMap[&vtx];
			auto& terminator = vtx.terminator;
			if (terminator.kind == GcnBlockTerminator::Unconditional)
			{
				auto succ  = terminator.successors[0];
				auto child = m_vertexMap[succ];
				RelooperAddBranch(parent, child, nullptr, nullptr);
			}
			else if (terminator.kind == GcnBlockTerminator::Conditional)
			{	
				auto op = getBranchOp(vtx.insList.back());

				auto labelTrue  = m_vertexMap[terminator.successors[0]];
				auto labelFalse = m_vertexMap[terminator.successors[1]];
				
				auto condition = BinaryenUnary(m_module,
											   op,
											   BinaryenNop(m_module));
				RelooperAddBranch(
					parent,
					labelTrue,
					condition,
					nullptr);
				RelooperAddBranch(
					parent,
					labelFalse,
					nullptr,
					nullptr);
			}
		}
	}

	std::vector<const GcnShaderInstruction*>
	GcnRelooper::copyInstructions(
		const std::vector<GcnShaderInstruction>& src)
	{
		std::vector<const GcnShaderInstruction*> result;
		for (const auto& ins : src)
		{
			if (isBranchInstruction(ins))
			{
				// Do not copy branch instructions,
				// as this these are translated to binaryen's IR
				continue;
			}
			result.push_back(&ins);
		}
		return result;
	}

	BinaryenOp GcnRelooper::getBranchOp(const GcnShaderInstruction& ins)
	{
		BinaryenOp op     = 0;
		auto       opcode = ins.opcode;
		switch (opcode)
		{
			case GcnOpcode::S_CBRANCH_SCC0:
				op = BinaryenScc0();
				break;
			case GcnOpcode::S_CBRANCH_SCC1:
				op = BinaryenScc1();
				break;
			case GcnOpcode::S_CBRANCH_VCCZ:
				op = BinaryenVccz();
				break;
			case GcnOpcode::S_CBRANCH_VCCNZ:
				op = BinaryenVccnz();
				break;
			case GcnOpcode::S_CBRANCH_EXECZ:
				op = BinaryenExecz();
				break;
			case GcnOpcode::S_CBRANCH_EXECNZ:
				op = BinaryenExecnz();
				break;
			default:
				LOG_ASSERT(false, "expected instruction.");
				break;
		}
		return op;
	}

}  // namespace sce::gcn