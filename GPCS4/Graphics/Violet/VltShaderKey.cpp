#include "VltShaderKey.h"

namespace sce::vlt
{
	VltShaderKey::VltShaderKey() :
		m_type(0),
		m_sha1(alg::Sha1Hash::compute(nullptr, 0))
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

		return util::str::formatex(prefix, m_sha1.toString());
	}

	size_t VltShaderKey::hash() const
	{
		VltHashState result;
		result.add(uint32_t(m_type));

		for (uint32_t i = 0; i < 5; i++)
			result.add(m_sha1.dword(i));

		return result;
	}

	bool VltShaderKey::eq(const VltShaderKey& key) const
	{
		return m_type == key.m_type && m_sha1 == key.m_sha1;
	}
}  // namespace sce::vlt