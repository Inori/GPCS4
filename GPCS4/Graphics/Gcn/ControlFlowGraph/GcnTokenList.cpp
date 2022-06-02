#include "GcnTokenList.h"

namespace sce::gcn
{
	GcnToken::GcnToken(GcnTokenKind kind, GcnCfgVertex vertex, GcnToken* match) :
		m_kind(kind),
		m_vertex(vertex),
		m_match(match)
	{
	}

	GcnToken::GcnToken(GcnTokenKind kind, GcnCondition condition, GcnToken* match) :
		m_kind(kind),
		m_interpretType(GcnInterpretType::CustomCondition),
		m_condition(condition),
		m_match(match)
	{
	}

	GcnToken::~GcnToken()
	{
	}

	std::list<GcnToken*>::iterator GcnToken::getIterator()
	{
		return m_container->find(this);
	}

	GcnToken* GcnToken::getPrevNode()
	{
		auto iter = getIterator();
		return *(--iter);
	}

	GcnToken* GcnToken::getNextNode()
	{
		auto iter = getIterator();
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