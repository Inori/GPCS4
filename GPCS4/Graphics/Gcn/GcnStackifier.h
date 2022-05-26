#pragma once

#include "GcnCommon.h"
#include "GcnTokenList.h"
#include "GcnLoopInfo.h"

#include <vector>

namespace sce::gcn
{


	class GcnStackifier
	{
	public:
		GcnStackifier(GcnControlFlowGraph& cfg);
		~GcnStackifier();

	private:
		GcnControlFlowGraph& m_cfg;
		GcnTokenList         m_tokens;
	};

}  // namespace sce::gcn