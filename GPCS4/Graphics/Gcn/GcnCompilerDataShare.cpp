#include "GcnCompiler.h"

LOG_CHANNEL(Graphic.Gcn.GcnCompiler);

namespace sce::gcn
{
	void GcnCompiler::emitDataShare(const GcnShaderInstruction& ins)
	{
		auto opClass = ins.opClass;
		switch (opClass)
		{
			case GcnInstClass::DsIdxRd:
				this->emitDsIdxRd(ins);
				break;
			case GcnInstClass::DsIdxWr:
				this->emitDsIdxWr(ins);
				break;
			case GcnInstClass::DsIdxWrXchg:
				this->emitDsIdxWrXchg(ins);
				break;
			case GcnInstClass::DsIdxCondXchg:
				this->emitDsIdxCondXchg(ins);
				break;
			case GcnInstClass::DsIdxWrap:
				this->emitDsIdxWrap(ins);
				break;
			case GcnInstClass::DsAtomicArith32:
				this->emitDsAtomicArith32(ins);
				break;
			case GcnInstClass::DsAtomicArith64:
				this->emitDsAtomicArith64(ins);
				break;
			case GcnInstClass::DsAtomicMinMax32:
				this->emitDsAtomicMinMax32(ins);
				break;
			case GcnInstClass::DsAtomicMinMax64:
				this->emitDsAtomicMinMax64(ins);
				break;
			case GcnInstClass::DsAtomicCmpSt32:
				this->emitDsAtomicCmpSt32(ins);
				break;
			case GcnInstClass::DsAtomicCmpSt64:
				this->emitDsAtomicCmpSt64(ins);
				break;
			case GcnInstClass::DsAtomicLogic32:
				this->emitDsAtomicLogic32(ins);
				break;
			case GcnInstClass::DsAtomicLogic64:
				this->emitDsAtomicLogic64(ins);
				break;
			case GcnInstClass::DsAppendCon:
				this->emitDsAppendCon(ins);
				break;
			case GcnInstClass::DsDataShareUt:
				this->emitDsDataShareUt(ins);
				break;
			case GcnInstClass::DsDataShareMisc:
				this->emitDsDataShareMisc(ins);
				break;
			case GcnInstClass::GdsSync:
				this->emitGdsSync(ins);
				break;
			case GcnInstClass::GdsOrdCnt:
				this->emitGdsOrdCnt(ins);
				break;
		}
	}

	std::array<sce::gcn::GcnRegisterPointer, 4> 
		GcnCompiler::emitDsAccess(const GcnShaderInstruction& ins)
	{
		// one u8, s8, u16, s16  : result[0]
		// one u32 result[0]
		// two u32 result[0] result[1] 
		// one u64 result[0] result[1]
		// two u64 (result[0] result[1]) (result[2] result[3])

		auto ds = gcnInstructionAs<GcnShaderInstDS>(ins);
		const auto& control = ds.control;
		
		LOG_ASSERT(control.relative == 0, "TODO: support relative offset mode.");

		LOG_ASSERT(control.gds == 0, "TODO: support GDS.");
		uint32_t dsBufferId = control.gds ? 0 : m_lds;

		std::array<uint32_t, 2> offsetId = {};
		if (!control.dual)
		{
			uint32_t offset = control.offset1 << 8 | control.offset0;
			offsetId[0]     = m_module.constu32(offset);
		}
		else
		{
			LOG_ASSERT(control.offset0 != control.offset1, "TODO: support equal dual offsets");
			offsetId[0] = m_module.constu32(control.offset0);
			offsetId[1] = m_module.constu32(control.offset1);
		}

		const uint32_t uintTypeId = getScalarTypeId(GcnScalarType::Uint32);
		uint32_t       ptrTypeId  = m_module.defPointerType(
				   uintTypeId, spv::StorageClassWorkgroup);

		auto vbindex = emitRegisterLoad(ds.addr);

		std::array<uint32_t, 2> regionAddr = {};
		if (!control.dual)
		{
			// TODO:
			// Do we really need to align?
			regionAddr[0] = m_module.opIAdd(uintTypeId,
											offsetId[0], vbindex.low.id);
		}
		else
		{
			uint32_t opDataSize = m_module.constu32(control.size);
			if (control.stride)
			{
				opDataSize = m_module.opIMul(uintTypeId,
											 opDataSize,
											 m_module.constu32(64));
			}

			regionAddr[0] = m_module.opIAdd(uintTypeId,
											m_module.opIMul(uintTypeId,
															offsetId[0],
															opDataSize),
											vbindex.low.id);
			regionAddr[1] = m_module.opIAdd(uintTypeId,
											m_module.opIMul(uintTypeId,
															offsetId[1],
															opDataSize),
											vbindex.low.id);
		}

		std::array<sce::gcn::GcnRegisterPointer, 4> result = {};

		GcnRegisterPointer ptr;
		ptr.type.ctype  = GcnScalarType::Uint32;
		ptr.type.ccount = 1;

		// LDS and GDS are all uint arrays.
		uint32_t resultIdx = 0;
		for (uint32_t i = 0; i != 2; ++i)
		{
			uint32_t uintIdx    = m_module.opUDiv(uintTypeId,
												  regionAddr[i],
												  m_module.constu32(4));
			ptr.id              = m_module.opAccessChain(ptrTypeId, dsBufferId,
														 1, &uintIdx);
			result[resultIdx++] = ptr;
			if (control.size == 8)
			{
				uint32_t nextIdx    = m_module.opIAdd(uintTypeId, uintIdx, m_module.constu32(1));
				ptr.id              = m_module.opAccessChain(ptrTypeId, dsBufferId,
															 1, &nextIdx);
				result[resultIdx++] = ptr;
			}

			if (!control.dual)
			{
				break;
			}
		}

		return result;
	}

	void GcnCompiler::emitDsRead(const GcnShaderInstruction& ins)
	{
		auto ptrList = emitDsAccess(ins);

		GcnRegisterValuePair dst = {};
		dst.low.type.ctype       = getDestinationType(ins.dst[0].type);
		dst.low.type.ccount      = 1;
		dst.high.type            = dst.low.type;

		// We need to fix dst type for some instructions so make a copy.
		auto dstReg = ins.dst[0];

		uint32_t typeId = getScalarTypeId(dst.low.type.ctype);

		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::DS_READ_I8:
			case GcnOpcode::DS_READ_U8:
			case GcnOpcode::DS_READ_I16:
			case GcnOpcode::DS_READ_U16:
				LOG_GCN_UNHANDLED_INST();
				break;
			case GcnOpcode::DS_READ_B64:
				dst.high.id = m_module.opLoad(typeId, ptrList[1].id);
				[[fallthrough]];
			case GcnOpcode::DS_READ_B32:
				dst.low.id = m_module.opLoad(typeId, ptrList[0].id);
				break;
			case GcnOpcode::DS_READ2_B32:
			{
				// Fix dst type so we don't miss high part.
				dstReg.type = GcnScalarType::Uint64;
				dst.low.id  = m_module.opLoad(typeId, ptrList[0].id);
				dst.high.id = m_module.opLoad(typeId, ptrList[1].id);
			}
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}

		emitRegisterStore(dstReg, dst);
	}

	void GcnCompiler::emitDsWrite(const GcnShaderInstruction& ins)
	{
		auto ptrList = emitDsAccess(ins);

		std::array<GcnRegisterValuePair, 2> src;
		for (uint32_t i = 0; i != 2; ++i)
		{
			// DS encoding src starts from src[1]
			src[i] = emitRegisterLoad(ins.src[1 + i]);
			if (!ins.control.ds.dual)
			{
				break;
			}
		}

		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::DS_WRITE_B8:
			case GcnOpcode::DS_WRITE_B16:
				LOG_GCN_UNHANDLED_INST();
				break;
			case GcnOpcode::DS_WRITE_B64:
				emitValueStore(ptrList[1], src[0].high, GcnRegMask::select(0));
				[[fallthrough]];
			case GcnOpcode::DS_WRITE_B32:
				emitValueStore(ptrList[0], src[0].low, GcnRegMask::select(0));
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}
	}

	void GcnCompiler::emitDsIdxRd(const GcnShaderInstruction& ins)
	{
		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::DS_READ_I8:
			case GcnOpcode::DS_READ_U8:
			case GcnOpcode::DS_READ_I16:
			case GcnOpcode::DS_READ_U16:
			case GcnOpcode::DS_READ_B64:
			case GcnOpcode::DS_READ_B32:
			case GcnOpcode::DS_READ2_B32:
				this->emitDsRead(ins);
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}
	}

	void GcnCompiler::emitDsIdxWr(const GcnShaderInstruction& ins)
	{
		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::DS_WRITE_B8:
			case GcnOpcode::DS_WRITE_B16:
			case GcnOpcode::DS_WRITE_B32:
			case GcnOpcode::DS_WRITE_B64:
				this->emitDsWrite(ins);
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}
	}

	void GcnCompiler::emitDsIdxWrXchg(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitDsIdxCondXchg(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitDsIdxWrap(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitDsAtomicArith32(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitDsAtomicArith64(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitDsAtomicMinMax32(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitDsAtomicMinMax64(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitDsAtomicCmpSt32(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitDsAtomicCmpSt64(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitDsAtomicLogic32(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitDsAtomicLogic64(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitDsAppendCon(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitDsDataShareUt(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitDsDataShareMisc(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitGdsSync(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitGdsOrdCnt(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}
}  // namespace sce::gcn
