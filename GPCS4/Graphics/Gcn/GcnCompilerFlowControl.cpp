#include "GcnCompiler.h"
#include "ControlFlowGraph/GcnTokenList.h"
#include "fmt/format.h"

using namespace sce::vlt;

LOG_CHANNEL(Graphic.Gcn.GcnCompiler);

namespace sce::gcn
{
	void GcnCompiler::compileGlobalVariable(const GcnTokenList& tokens)
	{
		// Initialize global variables
		for (const auto& token : tokens)
		{
			if (token->kind() != GcnTokenKind::Variable)
			{
				continue;
			}

			compileTokenVariable(*token);
		}
	}

	void GcnCompiler::compileTokenLoop(const GcnToken& token)
	{
		this->emitControlFlowLoop(token);
	}

	void GcnCompiler::compileTokenBlock(const GcnToken& token)
	{
		this->emitControlFlowBlock(token);
	}

	void GcnCompiler::compileTokenIf(const GcnToken& token)
	{
		this->emitControlFlowIf(token);
	}

	void GcnCompiler::compileTokenIfNot(const GcnToken& token)
	{
		this->emitControlFlowIf(token);
	}

	void GcnCompiler::compileTokenElse(const GcnToken& token)
	{
		this->emitControlFlowElse(token);
	}

	void GcnCompiler::compileTokenBranch(const GcnToken& token)
	{
		this->emitControlFlowBreak(token);
	}

	void GcnCompiler::compileTokenEnd(const GcnToken& token)
	{
		auto match = token.getMatch();
		auto kind  = match->kind();
		switch (kind)
		{
			case GcnTokenKind::If:
			case GcnTokenKind::IfNot:
				this->emitControlFlowEndIf(token);
				break;
			case GcnTokenKind::Loop:
				this->emitControlFlowEndLoop(token);
				break;
			case GcnTokenKind::Block:
				this->emitControlFlowEndBlock(token);
				break;
			default:
				Logger::exception(fmt::format("GcnCompiler: Invalid header kind: {}", (uint32_t)kind));
				break;
		}
	}

	void GcnCompiler::compileTokenVariable(const GcnToken& token)
	{
		auto key  = &token;
		auto iter = m_tokenVariables.find(key);
		if (iter == m_tokenVariables.end())
		{
			const auto& value = token.getValue();

			// create a global variable
			GcnRegisterInfo info;
			info.type.ctype   = value.type;
			info.type.ccount  = 1;
			info.type.alength = 0;
			info.sclass       = spv::StorageClassPrivate;
			uint32_t varId    = emitNewVariable(info);
			m_module.setDebugName(varId, fmt::format("goto_{}", varId).c_str());

			// init with a value
			auto init = emitBuildConstValue(value.value, value.type);
			m_module.opStore(varId, init.id);

			// save the new variable
			GcnRegisterPointer ptr;
			ptr.type.ctype        = info.type.ctype;
			ptr.type.ccount       = info.type.ccount;
			ptr.id                = varId;
			m_tokenVariables[key] = ptr;
		}
	}

	void GcnCompiler::compileTokenSetValue(const GcnToken& token)
	{
		auto target = token.getMatch();
		auto iter   = m_tokenVariables.find(target);

		LOG_ASSERT(iter != m_tokenVariables.end(), "Value set before define.");

		auto& value    = token.getValue();
		auto& var      = iter->second;
		auto  newValue = emitBuildConstValue(value.value, value.type);
		m_module.opStore(var.id, newValue.id);
	}	

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

	uint32_t GcnCompiler::emitComputeDivergence()
	{
		// For compute shader, we use even subgroup against low 32 bits of exec
		// and odd subgroup for high 32 bits.
		/*
		 * if (mod(gl_SubgroupID, 2.0) < 1.0)
		 * {
		 *	value = exec_lo
		 * }
		 * else
		 * {
		 *   value = exec_hi
		 * }
		 * result = (value & gl_SubgroupEqMask.x) != 0;
		 */
		const uint32_t utypeId = getScalarTypeId(GcnScalarType::Uint32);
		const uint32_t ftypeId = getScalarTypeId(GcnScalarType::Float32);
		const uint32_t btypeId = m_module.defBoolType();

		auto subgroupId = emitCommonSystemValueLoad(GcnSystemValue::SubgroupID, GcnRegMask::select(0));
		// float modulo should be faster than integer modulo
		auto subFId = m_module.opConvertUtoF(ftypeId, subgroupId.id);
		auto isEven = m_module.opFOrdLessThan(btypeId, subFId, m_module.constf32(1.0));

		uint32_t labelEven  = m_module.allocateId();
		uint32_t labelOdd   = m_module.allocateId();
		uint32_t labelMerge = m_module.allocateId();

		m_module.opSelectionMerge(labelMerge, spv::SelectionControlMaskNone);
		m_module.opBranchConditional(isEven, labelEven, labelOdd);
		m_module.opLabel(labelEven);

		auto execLo = m_state.exec.emitLoad(GcnRegMask::select(0));

		m_module.opBranch(labelMerge);
		m_module.opLabel(labelOdd);

		auto execHi = m_state.exec.emitLoad(GcnRegMask::select(1));

		m_module.opBranch(labelMerge);
		m_module.opLabel(labelMerge);

		// Merge the result with a phi function
		const std::array<SpirvPhiLabel, 2> phiLabels = { {
			{ execLo.low.id, labelEven },
			{ execHi.low.id, labelOdd },
		} };

		uint32_t exec = m_module.opPhi(
			utypeId, phiLabels.size(), phiLabels.data());
		auto mask = emitCommonSystemValueLoad(
			GcnSystemValue::SubgroupEqMask, GcnRegMask::select(0));

		auto     value  = m_module.opBitwiseAnd(utypeId, exec, mask.id);
		uint32_t result = m_module.opINotEqual(btypeId, value, m_module.constu32(0));
		return result;
	}

	uint32_t GcnCompiler::emitControlFlowDivergence()
	{
		uint32_t result = 0;

		const uint32_t utypeId = getScalarTypeId(GcnScalarType::Uint32);
		const uint32_t btypeId = m_module.defBoolType();
		// We cheat the shader as if the CU only provide one single thread,
		// so we only set EXEC bit against invocation id.
		if (m_moduleInfo.options.separateSubgroup)
		{
			auto mask = emitCommonSystemValueLoad(
				GcnSystemValue::SubgroupEqMask, GcnRegMask::select(0));
			// For non-compute shader, we only use low 32 bits of exec.
			auto exec  = m_state.exec.emitLoad(GcnRegMask::select(0));
			auto value = m_module.opBitwiseAnd(utypeId, exec.low.id, mask.id);
			result     = m_module.opINotEqual(btypeId, value, m_module.constu32(0));
		}
		else
		{
			auto mask = emitCommonSystemValueLoad(
				GcnSystemValue::SubgroupEqMask, GcnRegMask::firstN(2));

			auto exec = m_state.exec.emitLoad(GcnRegMask::firstN(2));

			auto maskX   = emitRegisterExtract(mask, GcnRegMask::select(0));
			auto maskY   = emitRegisterExtract(mask, GcnRegMask::select(1));
			auto valueX  = m_module.opBitwiseAnd(utypeId, exec.low.id, maskX.id);
			auto valueY  = m_module.opBitwiseAnd(utypeId, exec.high.id, maskY.id);
			auto resultX = m_module.opINotEqual(btypeId, valueX, m_module.constu32(0));
			auto resultY = m_module.opINotEqual(btypeId, valueY, m_module.constu32(0));
			result       = m_module.opLogicalOr(btypeId, resultX, resultY);
		}
		return result;
	}

	uint32_t GcnCompiler::emitControlFlowCondition(const GcnTokenCondition& condition)
	{
		uint32_t result = 0;

		const uint32_t typeId = m_module.defBoolType();

		auto loadCompareValue = [&]()
		{
			auto        key     = condition.condVar;
			const auto& condVar = m_tokenVariables[key];
			uint32_t    lhs     = m_module.opLoad(getVectorTypeId(condVar.type), condVar.id);

			auto     cmpValue = emitBuildConstValue(condition.cmpValue, condVar.type.ctype);
			uint32_t rhs      = cmpValue.id;
			return std::make_pair(lhs, rhs);
		};

		auto op = condition.op;
		switch (op)
		{
			case GcnConditionOp::EqBool:
			{
				auto cmpValue = loadCompareValue();
				result        = m_module.opLogicalEqual(typeId, cmpValue.first, cmpValue.second);
			}
			break;
			case GcnConditionOp::NeBool:
			{
				auto cmpValue = loadCompareValue();
				result        = m_module.opLogicalNotEqual(typeId, cmpValue.first, cmpValue.second);
			}
			break;
			case GcnConditionOp::Scc0:
			{
				uint32_t scc = m_module.opLoad(typeId, m_state.scc.id);
				result       = m_module.opLogicalNot(typeId, scc);
			}
			break;
			case GcnConditionOp::Scc1:
				result = m_module.opLoad(typeId, m_state.scc.id);
				break;
			case GcnConditionOp::Vccz:
				result = m_state.vcc.zflag();
				break;
			case GcnConditionOp::Vccnz:
			{
				uint32_t vccz = m_state.vcc.zflag();
				result        = m_module.opLogicalNot(typeId, vccz);
			}
			break;
			case GcnConditionOp::Execz:
				result = m_state.exec.zflag();
				break;
			case GcnConditionOp::Execnz:
			{
				uint32_t execz = m_state.exec.zflag();
				result         = m_module.opLogicalNot(typeId, execz);
			}
			break;
			case GcnConditionOp::Divergence:
				result = emitControlFlowDivergence();
				break;
			case GcnConditionOp::EqU32:
			case GcnConditionOp::NeU32:
			case GcnConditionOp::GeU32:
			case GcnConditionOp::GtU32:
			case GcnConditionOp::LeU32:
			case GcnConditionOp::LtU32:
				LOG_GCN_UNHANDLED_INST();
				break;
		}
		return result;
	}

	void GcnCompiler::emitControlFlowIf(const GcnToken& token)
	{
		const auto& condition   = token.getCondition();
		uint32_t    conditionId = emitControlFlowCondition(condition);

		if (token.kind() == GcnTokenKind::IfNot)
		{
			conditionId = m_module.opLogicalNot(m_module.defBoolType(), conditionId);
		}

		// Declare the 'if' block. We do not know if there
		// will be an 'else' block or not, so we'll assume
		// that there is one and leave it empty otherwise.
		GcnCfgBlock block;
		block.type             = GcnCfgBlockType::If;
		block.b_if.conditionId = conditionId;
		block.b_if.labelIf     = m_module.allocateId();
		block.b_if.labelElse   = 0;
		block.b_if.labelEnd    = m_module.allocateId();
		block.b_if.headerPtr   = m_module.getInsertionPtr();
		m_controlFlowStack.push_back(block);

		// We'll insert the branch instruction when closing
		// the block, since we don't know whether or not an
		// else block is needed right now.
		m_module.opLabel(block.b_if.labelIf);
	}

	void GcnCompiler::emitControlFlowElse(const GcnToken& token)
	{
		if (m_controlFlowStack.size() == 0 ||
			m_controlFlowStack.back().type != GcnCfgBlockType::If ||
			m_controlFlowStack.back().b_if.labelElse != 0)
		{
			Logger::exception("GcnCompiler: 'Else' without 'If' found");
		}

		GcnCfgBlock& block = m_controlFlowStack.back();

		if (m_blockTerminators.empty())
		{
			// Set the 'Else' flag so that we do
			// not insert a dummy block on 'EndIf'
			block.b_if.labelElse = m_module.allocateId();
			// Close the 'If' block by branching to
			// the merge block we declared earlier
			m_module.opBranch(block.b_if.labelEnd);
			m_module.opLabel(block.b_if.labelElse);
		}
		else
		{
			// if the block is already closed by other terminator instruction,
			// then don't emit branch
			block.b_if.labelElse = m_blockTerminators.back();
			// remove the terminator
			m_blockTerminators.pop_back();
		}
	}

	void GcnCompiler::emitControlFlowEndIf(const GcnToken& token)
	{
		if (m_controlFlowStack.size() == 0 ||
			m_controlFlowStack.back().type != GcnCfgBlockType::If)
		{
			Logger::exception("GcnCompiler: 'EndIf' without 'If' found");
		}

		// Remove the block from the stack, it's closed
		GcnCfgBlock block = m_controlFlowStack.back();
		m_controlFlowStack.pop_back();

		uint32_t labelMerge = m_blockTerminators.empty()
								  ? block.b_if.labelEnd
								  : m_blockTerminators.back();

		// Write out the 'if' header
		m_module.beginInsertion(block.b_if.headerPtr);

		m_module.opSelectionMerge(
			labelMerge,
			spv::SelectionControlMaskNone);

		m_module.opBranchConditional(
			block.b_if.conditionId,
			block.b_if.labelIf,
			block.b_if.labelElse != 0
				? block.b_if.labelElse
				: labelMerge);

		m_module.endInsertion();

		if (m_blockTerminators.empty())
		{
			// End the active 'if' or 'else' block
			m_module.opBranch(labelMerge);
			m_module.opLabel(labelMerge);
		}
		else
		{
			// if the block is already closed by other terminator instruction,
			// then don't emit branch
			// remove the terminator
			m_blockTerminators.pop_back();
		}
	}

	void GcnCompiler::emitControlFlowLoop(const GcnToken& token)
	{
		// Declare the 'loop' block
		GcnCfgBlock block;
		block.type                 = GcnCfgBlockType::Loop;
		block.b_loop.labelHeader   = m_module.allocateId();
		block.b_loop.labelBegin    = m_module.allocateId();
		block.b_loop.labelContinue = m_module.allocateId();
		block.b_loop.labelBreak    = m_module.allocateId();
		m_controlFlowStack.push_back(block);

		m_module.opBranch(block.b_loop.labelHeader);
		m_module.opLabel(block.b_loop.labelHeader);

		m_module.opLoopMerge(
			block.b_loop.labelBreak,
			block.b_loop.labelContinue,
			spv::LoopControlMaskNone);

		m_module.opBranch(block.b_loop.labelBegin);
		m_module.opLabel(block.b_loop.labelBegin);

		// This will make for(;;) becomes while(true)
		uint32_t labelTrue = m_module.allocateId();
		m_module.opBranchConditional(m_module.constBool(true),
									 labelTrue,
									 block.b_loop.labelBreak);
		m_module.opLabel(labelTrue);
	}

	void GcnCompiler::emitControlFlowEndLoop(const GcnToken& token)
	{
		if (m_controlFlowStack.size() == 0 ||
			m_controlFlowStack.back().type != GcnCfgBlockType::Loop)
		{
			Logger::exception("GcnCompiler: 'EndLoop' without 'Loop' found");
		}

		// Remove the block from the stack, it's closed
		const GcnCfgBlock block = m_controlFlowStack.back();
		m_controlFlowStack.pop_back();

		if (needBreakLoop(token))
		{
			m_module.opBranch(block.b_loop.labelBreak);
			// following OpBranch need a open block
			uint32_t label = m_module.allocateId();
			m_module.opLabel(label);
		}

		// Declare the continue block
		m_module.opBranch(block.b_loop.labelContinue);
		m_module.opLabel(block.b_loop.labelContinue);

		// Declare the merge block
		m_module.opBranch(block.b_loop.labelHeader);
		m_module.opLabel(block.b_loop.labelBreak);
	}

	void GcnCompiler::emitControlFlowBlock(const GcnToken& token)
	{
		// we use do {} while(false); to implement block,
		// so it's generally a loop.

		// Declare the 'loop' block
		GcnCfgBlock block;
		block.type                 = GcnCfgBlockType::Loop;
		block.b_loop.labelHeader   = m_module.allocateId();
		block.b_loop.labelBegin    = m_module.allocateId();
		block.b_loop.labelContinue = m_module.allocateId();
		block.b_loop.labelBreak    = m_module.allocateId();
		m_controlFlowStack.push_back(block);

		m_module.opBranch(block.b_loop.labelHeader);
		m_module.opLabel(block.b_loop.labelHeader);

		m_module.opLoopMerge(
			block.b_loop.labelBreak,
			block.b_loop.labelContinue,
			spv::LoopControlMaskNone);

		m_module.opBranch(block.b_loop.labelBegin);
		m_module.opLabel(block.b_loop.labelBegin);
	}

	void GcnCompiler::emitControlFlowEndBlock(const GcnToken& token)
	{
		if (m_controlFlowStack.size() == 0 ||
			m_controlFlowStack.back().type != GcnCfgBlockType::Loop)
		{
			Logger::exception("GcnCompiler: 'EndLoop' without 'Loop' found");
		}

		// Remove the block from the stack, it's closed
		const GcnCfgBlock block = m_controlFlowStack.back();
		m_controlFlowStack.pop_back();

		// Declare the continue block
		m_module.opBranch(block.b_loop.labelContinue);
		m_module.opLabel(block.b_loop.labelContinue);

		// Declare the merge block,
		// we use do {} while(false); to implement a block,
		m_module.opBranchConditional(m_module.constBool(false),
									 block.b_loop.labelHeader,
									 block.b_loop.labelBreak);
		m_module.opLabel(block.b_loop.labelBreak);
	}

	void GcnCompiler::emitControlFlowBreak(const GcnToken& token)
	{
		// implement break and continue
		auto       target  = token.getMatch();
		const bool isBreak = target->kind() != GcnTokenKind::Loop;

		GcnCfgBlock* cfgBlock = cfgFindBlock({ GcnCfgBlockType::Loop });
		if (cfgBlock == nullptr)
		{
			Logger::exception("GcnCompiler: 'Break' or 'Continue' outside 'Loop' or 'Switch' found");
		}

		m_module.opBranch(isBreak
							  ? cfgBlock->b_loop.labelBreak
							  : cfgBlock->b_loop.labelContinue);

		// Subsequent instructions assume that there is an open block
		const uint32_t labelId = m_module.allocateId();
		m_module.opLabel(labelId);
	}

	void GcnCompiler::emitControlFlowReturn()
	{
		if (m_controlFlowStack.size() != 0)
		{
			// Discard exp instruction in gcn shader
			// will follow a s_endpgm instruction.
			// We don't need OpReturn in such case.
			if (m_blockTerminators.empty())
			{
				uint32_t labelId = m_module.allocateId();

				m_module.opReturn();
				m_module.opLabel(labelId);

				m_blockTerminators.push_back(labelId);
			}
		}
		else
		{
			// Last instruction in the current function
			this->emitFunctionEnd();
		}
	}

	void GcnCompiler::emitControlFlowDiscard()
	{
		if (m_controlFlowStack.size() != 0)
		{
			uint32_t labelId = m_module.allocateId();

			m_module.opKill();
			m_module.opLabel(labelId);

			m_blockTerminators.push_back(labelId);
		}
		else
		{
			// Last instruction in the current function
			this->emitFunctionEnd();
		}
	}

	GcnCfgBlock* GcnCompiler::cfgFindBlock(
		const std::initializer_list<GcnCfgBlockType>& types)
	{
		for (auto cur = m_controlFlowStack.rbegin();
			 cur != m_controlFlowStack.rend(); cur++)
		{
			for (auto type : types)
			{
				if (cur->type == type)
					return &(*cur);
			}
		}

		return nullptr;
	}

	bool GcnCompiler::needBreakLoop(const GcnToken& token)
	{
		bool need = false;

		auto isSinkToken = [](const GcnToken& token)
		{
			if (token.kind() == GcnTokenKind::Code)
			{
				const auto& code     = token.getCode();
				const auto& lastInst = code.insList.back();
				return lastInst.opcode == GcnOpcode::S_ENDPGM;
			}
			return false;
		};

		auto match = token.getMatch();
		if (match->kind() == GcnTokenKind::Loop)
		{
			auto prev = token.getPrevNode();
			if (prev->kind() != GcnTokenKind::Branch && !isSinkToken(token))
			{
				need = true;
			}
		}

		return need;
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