#pragma once

#include "Gcn/GcnCommon.h"
#include "GcnControlFlowGraph.h"

#include <vector>
#include <boost/property_map/property_map.hpp>

namespace sce::gcn
{

    class GcnDominatorTree
	{
		typedef boost::property_map<
			GcnControlFlowGraph,
			boost::vertex_index_t>::type
			IndexMap;
		typedef boost::iterator_property_map<
			std::vector<GcnCfgVertex>::iterator,
			IndexMap>
			DomTreePredMap;

	public:
		GcnDominatorTree(
			const GcnControlFlowGraph& cfg);
		~GcnDominatorTree();

		/**
		 * \brief Detect if u dominate v
		 *
		 * Note that a vertex dominate itself
		 * by definition.
		 */
		bool dominates(
			GcnCfgVertex u, GcnCfgVertex v) const;

		/**
		 * \brief Get immediate dominator.
		 */
		GcnCfgVertex
		getImmDominator(GcnCfgVertex vtx);

	private:
		void buildDominatorMap();

	private:
		const GcnControlFlowGraph& m_cfg;
		DomTreePredMap             m_domMap;
		std::vector<GcnCfgVertex>  m_domVector;
	};

}  // namespace sce::gcn