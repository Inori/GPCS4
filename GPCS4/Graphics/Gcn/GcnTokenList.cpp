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

	std::list<GcnToken*>::iterator GcnToken::getIterator(GcnTokenList& list)
	{
		return list.find(this);
	}

	std::list<GcnToken*>::reverse_iterator GcnToken::getRevIterator(GcnTokenList& list)
	{
		return std::make_reverse_iterator(--getIterator(list));
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