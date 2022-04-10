#pragma once

#include "GcnCommon.h"
#include "GcnShaderBinary.h"

#include <vector>

namespace sce::gcn
{
	using VertexInputSemanticTable = std::vector<VertexInputSemantic>;

	class GcnFetchShader
	{
	public:
		GcnFetchShader(
			const std::vector<uint8_t>& code);
		~GcnFetchShader();

		VertexInputSemanticTable getVertexInputSemanticTable();

	private:
		void parseVsInputSemantic(
			const std::vector<uint8_t>& code);

	private:
		VertexInputSemanticTable m_vsInputSemanticTable;
	};


}  // namespace sce::gcn