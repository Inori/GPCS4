#include "GcnProgramInfo.h"

LOG_CHANNEL(Graphic.Gcn.GcnProgramInfo);

namespace sce::gcn
{
	VkShaderStageFlagBits GcnProgramInfo::shaderStage() const
	{
		switch (m_type)
		{
		case GcnProgramType::PixelShader:
			return VK_SHADER_STAGE_FRAGMENT_BIT;
		case GcnProgramType::VertexShader:
			return VK_SHADER_STAGE_VERTEX_BIT;
		case GcnProgramType::GeometryShader:
			return VK_SHADER_STAGE_GEOMETRY_BIT;
		case GcnProgramType::HullShader:
			return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		case GcnProgramType::DomainShader:
			return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		case GcnProgramType::ComputeShader:
			return VK_SHADER_STAGE_COMPUTE_BIT;
		}

		LOG_ERR("DxbcProgramInfo::shaderStage: Unsupported program type");
	}

	spv::ExecutionModel GcnProgramInfo::executionModel() const
	{
		switch (m_type)
		{
		case GcnProgramType::PixelShader:
			return spv::ExecutionModelFragment;
		case GcnProgramType::VertexShader:
			return spv::ExecutionModelVertex;
		case GcnProgramType::GeometryShader:
			return spv::ExecutionModelGeometry;
		case GcnProgramType::HullShader:
			return spv::ExecutionModelTessellationControl;
		case GcnProgramType::DomainShader:
			return spv::ExecutionModelTessellationEvaluation;
		case GcnProgramType::ComputeShader:
			return spv::ExecutionModelGLCompute;
		}

		LOG_ERR("DxbcProgramInfo::executionModel: Unsupported program type");
	}
}  // namespace sce::gcn