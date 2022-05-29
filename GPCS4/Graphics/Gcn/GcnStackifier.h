#pragma once

#include "GcnCommon.h"
#include "GcnTokenList.h"
#include "GcnLoopInfo.h"
#include "GcnDominatorTree.h"

#include <vector>
#include <unordered_map>

namespace sce::gcn
{
	class GcnTokenListVerifier
	{
	public:
		GcnTokenListVerifier();

		bool verify(const GcnTokenList& tokens) const;

	private:
	};

	class GcnTokenListBuilder
	{
		struct StackElement
		{
			enum Kind
			{
				Immediate,
				Delayed
			};

			StackElement(GcnCfgVertex vtx, Kind kind):
				m_vertex(vtx),
				m_kind(kind)
			{
			}

			GcnCfgVertex m_vertex;
			Kind         m_kind;
		};

		struct Scope
		{
			enum Kind
			{
				Direct,
				If,
				Loop,
			};

			Kind                   m_kind;
			GcnCfgVertex           m_vertex;
			GcnTokenList::iterator m_endPtr;
			bool                   m_nested;
		};

	public:
		GcnTokenListBuilder(GcnControlFlowGraph& cfg,
							GcnTokenFactory&     factory);
		~GcnTokenListBuilder();

		void build(GcnTokenList& tokens);

	private:
		void processVertex(GcnCfgVertex vtx);
		void enqueueSuccessor(GcnCfgVertex vtx, GcnCfgVertex succ);
		void processCodeTerminator(GcnToken* codeToken, GcnCfgVertex vtx);
		void processLoopScopes(GcnCfgVertex vtx);
		void processBlockScopes(const std::vector<GcnToken*>& branches);
		void popScopes(GcnCfgVertex vtx);

		GcnTokenList::iterator findBlockBegin(GcnTokenList::iterator target,
											  GcnTokenList::iterator candidate);
		GcnCfgVertex           getUniqueForwardPredecessor(GcnCfgVertex vtx);
		bool                   isBackedge(GcnCfgVertex from, GcnCfgVertex to);
		uint32_t               getNumForwardPreds(GcnCfgVertex vtx);
	private:
		GcnControlFlowGraph& m_cfg;
		GcnTokenFactory&     m_factory;

		GcnLoopInfo      m_loopInfo;
		GcnDominatorTree m_domTree;

		GcnTokenList*          m_tokens = nullptr;
		GcnTokenList::iterator m_insertPtr;

		std::unordered_map<GcnCfgVertex, uint32_t>                  m_visitedCounts;
		std::unordered_map<GcnCfgVertex, std::vector<GcnCfgVertex>> m_delayedQueues;
		std::unordered_map<const GcnLoop*, uint32_t>                m_loopCounts;
		std::unordered_map<GcnCfgVertex, GcnToken*>                 m_loopHeaders;
		std::unordered_map<GcnCfgVertex, std::vector<GcnToken*>>    m_blockScopes;
		std::vector<Scope>                                          m_scopeStack;
		std::vector<StackElement>                                   m_visitStack;
	};

	class GcnTokenListOptimizer
	{
	public:
		GcnTokenListOptimizer(GcnTokenFactory& factory);
		~GcnTokenListOptimizer();

		void optimize(GcnTokenList& tokens);

	private:
		template <uint32_t Kind, typename FnType>
		void for_each_kind(FnType func);
	
		template <uint32_t Kind, typename FnType>
		void for_each_kind(GcnTokenList::iterator begin,
						   GcnTokenList::iterator end,
						   FnType                 func);

		void erase(GcnToken* token);

		void removeEmptyIfs();
		void mergeBlocks();

	private:
		GcnTokenFactory& m_factory;
		GcnTokenList*    m_tokens = nullptr;
		// Global state used to track 
		// if the callback removed the current iteration element
		std::vector<GcnTokenList::iterator> m_iterStack;
		std::vector<uint32_t>               m_removeFlagStack;
	};

	/**
	 * \brief The stackifier algorithm
	 * 
	 * Build structured control flow graph
	 * from unstructured CFG for the shader.
	 * 
	 * Borrowed from Cheerp, originally designed
	 * by WebAssembly backend from LLVM project.
	 * 
	 * https://medium.com/leaningtech/solving-the-structured-control-flow-problem-once-and-for-all-5123117b1ee2
	 * https://github.com/leaningtech/cheerp-compiler/blob/cheerp-2.7/llvm/lib/CheerpWriter/CFGStackifier.cpp
	 *
	 * Well, to be honest, I don't really understand this algorithm. Orz.
	 */

	class GcnStackifier
	{
	public:
		GcnStackifier(GcnControlFlowGraph& cfg);
		~GcnStackifier();

		GcnTokenList generate();

	private:
		GcnControlFlowGraph& m_cfg;

		util::ObjectBank<GcnToken> m_pool;
		GcnTokenFactory            m_factory;

		GcnTokenListVerifier  m_verifier;
		GcnTokenListBuilder   m_builder;
		GcnTokenListOptimizer m_optimizer;
	};

}  // namespace sce::gcn