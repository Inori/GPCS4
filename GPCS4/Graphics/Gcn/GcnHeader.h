#pragma once

#include "GcnCommon.h"
#include "GcnShaderBinary.h"
#include "GcnShaderKey.h"

#include <vector>

namespace sce::gcn
{
	/**
	 * \brief Represent a resource bound to a GCN shader
	 */
	struct GcnShaderResource
	{
		VkDescriptorType type;

		// ShaderInputUsageType
		uint32_t usage;

		// SGPR index
		uint32_t startRegister;

		// Resource is in User Data Register
		// or in Extended User Data region.
		bool inEud;

		// Dword offset in EUD
		uint32_t eudOffsetInDwords;

		// Register size in dwords
		uint32_t sizeInDwords;
	};

	using GcnShaderResourceTable = std::vector<GcnShaderResource>;


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

		const ShaderBinaryInfo& getShaderBinaryInfo() const
		{
			return m_binInfo;
		}

		const InputUsageSlotTable& getInputUsageSlotTable() const
		{
			return m_inputUsageSlotTable;
		}

		const GcnShaderResourceTable& getShaderResourceTable() const
		{
			return m_resourceTable;
		}

	private:
		void parseHeader(
			const uint8_t* shaderCode);

		void extractResourceTable();

	private:
		ShaderBinaryInfo       m_binInfo = {};
		InputUsageSlotTable    m_inputUsageSlotTable;
		GcnShaderResourceTable m_resourceTable;
	};


}  // namespace sce::gcn