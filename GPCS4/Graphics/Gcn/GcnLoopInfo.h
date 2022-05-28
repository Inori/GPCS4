#pragma once

#include "GcnCommon.h"
#include "GcnControlFlowGraph.h"

#include <unordered_map>

namespace sce::gcn
{
	/**
	 * \brief Represents a loop in CFG
	 */
	class GcnLoop
	{
		friend class GcnLoopInfo;
	public:
		GcnLoop(
			const std::vector<GcnCfgVertex>& path);
		~GcnLoop();

		bool contains(GcnCfgVertex vtx) const
		{
			return std::find(
				m_vertices.begin(), m_vertices.end(), vtx) !=
				m_vertices.end();
		}

		GcnCfgVertex getHeader() const
		{
			return m_vertices.front();
		}

		uint32_t getNumVertices() const
		{
			return m_vertices.size();
		}

		GcnLoop* getParentLoop() const
		{
			return m_parent;
		}

		bool operator==(const GcnLoop& other) const
		{
			return m_vertices == other.m_vertices;
		}

	private:
		std::vector<GcnCfgVertex> m_vertices;
		GcnLoop*                  m_parent = nullptr;
		std::vector<GcnLoop*>     m_children;
	};

	/**
	 * \brief Loop collection of the CFG
	 */
	class GcnLoopInfo
	{
		class LoopVisitor
		{
		public:
			LoopVisitor(std::vector<GcnLoop>& loops);

			template <typename Path, typename Graph>
			void cycle(const Path& path, const Graph& g) const;

		private:
			std::vector<GcnLoop>& m_loops;
		};

	public:
		GcnLoopInfo(GcnControlFlowGraph& cfg);
		~GcnLoopInfo();

		/**
		 * \brief Get loop for vertex
		 * 
		 * Return the inner most loop that vertex lives in. If a vertex is in no
		 * loop (for example the entry node), null is returned.
		 */
		GcnLoop* getLoop(GcnCfgVertex vertex) const;

		bool isLoopHeader(GcnCfgVertex vtx) const;

	private:
		void detectLoops();
		void detectLoopNesting();
		void detectVertexMaping();

		GcnLoop* findLoop(GcnCfgVertex vtx);
		GcnLoop* findInnerLoop(GcnLoop* loop, GcnCfgVertex vtx);
	private:
		GcnControlFlowGraph& m_cfg;
		std::vector<GcnLoop> m_loops;

		// Mapping of vertex to the inner most loop they occur in
		std::unordered_map<GcnCfgVertex, GcnLoop*> m_vertexMap;
	};
}  // namespace sce::gcn