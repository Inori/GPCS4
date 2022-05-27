#include "GcnStackifier.h"

namespace sce::gcn
{

	//////////////////////////////////////////////////////////////////////////
	GcnStackifier::GcnStackifier(GcnControlFlowGraph& cfg):
		m_cfg(cfg),
		m_factory(m_pool),
		m_loopInfo(cfg),
		m_domTree(cfg)
	{
	}

	GcnStackifier::~GcnStackifier()
	{
	}
}  // namespace sce::gcn