#include "GcnCompiler.h"

LOG_CHANNEL(Graphic.Gcn.GcnCompiler);

namespace sce::gcn
{
	void GcnCompiler::emitFlowControl(const GcnShaderInstruction& ins)
	{
		auto opClass = ins.opClass;
		switch (opClass)
		{
			case GcnInstClass::ScalarProgFlow:
				this->emitScalarProgFlow(ins);
				break;
			case GcnInstClass::ScalarSync:
				this->emitScalarSync(ins);
				break;
			case GcnInstClass::ScalarWait:
				this->emitScalarWait(ins);
				break;
			case GcnInstClass::ScalarCache:
				this->emitScalarCache(ins);
				break;
			case GcnInstClass::ScalarPrior:
				this->emitScalarPrior(ins);
				break;
			case GcnInstClass::ScalarRegAccess:
				this->emitScalarRegAccess(ins);
				break;
			case GcnInstClass::ScalarMsg:
				this->emitScalarMsg(ins);
				break;
		}
	}
	
	void GcnCompiler::emitBranchLabel()
	{
		do
		{
			auto iter = m_controlFlowBlocks.find(m_programCounter);
			if (iter == m_controlFlowBlocks.end())
			{
				break;
			}

			auto& block = iter->second;

			if (block.lableId == 0)
			{
				// This is a label before the branch instruction,
				// we need to allocate an id, and this will be used
				// when processing the branch instruction which jumps
				// to this label.
				block.lableId = m_module.allocateId();
			}

			// Terminate the block when inside.
			if (m_insideBlock)
			{
				m_module.opBranch(block.lableId);
			}

			m_module.opLabel(block.lableId);
			m_insideBlock = true;

		} while (false);
	}

	void GcnCompiler::emitScalarProgFlow(const GcnShaderInstruction& ins)
	{
		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::S_SWAPPC_B64:
				// Nothing to do.
				break;
			case GcnOpcode::S_ENDPGM:
			{
				if (m_insideBlock)
				{
					m_module.opReturn();
					m_insideBlock = false;
				}
			}
				break;
			case GcnOpcode::S_CBRANCH_EXECZ:
			case GcnOpcode::S_CBRANCH_SCC0:
			case GcnOpcode::S_CBRANCH_SCC1:
				emitScalarBranch(ins);
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}
	}

	void GcnCompiler::emitScalarBranch(const GcnShaderInstruction& ins)
	{
		auto sopp = gcnInstructionAs<GcnShaderInstSOPP>(ins);

		uint32_t branchTarget = getBranchTarget(ins);
		auto     iter         = m_controlFlowBlocks.find(branchTarget);
		// The target must be found, or there must be errors in analyzer.
		LOG_ASSERT(iter != m_controlFlowBlocks.end(), "can not find branch target");

		auto& block = iter->second;
		if (block.lableId == 0)
		{
			// This is a label after the branch instruction,
			// we need to allocate an id, and this will be used
			// when we reach the label address after processing
			// the branch instruction here.
			block.lableId = m_module.allocateId();
		}

		const uint32_t boolTypeId = m_module.defBoolType();

		uint32_t condition = 0;

		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::S_CBRANCH_EXECZ:
				condition = m_state.exec.zflag();
				break;
			case GcnOpcode::S_CBRANCH_SCC0:
				condition = m_module.opLogicalNot(boolTypeId,
												  m_module.opLoad(boolTypeId, m_state.scc.id));
				break;
			case GcnOpcode::S_CBRANCH_SCC1:
				condition = m_module.opLoad(boolTypeId, m_state.scc.id);
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}

		if (condition != 0)
		{
			uint32_t trueLabelId  = block.lableId;
			uint32_t falseLabelId = m_module.allocateId();

			// TODO:
			// This is a dummy merge, it's only used to pass spirv-val checks.
			// To implement a real merge block, see:
			// https://github.com/Inori/GPCS4/blob/shader_cfg/GPCS4/Graphics/Gcn/GcnCfgGenerator.h#L13
			// m_module.opSelectionMerge(falseLabelId, spv::SelectionControlMaskNone);

			m_module.opBranchConditional(condition, trueLabelId, falseLabelId);
			m_module.opLabel(falseLabelId);

			m_insideBlock = true;
		}
	}

	void GcnCompiler::emitUpdateScc(
		GcnRegisterValuePair& dst, GcnScalarType dstType)
	{
		// TODO:
		// Currently we only support zero test,
		// we need to support carry-out and borrow-in eventually.

		auto result = emitRegisterZeroTest(dst.low, GcnZeroTest::TestNz);
		
		if (isDoubleType(dstType))
		{
			auto resultHigh = emitRegisterZeroTest(dst.high, GcnZeroTest::TestNz);
			result.id       = m_module.opLogicalAnd(m_module.defBoolType(),
													result.id, resultHigh.id);
		}

		emitValueStore(m_state.scc, result, GcnRegMask::select(0));
	}

	void GcnCompiler::emitScalarSync(const GcnShaderInstruction& ins)
	{
		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::S_WAITCNT:
				// Nothing to do.
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}
	}

	void GcnCompiler::emitScalarWait(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitScalarCache(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitScalarPrior(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitScalarRegAccess(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitScalarMsg(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}
}  // namespace sce::gcn