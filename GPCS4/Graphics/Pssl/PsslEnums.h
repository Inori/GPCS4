#pragma once

namespace pssl
{;


enum class PsslProgramType
{
	PixelShader    = 0,
	VertexShader   = 1,
	GeometryShader = 2,
	HullShader     = 3,
	DomainShader   = 4,
	ComputeShader  = 5,
	FetchShader    = 6,
	ShaderTypeCount
};


}  // namespace pssl
