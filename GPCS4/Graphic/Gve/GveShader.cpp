#include "GveShader.h"
#include "Platform/UtilFile.h"

#ifdef GPCS4_DEBUG
// Dump shader to file
#define GVE_DUMP_SHADER
#endif

namespace gve
{;

GveShader::GveShader(VkShaderStageFlagBits stage, 
	SpirvCodeBuffer code,
	const PsslKey& key):
	m_stage(stage),
	m_code(code),
	m_key(key)
{

#ifdef GVE_DUMP_SHADER
	dumpShader();
#endif // GVE_DUMP_SHADER

}

GveShader::~GveShader()
{
}

void GveShader::dumpShader()
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

	auto code = m_code.decompress();
	sprintf_s(filename, 64, format, m_key.toUint64());
	UtilFile::StoreFile(filename, (uint8_t*)code.data(), code.size());
}

}  // namespace gve