#include "GcnDominatorTree.h"

#include <boost/graph/dominator_tree.hpp>

namespace sce::gcn
{

	GcnDominatorTree::GcnDominatorTree(GcnControlFlowGraph& cfg):
		m_cfg(cfg)
	{
		buildDominatorMap();
	}

	GcnDominatorTree::~GcnDominatorTree()
	{
	}

	void GcnDominatorTree::buildDominatorMap()
	{
		const IndexMap indexMap = boost::get(boost::vertex_index, m_cfg);

		m_domVector =
			std::vector<GcnCfgVertex>(boost::num_vertices(m_cfg), GcnControlFlowGraph::null_vertex());

		m_domMap =
			boost::make_iterator_property_map(m_domVector.begin(), indexMap);

		boost::lengauer_tarjan_dominator_tree(m_cfg, boost::vertex(0, m_cfg), m_domMap);
	}

}  // namespace sce::gcn