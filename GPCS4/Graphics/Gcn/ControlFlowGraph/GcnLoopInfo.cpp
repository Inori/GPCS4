#include "GcnLoopInfo.h"

LOG_CHANNEL(Graphic.Gcn.GcnLoopInfo);

// A workaround for boost bug.
// See https://github.com/boostorg/graph/issues/182
namespace boost
{
	template <typename G> 
	inline void renumber_vertex_indices(G& g) { throw; }
}  // namespace boost

namespace sce::gcn
{
	GcnLoop::GcnLoop()
	{
	}

	GcnLoop::~GcnLoop()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	GcnLoopInfo::HeaderDetector::HeaderDetector(std::unordered_set<GcnCfgVertex>& headers):
		m_headers(headers)
	{
	}

	void GcnLoopInfo::HeaderDetector::back_edge(GcnCfgEdge edge, const GcnControlFlowGraph& g)
	{
		auto target = boost::target(edge, g);
		m_headers.insert(target);
	}

	GcnLoopInfo::LoopVisitor::LoopVisitor(
		const std::unordered_set<GcnCfgVertex>& headers,
		std::vector<GcnLoop>&                   loops):
		m_headers(headers),
		m_loops(loops)
	{
	}

	void GcnLoopInfo::LoopVisitor::discover_vertex(GcnCfgVertex v, const GcnControlFlowGraph& g)
	{
		if (m_headers.find(v) != m_headers.end())
		{
			// create loop
			auto& loop = m_loops.emplace_back(GcnLoop());
			// add header
			loop.m_vertices.push_back(v);

			// set parent and child
			if (!m_loopStack.empty())
			{
				auto parent = m_loopStack.back();
				parent->m_children.push_back(&loop);
				loop.m_parent = parent;
				// also add the header to parent loop
				// so that the parent is a complete circle
				parent->m_vertices.push_back(v);
			}

			// push onto the stack
			m_loopStack.push_back(&loop);

			// there is a problem for boost DFS,
			// the back_edge callback for a 
			// self-loop vertex will not be called
			// in DFS order, instead it will be called
			// after all other non-self-loop back edges
			// are visited, this cause some inconvenience.
			// see https://github.com/boostorg/graph/issues/295
			// 
			// a simple solution is never push self-loop 
			// onto the stack.
			// so here we check if the vertex is a self-loop vertex,
			// then pop it right now as if it is never pushed
			if (isSelfLoop(v, g))
			{
				m_loopStack.pop_back();
			}
		}
		else
		{
			if (!m_loopStack.empty())
			{
				auto loop = m_loopStack.back();
				loop->m_vertices.push_back(v);
			}
		}
	}

	void GcnLoopInfo::LoopVisitor::back_edge(GcnCfgEdge edge, const GcnControlFlowGraph& g)
	{
		auto target = boost::target(edge, g);
		LOG_ASSERT(m_loopStack.empty() || target == m_loopStack.back()->getHeader(), "loop stack not match.");
		// self-loop vertex loop is already popped.
		if (!isSelfLoop(target, g))
		{
			m_loopStack.pop_back();
		}
	}

	bool GcnLoopInfo::LoopVisitor::isSelfLoop(GcnCfgVertex v, const GcnControlFlowGraph& g)
	{
		bool selfLoop = false;
		for (const auto& edge : boost::make_iterator_range(boost::out_edges(v, g)))
		{
			auto target = boost::target(edge, g);
			if (target == v)
			{
				selfLoop = true;
				break;
			}
		}
		return selfLoop;
	}

	GcnLoopInfo::GcnLoopInfo(const GcnControlFlowGraph& cfg) :
		m_cfg(cfg)
	{
		detectLoops();
		detectVertexMaping();
	}

	GcnLoopInfo::~GcnLoopInfo()
	{
	}

	GcnLoop* GcnLoopInfo::getLoop(GcnCfgVertex vertex) const
	{
		GcnLoop* loop = nullptr;
		auto     iter = m_vertexMap.find(vertex);
		if (iter != m_vertexMap.end())
		{
			loop = iter->second;
		}
		return loop;
	}

	bool GcnLoopInfo::isLoopHeader(GcnCfgVertex vtx) const
	{
		auto loop = getLoop(vtx);
		return (loop != nullptr) && 
			   (loop->getHeader() == vtx);
	}

	std::unordered_set<GcnCfgVertex> GcnLoopInfo::findLoopHeaders()
	{
		std::unordered_set<GcnCfgVertex> loopHeaders;
		HeaderDetector                   detector(loopHeaders);
		boost::depth_first_search(m_cfg, boost::visitor(detector));
		return loopHeaders;
	}


	void GcnLoopInfo::detectLoops()
	{
		// Collect loop information in two steps:
		// 1. find all loop headers in the first traverse, so that we know
		//    where a loop starts during the second traverse.
		// 2. build a loop scope stack,
		//    create the loop when we meet a loop header and insert all 
		//    vertex in the loop
		
		// find all loop headers
		auto headers = findLoopHeaders();

		// create loop infos
		LoopVisitor vis(headers, m_loops);
		// Note this reserve is critical,
		// it makes sure the vector will not reallocate
		// memory, such that we can safely save
		// pointers to the element in the vector.
		m_loops.reserve(headers.size());
		// do the DFS
		boost::depth_first_search(m_cfg, boost::visitor(vis));
	}

	void GcnLoopInfo::detectVertexMaping()
	{
		// Generate the mapping of vertex to the inner most loop they occur in
		for (const auto& vertex : boost::make_iterator_range(boost::vertices(m_cfg)))
		{
			auto loop = findLoop(vertex);
			m_vertexMap.insert(std::make_pair(vertex, loop));
		}
	}

	GcnLoop* GcnLoopInfo::findLoop(GcnCfgVertex vtx)
	{
		GcnLoop* innerMostLoop = nullptr;
		for (auto& loop : m_loops)
		{
			if (!loop.contains(vtx))
			{
				continue;
			}

			innerMostLoop = findInnerLoop(&loop, vtx);
			break;
		}
		return innerMostLoop;
	}

	GcnLoop* GcnLoopInfo::findInnerLoop(GcnLoop* loop, GcnCfgVertex vtx)
	{
		GcnLoop* result = nullptr;
		if (loop->m_children.empty())
		{
			result = loop;
		}
		else
		{
			bool foundChild = false;
			for (auto& child : loop->m_children)
			{
				if (!child->contains(vtx))
				{
					continue;
				}
				else
				{
					result = findInnerLoop(child, vtx);
					foundChild  = true;
					break;
				}
			}

			if (!foundChild)
			{
				result = loop;
			}
		}
		return result;
	}

}  // namespace sce::gcn