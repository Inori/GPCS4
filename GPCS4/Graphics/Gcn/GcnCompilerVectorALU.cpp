#include "GcnCompiler.h"

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
            case GcnInstClass::VectorBitField32:
                this->emitVectorBitField32(ins);
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
            case GcnInstClass::VectorIntGraph:
                this->emitVectorIntGraph(ins);
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
        dst.low.type.ctype       = ins.dst[0].type;
        dst.low.type.ccount      = 1;
        dst.high.type.ctype      = ins.dst[0].type;
        dst.high.type.ccount     = 1;

        const uint32_t typeId = getVectorTypeId(dst.low.type);

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
				dst.low.id = m_module.opISub(typeId,
											 src[0].low.id,
											 src[1].low.id);
                break;
			case GcnOpcode::V_MAX_F32:
				dst.low.id = m_module.opFMax(typeId,
											 src[0].low.id,
											 src[1].low.id);
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
            // VectorFpTran32
			case GcnOpcode::V_RSQ_F32:
				dst.low.id = m_module.opInverseSqrt(typeId,
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
			case GcnOpcode::V_MADMK_F32:
				dst.low.id = m_module.opFAdd(typeId,
											 m_module.opFMul(typeId, src[0].low.id, src[2].low.id),
											 src[1].low.id);
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
			case GcnOpcode::V_CMPX_GT_U32:
			{
				condition  = m_module.opUGreaterThan(conditionType,
													 src[0].low.id,
													 src[1].low.id);
				updateExec = true;
			}
                break;
			case GcnOpcode::V_CMP_GE_F32:
				condition = m_module.opFOrdGreaterThanEqual(conditionType,
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
        LOG_GCN_UNHANDLED_INST();
    }

    void GcnCompiler::emitVectorIntGraph(const GcnShaderInstruction& ins)
    {
        LOG_GCN_UNHANDLED_INST();
    }

    void GcnCompiler::emitVectorMisc(const GcnShaderInstruction& ins)
    {
        LOG_GCN_UNHANDLED_INST();
    }
}  // namespace sce::gcn