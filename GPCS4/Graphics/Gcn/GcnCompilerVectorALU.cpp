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
			case GcnInstClass::VectorFpTran32:
			case GcnInstClass::VectorThreadMask:
			case GcnInstClass::VectorBitField32:
			case GcnInstClass::VectorIntGraph:
                this->emitVectorAluCommon(ins);
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
            case GcnInstClass::VectorFpRound32:
                this->emitVectorFpRound32(ins);
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
            case GcnInstClass::VectorIntArith64:
                this->emitVectorIntArith64(ins);
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
			// VectorConv
			case GcnOpcode::V_CVT_F32_U32:
				dst.low.id = m_module.opConvertUtoF(typeId,
													src[0].low.id);
			    break;
			case GcnOpcode::V_CVT_U32_F32:
				dst.low.id = m_module.opConvertFtoU(typeId,
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
			case GcnOpcode::V_MUL_F32:
				dst.low.id = m_module.opFMul(typeId,
											 src[0].low.id,
											 src[1].low.id);
			    break;
			case GcnOpcode::V_MUL_LO_I32:
				dst.low.id = m_module.opIMul(typeId,
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
			case GcnOpcode::V_MAD_U32_U24:
			case GcnOpcode::V_MAD_I32_I24:
				dst.low.id = m_module.opIAdd(typeId,
											 m_module.opIMul(typeId,
															 src[0].low.id,
															 src[1].low.id),
											 src[2].low.id);
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
				dst.low.id = m_module.opFDiv(typeId,
											 m_module.constf32(1.0),
											 src[0].low.id);
				break;
            // VectorThreadMask
			case GcnOpcode::V_CNDMASK_B32:
			{
				auto smask = ins.encoding == GcnInstEncoding::VOP3 ? 
                    src[2].low : 
                    m_state.vcc.emitLoad(GcnRegMask::select(0)).low;

				// Should we calculate the LSB of the mask?
				auto condition = emitRegisterZeroTest(smask, GcnZeroTest::TestNz);
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
				
				uint32_t sub        = m_module.opISub(typeId,
													  src[0].low.id,
													  src[1].low.id);
				uint32_t isPositive = m_module.opUGreaterThan(typeId,
															  sub,
															  m_module.constu32(0));
				// We shouldn't use OpSAbs directly,
				// OpSAbs will interpret x as a signed integer first,
				// in the edge cases like if x = 0xFFFFFFFF,
				// the result will be 1, not 0xFFFFFFFF, which is our expected result.
				// So we only apply opSAbs when it's negative or zero.
				uint32_t diff       = m_module.opSelect(typeId,
														isPositive,
														sub,
														m_module.opSAbs(typeId,
																		sub));
				dst.low.id          = m_module.opIAdd(typeId,
													  diff, src[2].low.id);
			}
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}

        emitRegisterStore(ins.dst[0], dst);
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

        bool updateExec = false;

		auto op = ins.opcode;
		switch (op)
		{
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
			case GcnOpcode::V_CMP_LT_U32:
				condition = m_module.opULessThan(conditionType,
												 src[0].low.id,
												 src[1].low.id);
				break;
			case GcnOpcode::V_CMPX_NE_U32:
				updateExec = true;
				[[fallthrough]];
			case GcnOpcode::V_CMP_NE_U32:
				condition = m_module.opINotEqual(conditionType,
												 src[0].low.id,
												 src[1].low.id);
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}

        // Generate constant vectors for selection
		uint32_t sFalse = m_module.constu32(0u);
		uint32_t sTrue  = m_module.constu32(~0u);

        // We only need to update the LSB of the low part of the result,
        // since we always assume we have only one thread.
		GcnRegisterValuePair result = {};
		result.low.type.ctype       = GcnScalarType::Uint32;
		result.low.type.ccount      = 1;
		if (isDoubleType(ins.dst[1].type))
        {
            // Alway set high 32-bits of a compare result 
            // to zero.
			result.high.type = result.low.type;
			result.high.id   = m_module.constu32(0);
        }

		const uint32_t typeId = getVectorTypeId(result.low.type);

		// Perform component-wise mask selection
		// based on the condition evaluated above.
		result.low.id = m_module.opSelect(
			typeId, condition, sTrue, sFalse);

        if (updateExec)
        {
			m_state.exec.emitStore(result, GcnRegMask::select(0));
        }

		emitRegisterStore(ins.dst[1], result);
	}

    void GcnCompiler::emitVectorRegMov(const GcnShaderInstruction& ins)
    {
        LOG_GCN_UNHANDLED_INST();
    }

    void GcnCompiler::emitVectorLane(const GcnShaderInstruction& ins)
    {
        LOG_GCN_UNHANDLED_INST();
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