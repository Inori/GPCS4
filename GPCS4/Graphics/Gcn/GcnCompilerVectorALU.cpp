#include "GcnCompiler.h"
#include "GcnHeader.h"

LOG_CHANNEL(Graphic.Gcn.GcnCompiler);

namespace sce::gcn
{
    void GcnCompiler::emitVectorALU(const GcnShaderInstruction& ins)
    {
        auto opClass = ins.opClass;
        switch (opClass)
        {
            // Many classes of vector ALU instructions can be translated
            // in the same flow. We place them all in the common method.
            case GcnInstClass::VectorRegMov:
            case GcnInstClass::VectorBitLogic:
            case GcnInstClass::VectorConv:
            case GcnInstClass::VectorFpArith32:
			case GcnInstClass::VectorIntArith32:
			case GcnInstClass::VectorIntArith64:
			case GcnInstClass::VectorFpTran32:
			case GcnInstClass::VectorThreadMask:
			case GcnInstClass::VectorBitField32:
			case GcnInstClass::VectorIntGraph:
			case GcnInstClass::VectorFpRound32:
                this->emitVectorAluCommon(ins);
                break;
			case GcnInstClass::VectorMovRel:
				this->emitVectorMovRel(ins);
				break;
			case GcnInstClass::VectorFpCmp64:
			case GcnInstClass::VectorFpCmp32:
			case GcnInstClass::VectorIntCmp32:
			case GcnInstClass::VectorIntCmp64:
				this->emitVectorCmp(ins);
				break;
            case GcnInstClass::VectorLane:
                this->emitVectorLane(ins);
                break;
            case GcnInstClass::VectorBitField64:
                this->emitVectorBitField64(ins);
                break;
            case GcnInstClass::VectorFpField32:
                this->emitVectorFpField32(ins);
                break;
            case GcnInstClass::VectorFpArith64:
                this->emitVectorFpArith64(ins);
                break;
            case GcnInstClass::VectorFpRound64:
                this->emitVectorFpRound64(ins);
                break;
            case GcnInstClass::VectorFpField64:
                this->emitVectorFpField64(ins);
                break;
            case GcnInstClass::VectorFpTran64:
                this->emitVectorFpTran64(ins);
                break;
            case GcnInstClass::VectorFpGraph32:
                this->emitVectorFpGraph32(ins);
                break;
            case GcnInstClass::VectorMisc:
                this->emitVectorMisc(ins);
                break;
        }
    }

    void GcnCompiler::emitVectorAluCommon(const GcnShaderInstruction& ins)
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

        const uint32_t typeId     = getVectorTypeId(dst.low.type);
		uint32_t       uintTypeId = getScalarTypeId(GcnScalarType::Uint32);
		uint32_t       sintTypeId = getScalarTypeId(GcnScalarType::Sint32);

        auto op = ins.opcode;
		switch (op)
		{
			// VectorBitLogic
			case GcnOpcode::V_AND_B32:
				dst.low.id = m_module.opBitwiseAnd(typeId,
												   src[0].low.id, src[1].low.id);
			    break;
			case GcnOpcode::V_OR_B32:
				dst.low.id = m_module.opBitwiseOr(typeId,
												  src[0].low.id, src[1].low.id);
				break;
			case GcnOpcode::V_XOR_B32:
				dst.low.id = m_module.opBitwiseXor(typeId,
												   src[0].low.id, src[1].low.id);
				break;
			// VectorConv
			case GcnOpcode::V_CVT_F32_U32:
				dst.low.id = m_module.opConvertUtoF(typeId,
													src[0].low.id);
			    break;
			case GcnOpcode::V_CVT_F32_I32:
				dst.low.id = m_module.opConvertStoF(typeId,
													src[0].low.id);
				break;
			case GcnOpcode::V_CVT_U32_F32:
				dst.low.id = m_module.opConvertFtoU(typeId,
													src[0].low.id);
				break;
			case GcnOpcode::V_CVT_I32_F32:
				dst.low.id = m_module.opConvertFtoS(typeId,
													src[0].low.id);
				break;
			case GcnOpcode::V_CVT_PKRTZ_F16_F32:
			{
				GcnRegisterValuePair pair;
				pair.low    = src[0].low;
				pair.high   = src[1].low;
                // Do we need to "Round Toward Zero" manually?
				auto packed = emitPackHalf2x16(pair);
				dst.low     = packed;
			}
				break;
			// VectorFpArith32
			case GcnOpcode::V_ADD_F32:
				dst.low.id = m_module.opFAdd(typeId,
											 src[0].low.id,
											 src[1].low.id);
				break;
			case GcnOpcode::V_MAD_F32:
				dst.low.id = m_module.opFAdd(typeId,
											 m_module.opFMul(typeId,
															 src[0].low.id,
															 src[1].low.id),
											 src[2].low.id);
				break;
			case GcnOpcode::V_FMA_F32:
				dst.low.id = m_module.opFFma(typeId,
											 src[0].low.id,
											 src[1].low.id,
											 src[2].low.id);
				break;
			case GcnOpcode::V_MUL_F32:
				dst.low.id = m_module.opFMul(typeId,
											 src[0].low.id,
											 src[1].low.id);
			    break;
			case GcnOpcode::V_MAC_F32:
			{
				auto vdst  = emitVgprLoad(ins.dst[0]);
				dst.low.id = m_module.opFAdd(typeId,
											 m_module.opFMul(typeId,
															 src[0].low.id,
															 src[1].low.id),
											 vdst.id);
			}
				break;
			case GcnOpcode::V_SUB_F32:
				dst.low.id = m_module.opFSub(typeId,
											 src[0].low.id,
											 src[1].low.id);
                break;
			case GcnOpcode::V_SUBREV_F32:
				dst.low.id = m_module.opFSub(typeId,
											 src[1].low.id,
											 src[0].low.id);
				break;
			case GcnOpcode::V_SUB_I32:
				dst.low.id = m_module.opISub(typeId,
											 src[0].low.id,
											 src[1].low.id);
				break;
			case GcnOpcode::V_SUBREV_I32:
				dst.low.id = m_module.opISub(typeId,
											 src[1].low.id,
											 src[0].low.id);
				break;
			case GcnOpcode::V_MAX_F32:
				dst.low.id = m_module.opFMax(typeId,
											 src[0].low.id,
											 src[1].low.id);
				break;
			case GcnOpcode::V_MIN_F32:
				dst.low.id = m_module.opFMin(typeId,
											 src[0].low.id,
											 src[1].low.id);
				break;
			case GcnOpcode::V_MADMK_F32:
				dst.low.id = m_module.opFAdd(typeId,
											 m_module.opFMul(typeId,
															 src[0].low.id,
															 src[2].low.id),
											 src[1].low.id);
				break;
			case GcnOpcode::V_MADAK_F32:
				dst.low.id = m_module.opFAdd(typeId,
											 m_module.opFMul(typeId,
															 src[0].low.id,
															 src[1].low.id),
											 src[2].low.id);
				break;
			// VectorFpRound32
			case GcnOpcode::V_FLOOR_F32:
				dst.low.id = m_module.opFloor(typeId,
											  src[0].low.id);
				break;
			case GcnOpcode::V_FRACT_F32:
				dst.low.id = m_module.opFract(typeId,
											  src[0].low.id);
				break;
			// VectorRegMov
			case GcnOpcode::V_MOV_B32:
				dst.low.id = src[0].low.id;
			    break;
            // VectorIntArith32
			case GcnOpcode::V_ADD_I32:
				dst.low.id = m_module.opIAdd(typeId,
											 src[0].low.id,
											 src[1].low.id);
				break;
			case GcnOpcode::V_ADDC_U32:
			{
				if (m_moduleInfo.options.separateSubgroup)
				{
					auto mask    = emitCommonSystemValueLoad(GcnSystemValue::SubgroupEqMask, GcnRegMask::select(0));
					auto laneIdx = emitCommonSystemValueLoad(GcnSystemValue::SubgroupInvocationID, GcnRegMask::select(0));

					auto value = m_module.opBitwiseAnd(typeId, src[2].low.id, mask.id);
					auto carry = m_module.opShiftRightLogical(typeId, value, laneIdx.id);

					dst.low.id = m_module.opIAdd(typeId,
												 m_module.opIAdd(typeId,
																 src[0].low.id,
																 src[1].low.id),
												 carry);
					// TODO:
					// use OpIAddCarry and ballot op to save carry-out bits to sdst.
				}
				else
				{
					LOG_GCN_UNHANDLED_INST();
				}
			}
				break;
			case GcnOpcode::V_MAD_U32_U24:
			case GcnOpcode::V_MAD_I32_I24:
				dst.low.id = m_module.opIAdd(typeId,
											 m_module.opIMul(typeId,
															 src[0].low.id,
															 src[1].low.id),
											 src[2].low.id);
				break;
			case GcnOpcode::V_MUL_U32_U24:
			{
				uint32_t src0 = m_module.opBitFieldUExtract(typeId,
															src[0].low.id,
															m_module.constu32(0),
															m_module.constu32(24));
				uint32_t src1 = m_module.opBitFieldUExtract(typeId,
															src[1].low.id,
															m_module.constu32(0),
															m_module.constu32(24));
				dst.low.id    = m_module.opIMul(typeId, src0, src1);
			}
				break;
			case GcnOpcode::V_MUL_I32_I24:
			{
				uint32_t src0 = m_module.opBitFieldSExtract(typeId,
															src[0].low.id,
															m_module.constu32(0),
															m_module.constu32(24));
				uint32_t src1 = m_module.opBitFieldSExtract(typeId,
															src[1].low.id,
															m_module.constu32(0),
															m_module.constu32(24));
				dst.low.id    = m_module.opIMul(typeId, src0, src1);
			}
				break;
			case GcnOpcode::V_MUL_LO_I32:
			case GcnOpcode::V_MUL_LO_U32:
				dst.low.id = m_module.opIMul(typeId,
											 src[0].low.id,
											 src[1].low.id);
				break;
			case GcnOpcode::V_MUL_HI_I32:
			{
				std::array<uint32_t, 2> members    = { typeId, typeId };
				const uint32_t          resultType = m_module.defStructType(
							 members.size(), members.data());

				uint32_t mul         = m_module.opSMulExtended(resultType,
															   src[0].low.id,
															   src[1].low.id);
				uint32_t memberIndex = 1;
				dst.low.id           = m_module.opCompositeExtract(typeId, mul, 1, &memberIndex);
			}
				break;
			case GcnOpcode::V_MUL_HI_U32:
			{
				std::array<uint32_t, 2> members    = { typeId, typeId };
				const uint32_t          resultType = m_module.defStructType(
							 members.size(), members.data());

				uint32_t mul         = m_module.opUMulExtended(resultType,
															   src[0].low.id,
															   src[1].low.id);
				uint32_t memberIndex = 1;
				dst.low.id           = m_module.opCompositeExtract(typeId, mul, 1, &memberIndex);
			}
				break;
			case GcnOpcode::V_MAX3_F32:
			{
				uint32_t max01 = m_module.opFMax(typeId,
												 src[0].low.id,
												 src[1].low.id);
				dst.low.id     = m_module.opFMax(typeId,
												 max01,
												 src[2].low.id);
			}
				break;
			case GcnOpcode::V_MIN3_F32:
			{
				uint32_t min01 = m_module.opFMin(typeId,
												 src[0].low.id,
												 src[1].low.id);
				dst.low.id     = m_module.opFMin(typeId,
												 min01,
												 src[2].low.id);
			}
				break;
			//VectorIntArith64
			case GcnOpcode::V_MAD_U64_U32:
			{
				std::array<uint32_t, 2> members    = { typeId, typeId };
				const uint32_t          resultType = m_module.defStructType(
							 members.size(), members.data());

				uint32_t mul         = m_module.opUMulExtended(resultType,
															   src[0].low.id,
															   src[1].low.id);
				uint32_t memberIndex = 0;
				uint32_t low         = m_module.opCompositeExtract(typeId, mul, 1, &memberIndex);
				memberIndex          = 1;
				uint32_t high        = m_module.opCompositeExtract(typeId, mul, 1, &memberIndex);

				// add low part, this may produce carry out
				uint32_t add = m_module.opIAddCarry(resultType, low, src[2].low.id);
				// get low part result
				memberIndex    = 0;
				dst.low.id     = m_module.opCompositeExtract(typeId, add, 1, &memberIndex);
				// get carry
				memberIndex    = 1;
				uint32_t carry = m_module.opCompositeExtract(typeId, add, 1, &memberIndex);
				// for high part, we need to add all 3 former results
				dst.high.id = m_module.opIAdd(typeId,
											  m_module.opIAdd(typeId,
															  high,
															  src[2].high.id),
											  carry);
			}
				break;
            // VectorFpTran32
			case GcnOpcode::V_RSQ_F32:
				dst.low.id = m_module.opInverseSqrt(typeId,
													src[0].low.id);
				break;
			case GcnOpcode::V_SQRT_F32:
				dst.low.id = m_module.opSqrt(typeId,
											 src[0].low.id);
				break;
			case GcnOpcode::V_RCP_F32:
			case GcnOpcode::V_RCP_IFLAG_F32:
				dst.low.id = m_module.opFDiv(typeId,
											 m_module.constf32(1.0),
											 src[0].low.id);
				break;
			case GcnOpcode::V_LOG_F32:
				dst.low.id = m_module.opLog2(typeId,
											 src[0].low.id);
				break;
			case GcnOpcode::V_EXP_F32:
				dst.low.id = m_module.opExp2(typeId,
											 src[0].low.id);
				break;
			case GcnOpcode::V_SIN_F32:
			{
				uint32_t radians = m_module.opFMul(typeId,
												   src[0].low.id,
												   m_module.constf32(static_cast<float>(2.0 * GcnPi)));
				dst.low.id       = m_module.opSin(typeId,
												  radians);
			}
				break;
			case GcnOpcode::V_COS_F32:
			{
				uint32_t radians = m_module.opFMul(typeId,
												   src[0].low.id,
												   m_module.constf32(static_cast<float>(2.0 * GcnPi)));
				dst.low.id       = m_module.opCos(typeId,
												  radians);
			}
				break;	
            // VectorThreadMask
			case GcnOpcode::V_CNDMASK_B32:
			{
				auto smask  = ins.encoding == GcnInstEncoding::VOP3
								  ? src[2].low
								  : m_state.vcc.emitLoad(GcnRegMask::select(0)).low;
				auto eqMask = emitCommonSystemValueLoad(
					GcnSystemValue::SubgroupEqMask, GcnRegMask::select(0));

				auto mask = smask;
				mask.id   = m_module.opBitwiseAnd(
					  typeId, smask.id, eqMask.id);

				// Should we calculate the LSB of the mask?
				auto condition = emitRegisterZeroTest(mask, GcnZeroTest::TestNz);
				dst.low.id     = m_module.opSelect(typeId,
												   condition.id,
												   src[1].low.id,
												   src[0].low.id);
			}
				break;
			// VectorBitField32
			case GcnOpcode::V_LSHR_B32:
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
			case GcnOpcode::V_LSHRREV_B32:
			{
				uint32_t shift = m_module.opBitFieldUExtract(typeId,
															 src[0].low.id,
															 m_module.constu32(0),
															 m_module.constu32(5));
				dst.low.id     = m_module.opShiftRightLogical(typeId,
															  src[1].low.id,
															  shift);
			}
				break;
			case GcnOpcode::V_LSHL_B32:
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
			case GcnOpcode::V_LSHLREV_B32:
			{
				uint32_t shift = m_module.opBitFieldUExtract(typeId,
															 src[0].low.id,
															 m_module.constu32(0),
															 m_module.constu32(5));
				dst.low.id     = m_module.opShiftLeftLogical(typeId,
															 src[1].low.id,
															 shift);
			}
				break;
			case GcnOpcode::V_ASHR_I32:
			{
				uint32_t src1  = m_module.opBitcast(uintTypeId, src[1].low.id);
				uint32_t shift = m_module.opBitFieldUExtract(uintTypeId,
															 src1,
															 m_module.constu32(0),
															 m_module.constu32(5));
				dst.low.id     = m_module.opShiftRightArithmetic(typeId,
																 src[0].low.id,
																 shift);
			}
				break;
			case GcnOpcode::V_ASHRREV_I32:
			{
				uint32_t src0  = m_module.opBitcast(uintTypeId, src[0].low.id);
				uint32_t shift = m_module.opBitFieldUExtract(uintTypeId,
															 src0,
															 m_module.constu32(0),
															 m_module.constu32(5));
				dst.low.id     = m_module.opShiftRightArithmetic(typeId,
																 src[1].low.id,
																 shift);
			}
				break;
			case GcnOpcode::V_BFE_U32:
			{
				uint32_t shiftOffset = m_module.opBitFieldUExtract(typeId,
																   src[1].low.id,
																   m_module.constu32(0),
																   m_module.constu32(5));
				uint32_t shiftSize   = m_module.opBitFieldUExtract(typeId,
																   src[2].low.id,
																   m_module.constu32(0),
																   m_module.constu32(5));
				uint32_t field       = m_module.opShiftRightLogical(typeId,
																	src[0].low.id,
																	shiftOffset);
				uint32_t mask        = m_module.opISub(typeId,
													   m_module.opShiftLeftLogical(typeId,
																				   m_module.constu32(1),
																				   shiftSize),
													   m_module.constu32(1));
				dst.low.id           = m_module.opBitwiseAnd(typeId, field, mask);
			}
				break;
			//VectorIntGraph
			case GcnOpcode::V_SAD_U32:
			{
				// Sometimes this instruction relies on integer overflow.
				// For example, the following instruction is equal to v2 = (v2 -1)
				// v_sad_u32 v2, -1, 0, v2

				// Vega ISA:
				// ABSDIFF(x, y) := (x > y ? x - y : y - x) // UNSIGNED comparison
				// D.u = ABSDIFF(S0.u, S1.u) + S2.u.
				uint32_t isSrc0Greater = m_module.opUGreaterThan(m_module.defBoolType(),
																 src[0].low.id,
																 src[1].low.id);
				// We shouldn't use OpSAbs,
				// OpSAbs will interpret x as a signed integer first,
				// in the edge cases like if x = 0xFFFFFFFF,
				// the result will be 1, not 0xFFFFFFFF, which is our expected result.
				// So we detect which one is larger first and use the larger one minus the
				// smaller one.
				uint32_t diff = m_module.opSelect(typeId,
												  isSrc0Greater,
												  m_module.opISub(typeId,
																  src[0].low.id,
																  src[1].low.id),
												  m_module.opISub(typeId,
																  src[1].low.id,
																  src[0].low.id));
				dst.low.id    = m_module.opIAdd(typeId,
												diff, src[2].low.id);
			}
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}

        emitRegisterStore(ins.dst[0], dst);
    }

	void GcnCompiler::emitVectorMovRel(const GcnShaderInstruction& ins)
	{
		const uint32_t typeId = getScalarTypeId(ins.dst[0].type);

		GcnRegisterValue dst = {};
		dst.type.ctype       = ins.dst[0].type;
		dst.type.ccount      = 1;

		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::V_MOVRELD_B32:
			{
				auto     src   = emitRegisterLoad(ins.src[0]);
				auto     m0    = emitValueLoad(m_state.m0);
				auto     base  = m_module.constu32(ins.dst[0].code);
				uint32_t index = m_module.opIAdd(typeId, base, m0.id);

				dst.id = src.low.id;
				emitVgprStore(index, dst);
			}
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}
	}

	void GcnCompiler::emitVectorCmp(const GcnShaderInstruction& ins)
	{
		const std::array<GcnRegisterValuePair, 2> src = {
			emitRegisterLoad(ins.src[0]),
			emitRegisterLoad(ins.src[1]),
		};

		// Condition, which is a boolean vector used
		// to select between the ~0u and 0u vectors.
		uint32_t condition     = 0;
		uint32_t conditionType = m_module.defBoolType();

		// TODO:
		// unordered float support.

        bool updateExec = false;

		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::V_CMPX_EQ_U32:
				updateExec = true;
				[[fallthrough]];
			case GcnOpcode::V_CMP_EQ_U32:
			case GcnOpcode::V_CMP_EQ_I32:
				condition = m_module.opIEqual(conditionType,
											  src[0].low.id,
											  src[1].low.id);
				break;
			case GcnOpcode::V_CMP_GT_I32:
				condition = m_module.opSGreaterThan(conditionType,
													src[0].low.id,
													src[1].low.id);
				break;
			case GcnOpcode::V_CMPX_GT_U32:
				updateExec = true;
				[[fallthrough]];
			case GcnOpcode::V_CMP_GT_U32:
				condition = m_module.opUGreaterThan(conditionType,
													src[0].low.id,
													src[1].low.id);
				break;
			case GcnOpcode::V_CMPX_GE_U32:
				updateExec = true;
				[[fallthrough]];
			case GcnOpcode::V_CMP_GE_U32:
				condition = m_module.opUGreaterThanEqual(conditionType,
														 src[0].low.id,
														 src[1].low.id);
				break;
			case GcnOpcode::V_CMPX_NLE_F32:
				updateExec = true;
				[[fallthrough]];
			case GcnOpcode::V_CMP_GT_F32:
			case GcnOpcode::V_CMP_NLE_F32:
				condition = m_module.opFOrdGreaterThan(conditionType,
													   src[0].low.id,
													   src[1].low.id);
				break;
			case GcnOpcode::V_CMP_NGT_F32:
				condition = m_module.opFOrdLessThanEqual(conditionType,
														 src[0].low.id,
														 src[1].low.id);
				break;
			case GcnOpcode::V_CMP_GE_F32:
				condition = m_module.opFOrdGreaterThanEqual(conditionType,
															src[0].low.id,
															src[1].low.id);
			    break;
			case GcnOpcode::V_CMP_EQ_F32:
				condition = m_module.opFOrdEqual(conditionType,
												 src[0].low.id,
												 src[1].low.id);
				break;
			case GcnOpcode::V_CMP_NEQ_F32:
				condition = m_module.opFOrdNotEqual(conditionType,
													src[0].low.id,
													src[1].low.id);
				break;
			case GcnOpcode::V_CMP_LE_I32:
				condition = m_module.opSLessThanEqual(conditionType,
													  src[0].low.id,
													  src[1].low.id);
				break;
			case GcnOpcode::V_CMP_LE_U32:
				condition = m_module.opULessThanEqual(conditionType,
													  src[0].low.id,
													  src[1].low.id);
				break;
			case GcnOpcode::V_CMP_LE_F32:
				condition = m_module.opFOrdLessThanEqual(conditionType,
														 src[0].low.id,
														 src[1].low.id);
				break;
			case GcnOpcode::V_CMPX_LT_U32:
				updateExec = true;
				[[fallthrough]];
			case GcnOpcode::V_CMP_LT_U32:
				condition = m_module.opULessThan(conditionType,
												 src[0].low.id,
												 src[1].low.id);
				break;
			case GcnOpcode::V_CMPX_LT_I32:
				updateExec = true;
				[[fallthrough]];
			case GcnOpcode::V_CMP_LT_I32:
				condition = m_module.opSLessThan(conditionType,
												 src[0].low.id,
												 src[1].low.id);
				break;
			case GcnOpcode::V_CMP_LT_F32:
				condition = m_module.opFOrdLessThan(conditionType,
													src[0].low.id,
													src[1].low.id);
				break;
			case GcnOpcode::V_CMPX_NE_U32:
				updateExec = true;
				[[fallthrough]];
			case GcnOpcode::V_CMP_NE_U32:
			case GcnOpcode::V_CMP_NE_I32:
				condition = m_module.opINotEqual(conditionType,
												 src[0].low.id,
												 src[1].low.id);
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}

		const uint32_t typeId = getScalarTypeId(GcnScalarType::Uint32);

		GcnRegisterValuePair result = {};
		result.low.type.ctype       = GcnScalarType::Uint32;
		result.low.type.ccount      = 1;
		result.high.type            = result.low.type;

		GcnRegisterValue ballot = {};
		ballot.type.ctype       = GcnScalarType::Uint32;
		ballot.type.ccount      = 4;
		ballot.id               = m_module.opGroupNonUniformBallot(
						  getVectorTypeId(ballot.type),
						  m_module.constu32(spv::ScopeSubgroup),
						  condition);

		if (m_moduleInfo.options.separateSubgroup)
		{
			auto exec     = m_state.exec.emitLoad(GcnRegMask::select(0));
			auto ballotX  = emitRegisterExtract(ballot, GcnRegMask::select(0));
			result.low.id = m_module.opBitwiseAnd(typeId, ballotX.id, exec.low.id);

			// Always set high 32-bits of the compare result to zero,
			// which means the high 32 lanes is inactive,
			// we then process high 32 lanes in next neighbor subgroup.
			result.high.id = m_module.constu32(0);
		}
		else
		{
			auto exec = m_state.exec.emitLoad(GcnRegMask::firstN(2));

			auto ballotX   = emitRegisterExtract(ballot, GcnRegMask::select(0));
			auto ballotY   = emitRegisterExtract(ballot, GcnRegMask::select(1));
			result.low.id  = m_module.opBitwiseAnd(typeId, ballotX.id, exec.low.id);
			result.high.id = m_module.opBitwiseAnd(typeId, ballotY.id, exec.high.id);
		}

        if (updateExec)
        {
			m_state.exec.emitStore(result, GcnRegMask::firstN(2));
        }

		emitRegisterStore(ins.dst[1], result);
	}

    void GcnCompiler::emitVectorRegMov(const GcnShaderInstruction& ins)
    {
        LOG_GCN_UNHANDLED_INST();
    }

    void GcnCompiler::emitVectorLane(const GcnShaderInstruction& ins)
    {
		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::V_READFIRSTLANE_B32:
				this->emitLaneReadFirst(ins);
				break;
			case GcnOpcode::V_READLANE_B32:
				this->emitLaneRead(ins);
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}
    }

    void GcnCompiler::emitVectorBitLogic(const GcnShaderInstruction& ins)
    {
        LOG_GCN_UNHANDLED_INST();
    }

    void GcnCompiler::emitVectorBitField32(const GcnShaderInstruction& ins)
    {
        LOG_GCN_UNHANDLED_INST();
    }

    void GcnCompiler::emitVectorThreadMask(const GcnShaderInstruction& ins)
    {
        LOG_GCN_UNHANDLED_INST();
    }

    void GcnCompiler::emitVectorBitField64(const GcnShaderInstruction& ins)
    {
        LOG_GCN_UNHANDLED_INST();
    }

    void GcnCompiler::emitVectorFpArith32(const GcnShaderInstruction& ins)
    {
        LOG_GCN_UNHANDLED_INST();
    }

    void GcnCompiler::emitVectorFpRound32(const GcnShaderInstruction& ins)
    {
        LOG_GCN_UNHANDLED_INST();
    }

    void GcnCompiler::emitVectorFpField32(const GcnShaderInstruction& ins)
    {
        LOG_GCN_UNHANDLED_INST();
    }

    void GcnCompiler::emitVectorFpTran32(const GcnShaderInstruction& ins)
    {
        LOG_GCN_UNHANDLED_INST();
    }

    void GcnCompiler::emitVectorFpCmp32(const GcnShaderInstruction& ins)
    {
        LOG_GCN_UNHANDLED_INST();
    }

    void GcnCompiler::emitVectorFpArith64(const GcnShaderInstruction& ins)
    {
        LOG_GCN_UNHANDLED_INST();
    }

    void GcnCompiler::emitVectorFpRound64(const GcnShaderInstruction& ins)
    {
        LOG_GCN_UNHANDLED_INST();
    }

    void GcnCompiler::emitVectorFpField64(const GcnShaderInstruction& ins)
    {
        LOG_GCN_UNHANDLED_INST();
    }

    void GcnCompiler::emitVectorFpTran64(const GcnShaderInstruction& ins)
    {
        LOG_GCN_UNHANDLED_INST();
    }

    void GcnCompiler::emitVectorFpCmp64(const GcnShaderInstruction& ins)
    {
        LOG_GCN_UNHANDLED_INST();
    }

    void GcnCompiler::emitVectorIntArith32(const GcnShaderInstruction& ins)
    {
        LOG_GCN_UNHANDLED_INST();
    }

    void GcnCompiler::emitVectorIntArith64(const GcnShaderInstruction& ins)
    {
        LOG_GCN_UNHANDLED_INST();
    }

    void GcnCompiler::emitVectorIntCmp32(const GcnShaderInstruction& ins)
    {
        LOG_GCN_UNHANDLED_INST();
    }

    void GcnCompiler::emitVectorIntCmp64(const GcnShaderInstruction& ins)
    {
        LOG_GCN_UNHANDLED_INST();
    }

    void GcnCompiler::emitVectorConv(const GcnShaderInstruction& ins)
    {
        LOG_GCN_UNHANDLED_INST();
    }

    void GcnCompiler::emitVectorFpGraph32(const GcnShaderInstruction& ins)
    {
		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::V_CUBEID_F32:
			case GcnOpcode::V_CUBESC_F32:
			case GcnOpcode::V_CUBETC_F32:
			case GcnOpcode::V_CUBEMA_F32:
				emitCubeCalculate(ins);
                break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}
    }

	void GcnCompiler::emitLaneReadFirst(const GcnShaderInstruction& ins)
	{
		auto src = emitRegisterLoad(ins.src[0]);

		GcnRegisterValuePair dst = {};
		dst.low.type.ctype       = getDestinationType(ins.dst[0].type);
		dst.low.type.ccount      = 1;
		dst.high.type            = dst.low.type;

		const uint32_t typeId = getVectorTypeId(dst.low.type);

		// TODO:
		// there may be problems if we only broadcast the value
		// to 32 lanes, to perfectly implement this instruction,
		// we may need to use share memory to broadcast to all
		// 64 lanes.
		dst.low.id = m_module.opGroupNonUniformBroadcastFirst(typeId,
															  m_module.constu32(spv::ScopeSubgroup),
															  src.low.id);
		emitRegisterStore(ins.dst[1], dst);
	}

	GcnRegisterValue GcnCompiler::emitCsLaneRead(const GcnRegisterValue& slane,
												 const GcnRegisterValue& src)
	{
		// For compute shader, we need to broadcast the value
		// across all 64 lanes manually by share memory if we need to separate subgroup.
		// Means for gl_SubgroupID which is even number, we need to broadcast
		// the value to its' neighbor odd number subgroup, and vice versa.

		const uint32_t utypeId = getScalarTypeId(GcnScalarType::Uint32);
		const uint32_t btypeId = m_module.defBoolType();

		auto subgroupId = emitCsSystemValueLoad(
			GcnSystemValue::SubgroupID, GcnRegMask::select(0));

		// detect if slane is less than 32, which means
		// it's in low 32 lanes
		uint32_t isLowGroup = m_module.opULessThan(btypeId,
												   slane.id,
												   m_module.constu32(32));
		// detect if current subgroup is even or odd
		uint32_t indexMod = m_module.opUMod(
			utypeId, subgroupId.id, m_module.constu32(2));
		uint32_t isEvenGroup = m_module.opIEqual(
			btypeId, indexMod, m_module.constu32(0));

		uint32_t isLowAndEven = m_module.opLogicalAnd(btypeId, isLowGroup, isEvenGroup);
		uint32_t isHighAndOdd = m_module.opLogicalAnd(btypeId,
													  m_module.opLogicalNot(btypeId, isLowGroup),
													  m_module.opLogicalNot(btypeId, isEvenGroup));

		// subtract 32 if slane is in high 32 lanes
		uint32_t laneIndex = m_module.opSelect(utypeId, isLowGroup,
											   slane.id,
											   m_module.opISub(utypeId,
															   slane.id,
															   m_module.constu32(32)));

		uint32_t needLoad = m_module.opLogicalOr(btypeId, isLowAndEven, isHighAndOdd);
		// Elect one lane to perform the store operation
		uint32_t election = m_module.opGroupNonUniformElect(
			btypeId,
			m_module.constu32(spv::ScopeSubgroup));

		uint32_t condition = m_module.opLogicalAnd(btypeId,
												   needLoad,
												   election);

		uint32_t labelBegin = m_module.allocateId();
		uint32_t labelEnd   = m_module.allocateId();
		m_module.opSelectionMerge(labelEnd, spv::SelectionControlMaskNone);
		m_module.opBranchConditional(condition, labelBegin, labelEnd);
		m_module.opLabel(labelBegin);

		uint32_t value = m_module.opGroupNonUniformShuffle(utypeId,
														   m_module.constu32(spv::ScopeSubgroup),
														   src.id,
														   laneIndex);
		// store broadcast value to self slot
		const uint32_t ptrType = m_module.defPointerType(utypeId,
														 spv::StorageClassWorkgroup);
		uint32_t       ptrSelf = m_module.opAccessChain(ptrType,
														m_cs.crossGroupMemoryId,
														1,
														&subgroupId.id);
		m_module.opStore(ptrSelf, value);

		// store broadcast value to neighbor slot
		uint32_t indexNeighbor = m_module.opSelect(utypeId, isEvenGroup,
												   m_module.opIAdd(utypeId, subgroupId.id, m_module.constu32(1)),
												   m_module.opISub(utypeId, subgroupId.id, m_module.constu32(1)));
		uint32_t ptrNeighbor   = m_module.opAccessChain(ptrType,
														m_cs.crossGroupMemoryId,
														1,
														&indexNeighbor);
		m_module.opStore(ptrNeighbor, value);

		m_module.opBranch(labelEnd);
		m_module.opLabel(labelEnd);

		// wait for store finish for all lanes
		m_module.opControlBarrier(m_module.constu32(spv::ScopeWorkgroup),
								  m_module.constu32(spv::ScopeWorkgroup),
								  m_module.constu32(spv::MemorySemanticsWorkgroupMemoryMask |
													spv::MemorySemanticsAcquireReleaseMask));

		// now we can load the broadcast value from cross group share memory
		uint32_t ptr = m_module.opAccessChain(ptrType,
											  m_cs.crossGroupMemoryId,
											  1,
											  &subgroupId.id);

		GcnRegisterValue result;
		result.type.ctype  = GcnScalarType::Uint32;
		result.type.ccount = 1;
		result.id          = m_module.opLoad(utypeId, ptr);
		return result;
	}

	void GcnCompiler::emitLaneRead(const GcnShaderInstruction& ins)
	{
		m_module.enableCapability(spv::CapabilityGroupNonUniformShuffle);

		std::array<GcnRegisterValuePair, GcnMaxOperandCount> src;
		for (uint32_t i = 0; i != ins.srcCount; ++i)
		{
			src[i] = emitRegisterLoad(ins.src[i]);
		}

		GcnRegisterValuePair dst = {};
		dst.low.type.ctype       = getDestinationType(ins.dst[0].type);
		dst.low.type.ccount      = 1;
		dst.high.type            = dst.low.type;

		const uint32_t utypeId = getVectorTypeId(dst.low.type);

		if (m_programInfo.type() == GcnProgramType::ComputeShader &&
			m_moduleInfo.options.separateSubgroup)
		{
			auto value = emitCsLaneRead(src[1].low, src[0].low);
			dst.low.id = value.id;
		}
		else
		{
			dst.low.id = m_module.opGroupNonUniformShuffle(utypeId,
														   m_module.constu32(spv::ScopeSubgroup),
														   src[0].low.id,
														   src[1].low.id);
		}

		emitRegisterStore(ins.dst[1], dst);
	}

	void GcnCompiler::emitCubeCalculate(const GcnShaderInstruction& ins)
	{
		// For sampling a cube map,
		// image_sample instruction use (x, y, face_id) as the coordinate,
		// where x is calculated from s coordinate, y is calculated from t coordinate,
		// and where s, t are calculated from the original direction vector. 
		// See ISA manual for calculation details.
		// This is different from vulkan (spir-v) which uses a direction vector (x, y, z).
		// 
		// Here we don't care the calculation details,
		// since both v_cubexxx and image_sample instructions are under our control,
		// we simply stores the original coordinate to the target vgpr.
		// and when translating image_sample instruction, we just use the values stored here.
		// (with a little fix however).


		std::array<GcnRegisterValuePair, GcnMaxOperandCount> src;
		for (uint32_t i = 0; i != ins.srcCount; ++i)
		{
			src[i] = emitRegisterLoad(ins.src[i]);
		}

		GcnRegisterValuePair dst = {};
		dst.low.type.ctype       = getDestinationType(ins.dst[0].type);
		dst.low.type.ccount      = 1;
		dst.high.type            = dst.low.type;

		const uint32_t typeId     = getVectorTypeId(dst.low.type);
		const uint32_t boolTypeId = m_module.defBoolType();
		const uint32_t intTypeId  = getScalarTypeId(GcnScalarType::Sint32);
		auto           op         = ins.opcode;

		uint32_t x = src[0].low.id;
		uint32_t y = src[1].low.id;
		uint32_t z = src[2].low.id;

		switch (op)
		{
			case GcnOpcode::V_CUBEID_F32:
				// Stores z to face_id
				dst.low.id = z;
				break;
			case GcnOpcode::V_CUBESC_F32:
				// Stores x to s coordinate
				dst.low.id = x;
				break;
			case GcnOpcode::V_CUBETC_F32:
				// Stores y to t coordinate
				dst.low.id = y;
				break;
			case GcnOpcode::V_CUBEMA_F32:
				// This is used to calculate the largest magnitude coordinate 
				// of the direction vector, and the result will then be 
				// used to scale the s and t coordinate.
				// We simply returns 1.0 to disable scale.
				dst.low.id = m_module.constf32(1.0);
				break;
		}

		emitRegisterStore(ins.dst[0], dst);
	}

/*
    void GcnCompiler::emitCubeCalculate(const GcnShaderInstruction& ins)
	{
		// TODO:
		// Wrap cube calculate into a function

		//		float calcCube(float x, float y, float z)
		//		{
		//			float absX = abs(x);
		//			float absY = abs(y);
		//			float absZ = abs(z);
		//
		//			float faceId = 0.0;
		//			if (absZ >= absX && absZ >= absY)
		//				faceId = z < 0.0 ? 5 : 4;
		//			else if (absY >= absX)
		//				faceId = y < 0.0 ? 3 : 2;
		//			else
		//				faceId = x < 0.0 ? 1 : 0;
		//			return faceId;
		//		}

		std::array<GcnRegisterValuePair, GcnMaxOperandCount> src;
		for (uint32_t i = 0; i != ins.srcCount; ++i)
		{
			src[i] = emitRegisterLoad(ins.src[i]);
		}

		GcnRegisterValuePair dst = {};
		dst.low.type.ctype       = getDestinationType(ins.dst[0].type);
		dst.low.type.ccount      = 1;
		dst.high.type            = dst.low.type;

		const uint32_t typeId     = getVectorTypeId(dst.low.type);
		const uint32_t boolTypeId = m_module.defBoolType();
		const uint32_t intTypeId  = getScalarTypeId(GcnScalarType::Sint32);
		auto           op         = ins.opcode;

		GcnRegisterInfo info;
		info.type.ctype          = GcnScalarType::Float32;
		info.type.ccount         = 1;
		info.type.alength        = 0;
		info.sclass              = spv::StorageClassPrivate;
		const uint32_t resultVar = emitNewVariable(info);

        uint32_t x    = src[0].low.id;
		uint32_t y    = src[1].low.id;
		uint32_t z    = src[2].low.id;
        uint32_t absX = emitRegisterAbsolute(src[0].low).id;
		uint32_t absY = emitRegisterAbsolute(src[1].low).id;
		uint32_t absZ = emitRegisterAbsolute(src[2].low).id;

        // if (abs(vz.f) >= abs(vx.f) && abs(vz.f) >= abs(vy.f))
        uint32_t zxCondition = m_module.opFOrdGreaterThanEqual(boolTypeId,
															   absZ, absX);
		uint32_t zyCondition = m_module.opFOrdGreaterThanEqual(boolTypeId,
															   absZ, absY);
		uint32_t zxyCondition = m_module.opLogicalAnd(boolTypeId,
													  zxCondition, zyCondition);
        
        uint32_t endLabel = m_module.allocateId();
		m_module.opSelectionMerge(endLabel, spv::SelectionControlMaskNone);

		uint32_t zxyLabel  = m_module.allocateId();
		uint32_t yxIfLabel = m_module.allocateId();
		m_module.opBranchConditional(zxyCondition, zxyLabel, yxIfLabel);
        
        m_module.opLabel(zxyLabel);
		switch (op)
		{
			case GcnOpcode::V_CUBEID_F32:
			{
				uint32_t condition = m_module.opFOrdLessThan(boolTypeId,
															 z, m_module.constf32(0.0));
				uint32_t value     = m_module.opSelect(typeId,
													   condition,
													   m_module.constf32(5.0),
													   m_module.constf32(4.0));
				m_module.opStore(resultVar, value);
			}
                break;
			case GcnOpcode::V_CUBESC_F32:
			{
				uint32_t condition = m_module.opFOrdLessThan(boolTypeId,
															 z, m_module.constf32(0.0));
				uint32_t value     = m_module.opSelect(typeId,
													   condition,
													   m_module.opFNegate(typeId, x),
													   x);
				m_module.opStore(resultVar, value);
			}
                break;
			case GcnOpcode::V_CUBETC_F32:
			{
				uint32_t value = m_module.opFNegate(typeId, y);
				m_module.opStore(resultVar, value);
			}
                break;
			case GcnOpcode::V_CUBEMA_F32:
			{
				uint32_t value = m_module.opFMul(typeId,
										   m_module.constf32(2.0),
										   z);
				m_module.opStore(resultVar, value);
			}
                break;
		}
		m_module.opBranch(endLabel);

		// else if (abs(vy.f) >= abs(vx.f))
		m_module.opLabel(yxIfLabel);
		uint32_t yxCondition = m_module.opFOrdGreaterThanEqual(boolTypeId,
															   absY, absX);
		// A empty block right before end block used to build merge block
		uint32_t beforeEndLabel = m_module.allocateId();
		m_module.opSelectionMerge(beforeEndLabel, spv::SelectionControlMaskNone);

		uint32_t yxLabel = m_module.allocateId();
		uint32_t xLabel  = m_module.allocateId();
		m_module.opBranchConditional(yxCondition, yxLabel, xLabel);

		m_module.opLabel(yxLabel);
		switch (op)
		{
			case GcnOpcode::V_CUBEID_F32:
			{
				uint32_t condition = m_module.opFOrdLessThan(boolTypeId,
															 y, m_module.constf32(0.0));
				uint32_t value     = m_module.opSelect(typeId,
													   condition,
													   m_module.constf32(3.0),
													   m_module.constf32(2.0));
				m_module.opStore(resultVar, value);
			}
				break;
			case GcnOpcode::V_CUBESC_F32:
			{
				uint32_t value = x;
				m_module.opStore(resultVar, value);
			}
				break;
			case GcnOpcode::V_CUBETC_F32:
			{
				uint32_t condition = m_module.opFOrdLessThan(boolTypeId,
															 y, m_module.constf32(0.0));
				uint32_t value     = m_module.opSelect(typeId,
													   condition,
													   m_module.opFNegate(typeId, z),
													   z);
				m_module.opStore(resultVar, value);
			}
				break;
			case GcnOpcode::V_CUBEMA_F32:
			{
				uint32_t value = m_module.opFMul(typeId,
										   m_module.constf32(2.0),
										   y);
				m_module.opStore(resultVar, value);
			}
				break;
		}
		m_module.opBranch(beforeEndLabel);

		// else
		m_module.opLabel(xLabel);
		switch (op)
		{
			case GcnOpcode::V_CUBEID_F32:
			{
				uint32_t condition = m_module.opFOrdLessThan(boolTypeId,
															 x, m_module.constf32(0.0));
				uint32_t value     = m_module.opSelect(typeId,
													   condition,
													   m_module.constf32(1.0),
													   m_module.constf32(0.0));
				m_module.opStore(resultVar, value);
			}
				break;
			case GcnOpcode::V_CUBESC_F32:
			{
				uint32_t condition = m_module.opFOrdLessThan(boolTypeId,
															 x, m_module.constf32(0.0));
				uint32_t value     = m_module.opSelect(typeId,
													   condition,
													   z,
													   m_module.opFNegate(typeId, z));
				m_module.opStore(resultVar, value);
			}
				break;
			case GcnOpcode::V_CUBETC_F32:
			{
				uint32_t value = m_module.opFNegate(typeId, y);
				m_module.opStore(resultVar, value);
			}
				break;
			case GcnOpcode::V_CUBEMA_F32:
			{
				uint32_t value = m_module.opFMul(typeId,
												 m_module.constf32(2.0),
												 x);
				m_module.opStore(resultVar, value);
			}
				break;
		}
		m_module.opBranch(beforeEndLabel);

		// A empty block right before end block used to build merge block
		m_module.opLabel(beforeEndLabel);
		m_module.opBranch(endLabel);

		// End label
		m_module.opLabel(endLabel);

		dst.low.id = m_module.opLoad(typeId, resultVar);
        emitRegisterStore(ins.dst[0], dst);
	}
*/

    void GcnCompiler::emitVectorIntGraph(const GcnShaderInstruction& ins)
    {
        LOG_GCN_UNHANDLED_INST();
    }

    void GcnCompiler::emitVectorMisc(const GcnShaderInstruction& ins)
    {
        LOG_GCN_UNHANDLED_INST();
    }
}  // namespace sce::gcn