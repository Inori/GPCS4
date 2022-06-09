#pragma once

#include "Gcn/GcnCommon.h"
#include "GcnTokenList.h"

namespace sce::gcn
{
	class GcnDivergentFlow
	{
	public:
		GcnDivergentFlow(GcnTokenFactory& factory);
		~GcnDivergentFlow();

		void diverge(GcnTokenList& tokens);

	private:
		bool needDiverge(GcnToken* token);
		void divergeCode(GcnToken* token);

		GcnDivergentAction getDivergentAction(
			const GcnShaderInstruction& ins);

		GcnShaderInstruction makeClearInstruction();

		bool isNonCompileInst(
			const GcnShaderInstruction& ins);
	private:
		GcnTokenFactory& m_factory;
		GcnTokenList*    m_tokens;
	};
}  // namespace sce::gcn

