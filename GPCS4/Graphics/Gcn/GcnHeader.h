#pragma once

#include "GcnCommon.h"
#include "GcnShaderBinary.h"
#include "GcnShaderKey.h"

#include <vector>

namespace sce::gcn
{
	using InputUsageSlotTable = std::vector<InputUsageSlot>;

	/**
	 * \brief Header information 
	 *
	 * Stores header for a shader binary 
	 * sent to graphics driver
	 */
	class GcnHeader
	{
	public:
		GcnHeader(
			const uint8_t* shaderCode);
		~GcnHeader();

		/**
		 * \brief Unique id of the shader
		 */
		GcnShaderKey key() const
		{
			return GcnShaderKey(
				m_binInfo.m_shaderHash0, m_binInfo.m_crc32);
		}

		/**
		 * \brief Shader code size in bytes
		 */
		uint32_t length() const
		{
			return m_binInfo.m_length;
		}

		const ShaderBinaryInfo& getShaderBinaryInfo();

		InputUsageSlotTable getInputUsageSlotTable();

	private:
		void parseHeader(
			const uint8_t* shaderCode);

	private:
		ShaderBinaryInfo    m_binInfo = {};
		InputUsageSlotTable m_inputUsageSlotTable;
	};


}  // namespace sce::gcn