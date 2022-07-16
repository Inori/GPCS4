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

				emitScalarBufferLoad(index, smrd.sdst, smrd.control.count);
			}
				break;
			case GcnOpcode::S_LOAD_DWORDX4:
			case GcnOpcode::S_LOAD_DWORDX8:
				mapEudResource(ins);
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}
	}

	void GcnCompiler::mapEudResource(const GcnShaderInstruction& ins)
	{
		// Currently, the only usage case I found for S_LOAD_DWORDX4 S_LOAD_DWORDX8
		// is to load 4 or 8 sgpr sharp descriptors from EUD.

		LOG_ASSERT(ins.control.smrd.imm == 1, "TODO: support soffset eud load.");

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

			uint32_t eudOffset = ins.control.smrd.offset;
			uint32_t eudReg    = eudOffset + kMaxUserDataCount;
			uint32_t dstReg    = ins.dst[0].code;

			auto resIt = std::find_if(
				resouceTable.cbegin(),
				resouceTable.cend(),
				[eudReg](const GcnShaderResource& res)
				{
					return res.startRegister == eudReg;
				});

			auto& res = *resIt;
			switch (res.type)
			{
				case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
				case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
					m_buffers.at(dstReg) = m_buffersDcl.at(eudReg);
					break;
				case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
				case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
					m_textures.at(dstReg) = m_texturesDcl.at(eudReg);
					break;
				case VK_DESCRIPTOR_TYPE_SAMPLER:
					m_samplers.at(dstReg) = m_samplersDcl.at(eudReg);
					break;
				case VK_DESCRIPTOR_TYPE_MAX_ENUM:
					// skip
					break;
				default:
					LOG_ASSERT(false, "Not supported resouce type mapped.");
					break;
			}
		}
	}

	void GcnCompiler::emitScalarMemUt(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}
}  // namespace sce::gcn