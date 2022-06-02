#include "GcnLoopInfo.h"

#include <boost/graph/tiernan_all_cycles.hpp>

// A workaround for boost bug.
// See https://github.com/boostorg/graph/issues/182
namespace boost
{
	template <typename G> 
	inline void renumber_vertex_indices(G& g) { throw; }
}  // namespace boost

namespace sce::gcn
{
	GcnLoop::GcnLoop(const std::vector<GcnCfgVertex>& path):
		m_vertices(path)
	{
	}

	GcnLoop::~GcnLoop()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	GcnLoopInfo::LoopVisitor::LoopVisitor(std::vector<GcnLoop>& loops) :
		m_loops(loops)
	{
	}

	template <typename Path, typename Graph>
	void GcnLoopInfo::LoopVisitor::cycle(const Path& path, const Graph& g) const
	{
		// path is of type std::vector<vertex_descriptor>
		m_loops.emplace_back(path);
	}

	GcnLoopInfo::GcnLoopInfo(const GcnControlFlowGraph& cfg) :
		m_cfg(cfg)
	{
		detectLoops();
		detectLoopNesting();
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

	void GcnLoopInfo::detectLoops()
	{
		LoopVisitor visitor(m_loops);
		boost::tiernan_all_cycles(m_cfg, visitor);
	}

	void GcnLoopInfo::detectLoopNesting()
	{
		for (auto& loop : m_loops)
		{
			const auto& loopHeader = loop.m_vertices.front();
			for (auto& anotherloop : m_loops)
			{
				if (loop == anotherloop)
				{
					continue;
				}
				// If a loop's header is a vertex of another loop,
				// then this loop is the child of that loop.
				auto iter = std::find(
					anotherloop.m_vertices.begin(), 
					anotherloop.m_vertices.end(), 
					loopHeader);
				if (iter != anotherloop.m_vertices.end())
				{
					loop.m_parent       = &anotherloop;
					anotherloop.m_children.push_back(&loop);
					break;
				}
			}
		}
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