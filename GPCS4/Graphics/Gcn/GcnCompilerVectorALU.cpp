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
                this->emitVectorAluCommon(ins);
                break;
            case GcnInstClass::VectorLane:
                this->emitVectorLane(ins);
                break;
            case GcnInstClass::VectorBitField32:
                this->emitVectorBitField32(ins);
                break;
            case GcnInstClass::VectorThreadMask:
                this->emitVectorThreadMask(ins);
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
            case GcnInstClass::VectorFpTran32:
                this->emitVectorFpTran32(ins);
                break;
            case GcnInstClass::VectorFpCmp32:
                this->emitVectorFpCmp32(ins);
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
            case GcnInstClass::VectorFpCmp64:
                this->emitVectorFpCmp64(ins);
                break;
            case GcnInstClass::VectorIntArith32:
                this->emitVectorIntArith32(ins);
                break;
            case GcnInstClass::VectorIntArith64:
                this->emitVectorIntArith64(ins);
                break;
            case GcnInstClass::VectorIntCmp32:
                this->emitVectorIntCmp32(ins);
                break;
            case GcnInstClass::VectorIntCmp64:
                this->emitVectorIntCmp64(ins);
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
			// VectorRegMov
			case GcnOpcode::V_MOV_B32:
				dst.low.id = src[0].low.id;
			    break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}

        emitRegisterStore(ins.dst[0], dst);
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