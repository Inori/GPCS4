#include "VltShader.h"
#include "VltDevice.h"
#include "Platform/UtilFile.h"

LOG_CHANNEL(Graphic.Violet.VltShader);

namespace vlt
{;

VltShader::VltShader(VkShaderStageFlagBits          stage,
					 SpirvCodeBuffer                code,
					 const PsslKey&                 key,
					 std::vector<VltResourceSlot>&& resSlots) :
	m_stage(stage),
	m_code(code),
	m_key(key),
	m_slots(resSlots)
{
	generateBindingIdOffsets(code);
}

VltShader::~VltShader()
{
}

VkShaderStageFlagBits VltShader::stage() const
{
	return m_stage;
}

void VltShader::defineResourceSlots(VltDescriptorSlotMap& slotMap) const
{
	for (const auto& slot : m_slots)
	{
		slotMap.defineSlot(slot, m_stage);
	}
}

VltShaderModule VltShader::createShaderModule(
	const VltDevice*            device,
	const VltDescriptorSlotMap& slotMap)
{
	SpirvCodeBuffer spirvCode = m_code.decompress();

	// Remap resource binding IDs
	uint32_t* code = spirvCode.data();
	for (uint32_t off : m_bindingIdOffsets)
	{
		uint32_t binding = slotMap.getBindingId(m_stage, code[off]);
		if (binding < MaxNumResourceSlots)
		{
			code[off] = binding;
		}
	}

#ifdef VLT_DUMP_SHADER
	dumpShader(spirvCode);
#endif // VLT_DUMP_SHADER

	if (m_debugCode.size())
	{
		spirvCode = m_debugCode;
	}

	return VltShaderModule(device, this, spirvCode);
}

VltShader::PsslKey VltShader::key()
{
	return m_key;
}

bool VltShader::operator==(const VltShader& other)
{
	return m_key == other.m_key;
}

void VltShader::generateBindingIdOffsets(SpirvCodeBuffer& code)
{
	for (auto ins : code) 
	{
		if (ins.opCode() == spv::OpDecorate) 
		{
			if (ins.arg(2) == spv::DecorationBinding || 
				ins.arg(2) == spv::DecorationSpecId)
			{
				m_bindingIdOffsets.push_back(ins.offset() + 3);
			}
		}
	}
}


void VltShader::dumpShader(const SpirvCodeBuffer& code) const
{
	char filename[64] = { 0 };
	const char* format = nullptr;

	switch (m_stage)
	{
	case VK_SHADER_STAGE_VERTEX_BIT:
		format = "%016llX.vs.spv";
		break;
	case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
		format = "%016llX.hs.spv";
		break;
	case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
		format = "%016llX.ds.spv";
		break;
	case VK_SHADER_STAGE_GEOMETRY_BIT:
		format = "%016llX.gs.spv";
		break;
	case VK_SHADER_STAGE_FRAGMENT_BIT:
		format = "%016llX.ps.spv";
		break;
	case VK_SHADER_STAGE_COMPUTE_BIT:
		format = "%016llX.cs.spv";
		break;
	default:
		break;
	}

	sprintf_s(filename, 64, format, m_key.toUint64());
	UtilFile::StoreFile(filename, (uint8_t*)code.data(), code.size());
}

void VltShader::replaceCode(const std::vector<uint8_t>& code)
{
	SpirvCodeBuffer buffer(code.size() / sizeof(uint32_t), reinterpret_cast<const uint32_t*>(code.data()));
	m_debugCode.append(buffer);
}

void VltShader::dumpShader() const
{
	// Note:
	// Binding IDs in this dump is not fixed,
	// so it is only used to view the translation result for debugging purpose.
	auto code = m_code.decompress();
	dumpShader(code);
}
///

VltShaderModule::VltShaderModule():
	m_device(nullptr),
	m_stage()
{
}

VltShaderModule::VltShaderModule(
	const VltDevice*             device,
	const RcPtr<VltShader>&      shader,
	const pssl::SpirvCodeBuffer& code) :
	m_device(device),
	m_stage()
{
	m_stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	m_stage.pNext = nullptr;
	m_stage.flags = 0;
	m_stage.stage = shader->stage();
	m_stage.module = VK_NULL_HANDLE;
	m_stage.pName = "main";
	m_stage.pSpecializationInfo = nullptr;

	VkShaderModuleCreateInfo info;
	info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.codeSize = code.size();
	info.pCode = code.data();

	if (vkCreateShaderModule(*m_device, &info, nullptr, &m_stage.module) != VK_SUCCESS)
	{
		LOG_ERR("Failed to create shader module");
	}
}

VltShaderModule::VltShaderModule(VltShaderModule&& other)
{
	m_stage = other.m_stage;
	other.m_stage = VkPipelineShaderStageCreateInfo();
}

VltShaderModule::~VltShaderModule() 
{
	vkDestroyShaderModule(*m_device, m_stage.module, nullptr);
}

VltShaderModule& VltShaderModule::operator = (VltShaderModule&& other) 
{
	m_device = other.m_device;
	this->m_stage = other.m_stage;
	other.m_stage = VkPipelineShaderStageCreateInfo();
	return *this;
}


}  // namespace vlt