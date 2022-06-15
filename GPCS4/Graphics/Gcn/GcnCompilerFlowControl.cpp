#include "GcnCompiler.h"
#include "GcnHeader.h"

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

	void GcnCompiler::emitScalarProgFlow(const GcnShaderInstruction& ins)
	{
		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::S_SWAPPC_B64:
				// Nothing to do.
				break;
			case GcnOpcode::S_ENDPGM:
				this->emitControlFlowReturn();
				break;
			case GcnOpcode::S_BRANCH:
			case GcnOpcode::S_CBRANCH_EXECZ:
			case GcnOpcode::S_CBRANCH_SCC0:
			case GcnOpcode::S_CBRANCH_SCC1:
			case GcnOpcode::S_CBRANCH_VCCZ:
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
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
			result.id       = m_module.opLogicalOr(m_module.defBoolType(),
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
			case GcnOpcode::S_BARRIER:
				m_module.opControlBarrier(m_module.constu32(spv::ScopeWorkgroup),
										  m_module.constu32(spv::ScopeWorkgroup),
										  m_module.constu32(spv::MemorySemanticsWorkgroupMemoryMask |
															spv::MemorySemanticsAcquireReleaseMask));
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}
	}

	void GcnCompiler::emitScalarWait(const GcnShaderInstruction& ins)
	{
		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::S_NOP:
				// Nothing to do.
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}
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