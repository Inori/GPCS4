#include "GcnCompiler.h"
#include "GcnHeader.h"

LOG_CHANNEL(Graphic.Gcn.GcnCompiler);

namespace sce::gcn
{
	void GcnCompiler::emitScalarMemory(const GcnShaderInstruction& ins)
	{
		auto opClass = ins.opClass;
		switch (opClass)
		{
			case GcnInstClass::ScalarMemRd:
				this->emitScalarMemRd(ins);
				break;
			case GcnInstClass::ScalarMemUt:
				this->emitScalarMemUt(ins);
				break;
		}
	}

	void GcnCompiler::emitScalarMemRd(const GcnShaderInstruction& ins)
	{
		auto smrd = gcnInstructionAs<GcnShaderInstSMRD>(ins);

		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::S_BUFFER_LOAD_DWORD:
			case GcnOpcode::S_BUFFER_LOAD_DWORDX2:
			case GcnOpcode::S_BUFFER_LOAD_DWORDX4:
			case GcnOpcode::S_BUFFER_LOAD_DWORDX8:
			case GcnOpcode::S_BUFFER_LOAD_DWORDX16:
			{
				GcnRegIndex index = {};
				// This is the sgpr pair index, so we need to multiply by 2
				index.regIdx      = smrd.sbase.code * 2;
				if (smrd.control.imm)
				{
					index.offset = smrd.control.offset << 2;
					index.relReg = nullptr;
				}
				else
				{
					index.offset = 0;
					index.relReg = &smrd.offset;
				}
				emitConstantBufferLoad(index, smrd.sdst, smrd.control.count);
			}
				break;
			case GcnOpcode::S_LOAD_DWORDX4:
			case GcnOpcode::S_LOAD_DWORDX8:
				emitRawBufferLoad(ins);
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}
	}

	void GcnCompiler::emitRawBufferLoad(const GcnShaderInstruction& ins)
	{
		// Currently, the only usage case I found for S_LOAD_DWORDX4 S_LOAD_DWORDX8
		// is to load 4 or 8 sgpr sharp descriptors from EUD.
		// We've already declared those resource in emitDclInputSlots,
		// so we only check if it is used for loading sgprs in EUD in case of
		// some unknown use cases,
		// then we simply ignore these instructions.

		const auto& resouceTable = m_header->getShaderResourceTable();
		auto        iter         = std::find_if(
			resouceTable.cbegin(),
			resouceTable.cend(),
			[](const GcnShaderResource& res)
			{
                return res.usage == Gnm::kShaderInputUsagePtrExtendedUserData;
			});

		if (iter != resouceTable.end())
		{
			// We found an EUD slot.
			// Check if it is used as the sbase of the instruction.
			uint32_t sbase = ins.src[0].code << 1;
			LOG_ASSERT(sbase == iter->startRegister, "S_LOAD_XXX is not used to load EUD.");
		}
	}

	void GcnCompiler::emitScalarMemUt(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}
}  // namespace sce::gcn