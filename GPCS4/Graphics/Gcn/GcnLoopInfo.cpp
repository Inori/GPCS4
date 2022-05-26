#include "GcnLoopInfo.h"

#include <boost/graph/tiernan_all_cycles.hpp>

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

	GcnLoopInfo::GcnLoopInfo(GcnControlFlowGraph& cfg) :
		m_cfg(cfg)
	{
		detectLoops();
		detectLoopDomination();
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

	void GcnLoopInfo::detectLoopDomination()
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
				// If a loop's header is a vertex of an another loop,
				// then this loop is the child of that loop.
				auto iter = std::find(
					anotherloop.m_vertices.begin(), 
					anotherloop.m_vertices.end(), 
					loopHeader);
				if (iter != anotherloop.m_vertices.end())
				{
					loop.m_parent       = &anotherloop;
					anotherloop.m_child = &loop;
					break;
				}
			}
		}
	}

	void GcnLoopInfo::detectVertexMaping()
	{
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
		if (loop->m_child == nullptr || !loop->m_child->contains(vtx))
		{
			result = loop;
		}
		else
		{
			result = findInnerLoop(loop->m_child, vtx);
		}
		return result;
	}

}  // namespace sce::gcn