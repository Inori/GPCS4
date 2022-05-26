#include "GcnStackifier.h"



namespace sce::gcn
{


	//////////////////////////////////////////////////////////////////////////
	GcnStackifier::GcnStackifier(GcnControlFlowGraph& cfg):
		m_cfg(cfg)
	{
	}

	GcnStackifier::~GcnStackifier()
	{
	}
}  // namespace sce::gcn