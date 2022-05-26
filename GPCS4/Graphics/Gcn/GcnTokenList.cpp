#include "GcnTokenList.h"

namespace sce::gcn
{

	GcnToken::GcnToken(GcnTokenKind kind, GcnCfgVertex vertex, GcnToken* match) :
		m_kind(kind),
		m_vertex(vertex),
		m_match(match)
	{
	}

	GcnToken::~GcnToken()
	{
	}

	////////////////////////////////////////////////////////////
	GcnTokenFactory::GcnTokenFactory(util::ObjectBank<GcnToken>& tokenPool):
		m_pool(tokenPool)
	{
	}

	GcnTokenFactory::~GcnTokenFactory()
	{
	}

	////////////////////////////////////////////////////////////
	GcnTokenList::GcnTokenList()
	{
	}

	GcnTokenList::~GcnTokenList()
	{
	}

}  // namespace sce::gcn