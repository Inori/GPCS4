#include "GcnDominatorTree.h"

#include <boost/graph/dominator_tree.hpp>



namespace sce::gcn
{

	GcnDominatorTree::GcnDominatorTree(GcnControlFlowGraph& cfg):
		m_cfg(cfg),
		m_domVector(boost::num_vertices(m_cfg), GcnControlFlowGraph::null_vertex())
	{
		buildDominatorMap();
	}

	GcnDominatorTree::~GcnDominatorTree()
	{
	}

	bool GcnDominatorTree::dominates(GcnCfgVertex u, GcnCfgVertex v) const
	{
		bool result = false;

		// Post traverse the dom tree from v to entry,
		// if we can find a node equal to u, then u dominates v.
		auto node = v;
		while (node != GcnControlFlowGraph::null_vertex())
		{
			// Note that a vertex dominates itself by definition.
			if (u == node)
			{
				result = true;
				break;
			}

			// Get immediate dominator of node
			node = m_domMap[node];
		}
		return result;
	}

	void GcnDominatorTree::buildDominatorMap()
	{
		const IndexMap indexMap = boost::get(boost::vertex_index, m_cfg);

		m_domMap =
			boost::make_iterator_property_map(m_domVector.begin(), indexMap);
		
		boost::lengauer_tarjan_dominator_tree(m_cfg, boost::vertex(0, m_cfg), m_domMap);
	}

}  // namespace sce::gcn