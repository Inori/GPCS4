#pragma once

#include "GcnCommon.h"
#include "GcnShaderBinary.h"

#include <vector>

namespace sce::gcn
{
	class GcnFetchShader
	{
	public:
		GcnFetchShader(const uint8_t* code);
		~GcnFetchShader();

		const VertexInputSemanticTable&
			getVertexInputSemanticTable() const
		{
			return m_vsInputSemanticTable;
		}

	private:
		void parseVsInputSemantic(const uint8_t* code);

	private:
		VertexInputSemanticTable m_vsInputSemanticTable;
	};


}  // namespace sce::gcn