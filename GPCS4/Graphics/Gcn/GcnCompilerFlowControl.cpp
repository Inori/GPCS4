#include "GcnCompiler.h"
#include "GcnAnalysis.h"

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
			auto& labelSet = m_analysis->branchLabels;
			if (labelSet.find(m_programCounter) == labelSet.end())
			{
				break;
			}

			uint32_t labelId = 0;

			auto iter = m_controlFlowBlocks.find(m_programCounter);
			if (iter == m_controlFlowBlocks.end())
			{
				// This is pre-label before the branch instruction,
				labelId = m_module.allocateId();

				GcnCfgBlock block;
				block.labelPtr = m_module.getInsertionPtr();
				block.lableId  = labelId;

				m_controlFlowBlocks.insert(
					std::make_pair(m_programCounter, block));
			}
			else
			{
				// This is a post-label after the branch instruction
				labelId = iter->second.lableId;
			}

		} while (false);
	}

	void GcnCompiler::emitScalarProgFlow(const GcnShaderInstruction& ins)
	{
		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::S_SWAPPC_B64:
			case GcnOpcode::S_ENDPGM:
				// Nothing to do.
				break;
			case GcnOpcode::S_CBRANCH_EXECZ:
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}
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