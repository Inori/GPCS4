#pragma once

#include "Gcn/GcnCommon.h"
#include "GcnControlFlowGraph.h"

#include <unordered_map>
#include <boost/graph/depth_first_search.hpp>

namespace sce::gcn
{
	/**
	 * \brief Represents a loop in CFG
	 */
	class GcnLoop
	{
		friend class GcnLoopInfo;
	public:
		GcnLoop();
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
		class HeaderDetector : public boost::dfs_visitor<>
		{
		public:
			HeaderDetector(std::unordered_set<GcnCfgVertex>& headers);

			void back_edge(GcnCfgEdge edge, const GcnControlFlowGraph& g);

		private:
			std::unordered_set<GcnCfgVertex>& m_headers;
		};

		class LoopVisitor : public boost::dfs_visitor<>
		{
		public:
			LoopVisitor(const std::unordered_set<GcnCfgVertex>& headers,
						std::vector<GcnLoop>&                   loops);

			void discover_vertex(GcnCfgVertex v, const GcnControlFlowGraph& g);

			void back_edge(GcnCfgEdge edge, const GcnControlFlowGraph& g);

		private:
			bool isSelfLoop(GcnCfgVertex v, const GcnControlFlowGraph& g);

		private:
			const std::unordered_set<GcnCfgVertex>& m_headers;
			std::vector<GcnLoop>&                   m_loops;
			std::vector<GcnLoop*>                   m_loopStack;
		};

	public:
		GcnLoopInfo(const GcnControlFlowGraph& cfg);
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
		void detectVertexMaping();

		std::unordered_set<GcnCfgVertex>
		findLoopHeaders();

		GcnLoop* findLoop(GcnCfgVertex vtx);
		GcnLoop* findInnerLoop(GcnLoop* loop, GcnCfgVertex vtx);
	private:
		const GcnControlFlowGraph& m_cfg;
		std::vector<GcnLoop>       m_loops;

		// Mapping of vertex to the inner most loop they occur in
		std::unordered_map<GcnCfgVertex, GcnLoop*> m_vertexMap;
	};
}  // namespace sce::gcn