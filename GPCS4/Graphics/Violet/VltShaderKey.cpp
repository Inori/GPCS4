#include "VltShaderKey.h"

namespace sce::vlt
{
	VltShaderKey::VltShaderKey() :
		m_type(0),
		m_key(0, 0)
	{
	}

	std::string VltShaderKey::toString() const
	{
		const char* prefix = nullptr;

		switch (m_type)
		{
		case VK_SHADER_STAGE_VERTEX_BIT:
			prefix = "VS_";
			break;
		case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
			prefix = "TCS_";
			break;
		case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
			prefix = "TES_";
			break;
		case VK_SHADER_STAGE_GEOMETRY_BIT:
			prefix = "GS_";
			break;
		case VK_SHADER_STAGE_FRAGMENT_BIT:
			prefix = "FS_";
			break;
		case VK_SHADER_STAGE_COMPUTE_BIT:
			prefix = "CS_";
			break;
		default:
			prefix = "";
		}

		return util::str::formatex(prefix, m_key.name());
	}

	size_t VltShaderKey::hash() const
	{
		VltHashState result;
		result.add(uint32_t(m_type));

		uint64_t key = m_key.key();
		result.add(key);

		return result;
	}

	bool VltShaderKey::eq(const VltShaderKey& other) const
	{
		return m_type == other.m_type &&
			   m_key == other.m_key;
	}
}  // namespace sce::vlt