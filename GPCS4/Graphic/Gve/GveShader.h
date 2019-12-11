#pragma once

#include "GveCommon.h"
#include "GvePipelineLayout.h"
#include "../Pssl/PsslKey.h"
#include "../Pssl/PsslBindingCalculator.h"
#include "../SpirV/SpirvCodeBuffer.h"
#include "../SpirV/SpirvCompression.h"

namespace gve
{;

class GveDevice;
class GveShaderModule;

class GveShader : public RcObject
{
	using PsslKey = pssl::PsslKey;
	using SpirvCodeBuffer = pssl::SpirvCodeBuffer;
	using SpirvCompressedBuffer = pssl::SpirvCompressedBuffer;

public:
	GveShader(VkShaderStageFlagBits stage,
			  SpirvCodeBuffer code,
			  const PsslKey& key,
			  std::vector<GveResourceSlot>&& resSlots);

	virtual ~GveShader();

	VkShaderStageFlagBits stage() const;

	void fillResourceSlots(GveDescriptorSlotMap& slotMap) const;

	GveShaderModule createShaderModule(const GveDevice* device,
		const GveDescriptorSlotMap& slotMap);

	PsslKey key();

	void dumpShader() const;

	bool operator ==(const GveShader& other);

private:
	void generateBindingIdOffsets(SpirvCodeBuffer& code);
	void dumpShader(const SpirvCodeBuffer& code) const;
	
private:
	VkShaderStageFlagBits m_stage;
	SpirvCompressedBuffer m_code;
	PsslKey m_key;
	std::vector<uint32_t> m_bindingIdOffsets;
	std::vector<GveResourceSlot> m_slots;
};


/**
* \brief Shader module object
*
* Manages a Vulkan shader module. This will not
* perform any shader compilation. Instead, the
* context will create pipeline objects on the
* fly when executing draw calls.
*/
class GveShaderModule 
{

public:

	GveShaderModule();

	GveShaderModule(GveShaderModule&& other);

	GveShaderModule(
		const GveDevice* device,
		const RcPtr<GveShader>& shader,
		const pssl::SpirvCodeBuffer& code
	);

	~GveShaderModule();

	GveShaderModule& operator = (GveShaderModule&& other);

	/**
	 * \brief Shader stage creation info
	 *
	 * \param [in] specInfo Specialization info
	 * \returns Shader stage create info
	 */
	VkPipelineShaderStageCreateInfo stageInfo(const VkSpecializationInfo* specInfo) const
	{
		VkPipelineShaderStageCreateInfo stage = m_stage;
		stage.pSpecializationInfo = specInfo;
		return stage;
	}

	/**
	 * \brief Checks whether module is valid
	 * \returns \c true if module is valid
	 */
	operator bool() const 
	{
		return m_stage.module != VK_NULL_HANDLE;
	}

private:

	const GveDevice* m_device;
	VkPipelineShaderStageCreateInfo m_stage;

};


}  // namespace gve
