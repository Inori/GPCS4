#pragma once

#include "GcnCommon.h"
#include "GcnShaderBinary.h"

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
			const std::vector<uint8_t>& shaderCode);
		~GcnHeader();

		const ShaderBinaryInfo& getShaderBinaryInfo();

		InputUsageSlotTable getInputUsageSlotTable();

	private:
		void parseHeader(
			const std::vector<uint8_t>& shaderCode);

	private:
		ShaderBinaryInfo    m_binInfo = {};
		InputUsageSlotTable m_inputUsageSlotTable;
	};


}  // namespace sce::gcn