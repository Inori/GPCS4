#pragma once

#include "VltCommon.h"
#include "VltPipelineLayout.h"
#include "../Pssl/PsslKey.h"
#include "../Pssl/PsslBindingCalculator.h"
#include "../SpirV/SpirvCodeBuffer.h"
#include "../SpirV/SpirvCompression.h"

namespace vlt
{;

class VltDevice;
class VltShaderModule;

class VltShader : public RcObject
{
	using PsslKey               = pssl::PsslKey;
	using SpirvCodeBuffer       = pssl::SpirvCodeBuffer;
	using SpirvCompressedBuffer = pssl::SpirvCompressedBuffer;

public:
	VltShader(VkShaderStageFlagBits          stage,
			  SpirvCodeBuffer                code,
			  const PsslKey&                 key,
			  std::vector<VltResourceSlot>&& resSlots);

	virtual ~VltShader();

	VkShaderStageFlagBits stage() const;

	void defineResourceSlots(VltDescriptorSlotMap& slotMap) const;

	VltShaderModule createShaderModule(const VltDevice*            device,
									   const VltDescriptorSlotMap& slotMap);

	PsslKey key();

	bool operator==(const VltShader& other);

	/**
	 * \brief Replace shader bytecode content
	 * 
	 * Only used for debugging purpose.
	 *
	 * Note:
	 * The caller must make sure, the replaced shader
	 * code shares the same interface with the original shader.
	 **
	 * \param code New shader bytecode.
	 * \returns void
	 */
	void replaceCode(const std::vector<uint8_t>& code);

	/**
	 * \brief Dump shader to file
	 * 
	 * Only used for debugging purpose.
	 * \returns void
	 */
	void dumpShader() const;

private:
	void generateBindingIdOffsets(SpirvCodeBuffer& code);
	void dumpShader(const SpirvCodeBuffer& code) const;

private:
	VkShaderStageFlagBits        m_stage;
	SpirvCompressedBuffer        m_code;
	PsslKey                      m_key;
	std::vector<uint32_t>        m_bindingIdOffsets;
	std::vector<VltResourceSlot> m_slots;

	SpirvCodeBuffer				 m_debugCode;
};


/**
* \brief Shader module object
*
* Manages a Vulkan shader module. This will not
* perform any shader compilation. Instead, the
* context will create pipeline objects on the
* fly when executing draw calls.
*/
class VltShaderModule
{

public:
	VltShaderModule();

	VltShaderModule(VltShaderModule&& other);

	VltShaderModule(
		const VltDevice*             device,
		const RcPtr<VltShader>&      shader,
		const pssl::SpirvCodeBuffer& code);

	~VltShaderModule();

	VltShaderModule& operator=(VltShaderModule&& other);

	/**
	 * \brief Shader stage creation info
	 *
	 * \param [in] specInfo Specialization info
	 * \returns Shader stage create info
	 */
	VkPipelineShaderStageCreateInfo stageInfo(const VkSpecializationInfo* specInfo) const
	{
		VkPipelineShaderStageCreateInfo stage = m_stage;
		stage.pSpecializationInfo             = specInfo;
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
	const VltDevice*                m_device;
	VkPipelineShaderStageCreateInfo m_stage;
};


}  // namespace vlt
