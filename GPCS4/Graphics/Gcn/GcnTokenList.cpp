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

	GcnToken* GcnToken::getPrevNode(GcnTokenList& list)
	{
		auto iter = getIterator(list);
		return *(--iter);
	}

	GcnToken* GcnToken::getNextNode(GcnTokenList& list)
	{
		auto iter = getIterator(list);
		return *(++iter);
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