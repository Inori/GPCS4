#pragma once

#include "GcnCommon.h"
#include "GcnShaderBinary.h"
#include "GcnShaderKey.h"

#include <vector>
#include <unordered_set>

namespace sce::gcn
{
	enum class GcnProgramType;

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
	 * \brief Light weight binary information parser
	 *
	 * Data in this class is not persistent, will become
	 * invalid when shader code released.
	 */
	class GcnBinaryInfo
	{
	public:
		GcnBinaryInfo(
			const uint8_t* shaderCode);
		~GcnBinaryInfo();

		/**
		 * \brief Code length
		 * 
		 * Gcn instruction code length in bytes.
		 * Does not include header and 
		 * other meta information
		 */
		uint32_t length() const
		{
			return m_binInfo->m_length;
		}

		/**
		 * \brief Unique key
		 * 
		 * Used to identify the shader
		 */
		GcnShaderKey key() const
		{
			return GcnShaderKey(
				m_binInfo->m_shaderHash0, m_binInfo->m_crc32);
		}

		/**
		 * \brief Information of shader binary
		 */
		const ShaderBinaryInfo* info() const
		{
			return m_binInfo;
		}

	private:
		const ShaderBinaryInfo* m_binInfo = nullptr;
	};


	/**
	 * \brief Header information 
	 *
	 * Stores header for a shader binary 
	 * sent to graphics driver
	 */
	class GcnHeader
	{
		struct ResourceTypeInfo
		{
			std::unordered_set<uint32_t> m_storageImages;
		};

	public:
		GcnHeader(
			const uint8_t* shaderCode);
		~GcnHeader();

		GcnProgramType type() const;

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

		void extractResourceTable(const uint8_t* code);

		ResourceTypeInfo
			analyzeResourceType(const uint8_t* code);

	private:
		ShaderBinaryInfo       m_binInfo = {};
		InputUsageSlotTable    m_inputUsageSlotTable;
		GcnShaderResourceTable m_resourceTable;
	};


}  // namespace sce::gcn