#include "GcnCompiler.h"

LOG_CHANNEL(Graphic.Gcn.GcnCompiler);

namespace sce::gcn
{
	void GcnCompiler::emitScalarALU(const GcnShaderInstruction& ins)
	{
		auto opClass = ins.opClass;
		switch (opClass)
		{
			case GcnInstClass::ScalarArith:
			case GcnInstClass::ScalarMov:
			case GcnInstClass::ScalarBitLogic:
			case GcnInstClass::ScalarBitManip:
			case GcnInstClass::ScalarBitField:
				this->emitScalarAluCommon(ins);
				break;
			case GcnInstClass::ScalarMovRel:
				this->emitScalarMovRel(ins);
				break;
			case GcnInstClass::ScalarAbs:
				this->emitScalarAbs(ins);
				break;
			case GcnInstClass::ScalarCmp:
				this->emitScalarCmp(ins);
				break;
			case GcnInstClass::ScalarSelect:
				this->emitScalarSelect(ins);
				break;
			case GcnInstClass::ScalarConv:
				this->emitScalarConv(ins);
				break;
			case GcnInstClass::ScalarExecMask:
				this->emitScalarExecMask(ins);
				break;
			case GcnInstClass::ScalarQuadMask:
				this->emitScalarQuadMask(ins);
				break;
		}
	}

	void GcnCompiler::emitScalarAluCommon(const GcnShaderInstruction& ins)
	{
		std::array<GcnRegisterValuePair, GcnMaxOperandCount> src;
		for (uint32_t i = 0; i != ins.srcCount; ++i)
		{
			src[i] = emitRegisterLoad(ins.src[i]);
		}

		GcnRegisterValuePair dst = {};
		dst.low.type.ctype       = getDestinationType(ins.dst[0].type);
		dst.low.type.ccount      = 1;
		dst.high.type            = dst.low.type;

		bool           ignoreScc = false;
		const uint32_t typeId    = getVectorTypeId(dst.low.type);

		auto op = ins.opcode;
		switch (op)
		{
			// ScalarArith
			case GcnOpcode::S_ADD_U32:
			case GcnOpcode::S_ADD_I32:
			{
				dst.low.id = m_module.opIAdd(typeId,
											 src[0].low.id,
											 src[1].low.id);
				ignoreScc  = true;
			}
				break;
			case GcnOpcode::S_MUL_I32:
			{
				dst.low.id = m_module.opIMul(typeId,
											 src[0].low.id,
											 src[1].low.id);
				ignoreScc  = true;
			}
				break;
			// ScalarMov
			case GcnOpcode::S_MOV_B32:
			{
				dst.low.id = src[0].low.id;
				ignoreScc  = true;
			}
				break;
			case GcnOpcode::S_MOV_B64:
			{
				// Fix the type from Uint64 to Uint32 at the same time.
				dst.low  = src[0].low;
				dst.high = src[0].high;
				ignoreScc = true;
			}
				break;
			case GcnOpcode::S_MOVK_I32:
			{
				dst.low.id = m_module.consti32(ins.control.sopk.simm);
				ignoreScc  = true;
			}
				break;
			// ScalarBitLogic
			case GcnOpcode::S_AND_B32:
				dst.low.id = m_module.opBitwiseAnd(typeId,
													src[0].low.id,
													src[1].low.id);
				break;
			case GcnOpcode::S_AND_B64:
			{
				dst.low.id  = m_module.opBitwiseAnd(typeId, src[0].low.id, src[1].low.id);
				dst.high.id = m_module.opBitwiseAnd(typeId, src[0].high.id, src[1].high.id);
			}
				break;
			case GcnOpcode::S_OR_B64:
			{
				dst.low.id  = m_module.opBitwiseOr(typeId, src[0].low.id, src[1].low.id);
				dst.high.id = m_module.opBitwiseOr(typeId, src[0].high.id, src[1].high.id);
			}
				break;
			case GcnOpcode::S_NOR_B64:
			{
				dst.low.id  = m_module.opNot(typeId,
											 m_module.opBitwiseOr(typeId, src[0].low.id, src[1].low.id));
				dst.high.id = m_module.opNot(typeId,
											 m_module.opBitwiseOr(typeId, src[0].high.id, src[1].high.id));
			}
				break;
			case GcnOpcode::S_ANDN2_B64:
			{
				dst.low.id  = m_module.opBitwiseAnd(typeId,
													src[0].low.id,
													m_module.opNot(typeId, src[1].low.id));
				dst.high.id = m_module.opBitwiseAnd(typeId,
													src[0].high.id,
													m_module.opNot(typeId, src[1].high.id));
			}
				break;
			case GcnOpcode::S_ORN2_B64:
			{
				dst.low.id  = m_module.opBitwiseOr(typeId,
													src[0].low.id,
													m_module.opNot(typeId, src[1].low.id));
				dst.high.id = m_module.opBitwiseOr(typeId,
													src[0].high.id,
													m_module.opNot(typeId, src[1].high.id));
			}
				break;
			// ScalarBitManip
			case GcnOpcode::S_LSHL_B32:
			{
				uint32_t shift = m_module.opBitFieldUExtract(typeId,
															 src[1].low.id,
															 m_module.constu32(0),
															 m_module.constu32(5));
				dst.low.id     = m_module.opShiftLeftLogical(typeId,
															 src[0].low.id,
															 shift);
			}
				break;
			case GcnOpcode::S_LSHR_B32:
			{
				uint32_t shift = m_module.opBitFieldUExtract(typeId,
																src[1].low.id,
																m_module.constu32(0),
																m_module.constu32(5));
				dst.low.id     = m_module.opShiftRightLogical(typeId,
															  src[0].low.id,
															  shift);
			}
				break;
			// ScalarBitField
			case GcnOpcode::S_BFE_U32:
			{
				uint32_t offset = m_module.opBitFieldUExtract(typeId,
															  src[1].low.id,
															  m_module.constu32(0),
															  m_module.constu32(5));
				uint32_t width  = m_module.opBitFieldUExtract(typeId,
															  src[1].low.id,
															  m_module.constu32(16),
															  m_module.constu32(7));
				uint32_t field  = m_module.opShiftRightLogical(typeId,
															   src[0].low.id,
															   offset);
				uint32_t mask   = m_module.opISub(typeId,
												  m_module.opShiftLeftLogical(typeId,
																			  m_module.constu32(1),
																			  width),
												  m_module.constu32(1));
				dst.low.id      = m_module.opBitwiseAnd(typeId, field, mask);
			}
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}

		if (!ignoreScc)
		{
			emitUpdateScc(dst, ins.dst[0].type);
		}

		emitRegisterStore(ins.dst[0], dst);
	}

	void GcnCompiler::emitScalarMovRel(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitScalarArith(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitScalarAbs(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitScalarMov(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitScalarCmp(const GcnShaderInstruction& ins)
	{
		std::array<GcnRegisterValuePair, 2> src;

		if (ins.encoding == GcnInstEncoding::SOPC || ins.encoding == GcnInstEncoding::SOP2)
		{
			src[0] = emitRegisterLoad(ins.src[0]);
			src[1] = emitRegisterLoad(ins.src[1]);
		}
		else
		{
			// Deal with SOPK
			src[0]     = emitRegisterLoad(ins.dst[0]);
			src[1].low = emitBuildConstValue(ins.control.sopk.simm, ins.dst[0].type);
		}

		const uint32_t typeId = m_module.defBoolType();

		GcnRegisterValuePair dst = {};
		dst.low.type.ctype       = getDestinationType(ins.dst[0].type);
		dst.low.type.ccount      = 1;
		dst.high.type            = dst.low.type;

		uint32_t condition = 0;

		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::S_CMP_EQ_I32:
				condition = m_module.opIEqual(typeId,
											  src[0].low.id,
											  src[1].low.id);
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}

		// Update SCC
		m_module.opStore(m_state.scc.id, condition);

		// SOP2 encoding requires us to save dst
		if (ins.encoding == GcnInstEncoding::SOP2)
		{
			emitRegisterStore(ins.dst[0], dst);
		}
	}

	void GcnCompiler::emitScalarSelect(const GcnShaderInstruction& ins)
	{
		const std::array<GcnRegisterValuePair, 2> src = {
			emitRegisterLoad(ins.src[0]),
			emitRegisterLoad(ins.src[1]),
		};

		GcnRegisterValuePair dst = {};
		dst.low.type.ctype       = getDestinationType(ins.dst[0].type);
		dst.low.type.ccount      = 1;
		dst.high.type            = dst.low.type;

		const uint32_t typeId = getVectorTypeId(dst.low.type);

		uint32_t condition = m_module.opLoad(m_module.defBoolType(),
											 m_state.scc.id);

		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::S_CSELECT_B32:
				dst.low.id = m_module.opSelect(typeId,
											   condition,
											   src[0].low.id,
											   src[1].low.id);
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}

		emitRegisterStore(ins.dst[0], dst);
	}

	void GcnCompiler::emitScalarBitLogic(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitScalarBitManip(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitScalarBitField(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitScalarConv(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitScalarExecMask(const GcnShaderInstruction& ins)
	{
		auto src = emitRegisterLoad(ins.src[0]);

		// Save exec first
		auto exec = m_state.exec.emitLoad(GcnRegMask::firstN(2));
		emitRegisterStore(ins.dst[0], exec);

		GcnRegisterValuePair result = {};
		result.low.type             = exec.low.type;
		result.high.type            = exec.high.type;

		const uint32_t typeId = getVectorTypeId(result.low.type);

		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::S_AND_SAVEEXEC_B64:
			{
				result.low.id  = m_module.opBitwiseAnd(typeId,
													   src.low.id,
													   exec.low.id);
				result.high.id = m_module.opBitwiseAnd(typeId,
													   src.high.id,
													   exec.high.id);
			}
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}

		m_state.exec.emitStore(result, GcnRegMask::firstN(2));
	}

	void GcnCompiler::emitScalarQuadMask(const GcnShaderInstruction& ins)
	{
		auto src = emitRegisterLoad(ins.src[0]);

		GcnRegisterValuePair dst = {};
		dst.low.type.ctype       = getDestinationType(ins.dst[0].type);
		dst.low.type.ccount      = 1;
		dst.high.type            = dst.low.type;

		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::S_WQM_B64:
				dst.high = emitWholeQuadMode(src.high);
				[[fallthrough]];
			case GcnOpcode::S_WQM_B32:
				dst.low = emitWholeQuadMode(src.low);
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}

		emitRegisterStore(ins.dst[0], dst);
	}

	GcnRegisterValue GcnCompiler::emitWholeQuadMode(GcnRegisterValue src)
	{
		// Whole Quad Mode, 32 Bit
		// set mask for all 4 threads in each quad which has any threads active
		// for (q=0; q<8; q++)
		//     sdst[q*4+3:q*4].u = (ssrc[4*q+3:4*q].u != 0) ? 0xF : 0
		//
		// It seems we don't have an identical spirv instruction for gcn wqm.
		//
		// GLSL:
		// uint mask = xxx;
		// uint value = 0;
		// for(uint i = 0; i != 8; ++i)
		// {
		//     value |= ( (((mask >> i * 4) & 0xF) != 0 ? 0xFu : 0u) << (i * 4) );
		// }
		// mask = value;

		const uint32_t typeId = getScalarTypeId(GcnScalarType::Uint32);

		uint32_t mask = src.id;
		// Declare a temp value and initialize to zero.
		GcnRegisterInfo info;
		info.type.ctype   = GcnScalarType::Uint32;
		info.type.ccount  = 1;
		info.type.alength = 0;
		info.sclass       = spv::StorageClassPrivate;
		// Declare a temp value and initialize to zero.
		uint32_t value = emitNewVariable(info);
		m_module.opStore(value, m_module.constu32(0));
		// Declare i and initialize to zero.
		uint32_t i = emitNewVariable(info);
		m_module.opStore(i, m_module.constu32(0));

		uint32_t labelBegin    = m_module.allocateId();
		uint32_t labelContinue = m_module.allocateId();
		uint32_t labelEnd      = m_module.allocateId();

		m_module.opBranch(labelBegin);
		m_module.opLabel(labelBegin);

		m_module.opLoopMerge(labelEnd, labelContinue, spv::LoopControlMaskNone);
		uint32_t labelLoop = m_module.allocateId();
		m_module.opBranch(labelLoop);
		m_module.opLabel(labelLoop);

		uint32_t iValue    = m_module.opLoad(typeId, i);
		uint32_t condition = m_module.opINotEqual(m_module.defBoolType(), iValue, m_module.constu32(8));

		uint32_t labelDoWork = m_module.allocateId();
		m_module.opBranchConditional(condition, labelDoWork, labelEnd);
		m_module.opLabel(labelDoWork);

		iValue              = m_module.opLoad(typeId, i);
		uint32_t shift      = m_module.opIMul(typeId, iValue, m_module.constu32(4));
		uint32_t threadMask = m_module.opShiftRightLogical(typeId, mask, shift);
		threadMask          = m_module.opBitwiseAnd(typeId, threadMask, m_module.constu32(0xF));

		condition     = m_module.opINotEqual(m_module.defBoolType(), threadMask, m_module.constu32(0));
		uint32_t quad = m_module.opSelect(typeId, condition, m_module.constu32(0xF), m_module.constu32(0));

		iValue             = m_module.opLoad(typeId, i);
		shift              = m_module.opIMul(typeId, iValue, m_module.constu32(4));
		uint32_t shiftQuad = m_module.opShiftLeftLogical(typeId, quad, shift);
		uint32_t oldValue  = m_module.opLoad(typeId, value);
		uint32_t newValue  = m_module.opBitwiseOr(typeId, oldValue, shiftQuad);
		m_module.opStore(value, newValue);

		m_module.opBranch(labelContinue);
		m_module.opLabel(labelContinue);
		iValue = m_module.opLoad(typeId, i);
		iValue = m_module.opIAdd(typeId, iValue, m_module.constu32(1));
		m_module.opStore(i, iValue);
		m_module.opBranch(labelBegin);

		m_module.opLabel(labelEnd);

		GcnRegisterValue result;
		result.type = src.type;
		result.id   = m_module.opLoad(typeId, value);
		return result;
	}
}  // namespace sce::gcn
