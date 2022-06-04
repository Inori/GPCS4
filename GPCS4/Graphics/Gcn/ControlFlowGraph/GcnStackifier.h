#pragma once

#include "Gcn/GcnCommon.h"
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
		GcnTokenListBuilder(const GcnControlFlowGraph& cfg,
							GcnTokenFactory&           factory);
		~GcnTokenListBuilder();

		void build(GcnTokenList& tokens);

	private:
		void processVertex(GcnCfgVertex vtx);
		void enqueueSuccessor(GcnCfgVertex vtx, GcnCfgVertex succ);
		void processCodeTerminator(GcnToken* codeToken, GcnCfgVertex vtx);
		void processLoopScopes(GcnCfgVertex vtx);
		void processBlockScopes(const std::vector<GcnToken*>& branches);
		void popScopes(GcnCfgVertex vtx);

		GcnToken* makeCode(GcnCfgVertex vtx);
		GcnToken* makeCondition(GcnCfgVertex vtx);

		GcnTokenList::iterator findBlockBegin(GcnTokenList::iterator target,
											  GcnTokenList::iterator candidate);
		GcnCfgVertex           getUniqueForwardPredecessor(GcnCfgVertex vtx);
		bool                   isBackedge(GcnCfgVertex from, GcnCfgVertex to);
		uint32_t               getNumForwardPreds(GcnCfgVertex vtx);
	private:
		const GcnControlFlowGraph& m_cfg;
		GcnTokenFactory&           m_factory;

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
		GcnTokenListOptimizer(const GcnControlFlowGraph& cfg,
							  GcnTokenFactory&           factory);
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
		void removeRedundantNesting();
		void mergeBlocks();
		void adjustLoopEnds();
		void removeRedundantLoops();
		void removeRedundantBlocks();

		bool     canFallThrough(GcnToken* T);
		uint32_t blockLikeTokens();
		bool     isSinkToken(GcnToken* token);

	private:
		const GcnControlFlowGraph& m_cfg;
		GcnTokenFactory&           m_factory;
		GcnTokenList*              m_tokens = nullptr;
		// Global state used to track
		// if the callback removed the current iteration element
		std::vector<GcnTokenList::iterator> m_iterStack;
		std::vector<uint32_t>               m_removeFlagStack;
	};

	/**
	 * \brief Goto eliminator
	 * 
	 * Let me clarify some concepts here.
	 * 
	 * The original stackifier algorithm is designed for
	 * WebAssembly and Javascript, although there is no
	 * goto statement for them, both of which has the "goto" concept,
	 * WebAssembly has multi-depth br instruction and 
	 * Javascript has labeled break/continue. They can achieve the
	 * same effect as goto.
	 * 
	 * But in spirv-v and glsl world, we don't even have such method,
	 * so we must eliminate all "goto"s in the token list.
	 * 
	 * There is no Goto and Label token from original stackifier,
	 * we only have Branch token.
	 * 
	 * If a branch is only used to go out/continue current block,
	 * it's just a normal break/continue statement, not goto.
	 * If a branch is used to go out/continue blocks for more than one
	 * depth, then it is a goto. 
	 * The the branch's target is the label.
	 * 
	 * Previously, I think we can eliminate all gotos by only
	 * restructure the program without introducing new variable
	 * and repeating code.
	 * But soon I found it is impossible.
	 * See https://en.wikipedia.org/wiki/Structured_program_theorem
	 * In particular:
	 * "In 1973, S. Rao Kosaraju proved that it's possible to 
	 * avoid adding additional variables in structured programming, 
	 * as long as arbitrary-depth, multi-level breaks from loops are allowed."
	 * 
	 * Sadly, We don't have multi-level exit either, maybe we can
	 * wrap code in a function such that we can use early return
	 * to emulate multi-level exit, but I think the generated code
	 * would be unreadable, so I choose to use new variables.
	 * 
	 * The algorithm used here is from:
	 * Taming Control Flow: A Structured Approach to Eliminating Goto Statements. 
	 * Ana M. Erosa and Laurie J. Hendren
	 * http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.42.1485&rep=rep1&type=pdf
	 * 
	 * All notations used here keeps the same with the above paper.
	 *
	 * Luckily, the level(depth) of goto produced by stackifier 
	 * is always greater than the level of corresponding label,
	 * so we only need to perform outward-movement transformation.
	 * 
	 */
	class GcnGotoEliminator
	{
		struct GotoInfo
		{
			GcnToken* branch;        // the "goto" token, treat branch.match as label
			uint32_t  levelDistance; // level(goto) - level(label)
		};

		struct Scope
		{
			GcnToken* head;
			GcnToken* end;
		};

	public:
		GcnGotoEliminator(const GcnControlFlowGraph& cfg,
						  GcnTokenFactory&           factory);
		~GcnGotoEliminator();

		void eliminate(GcnTokenList& tokens);

	private:
		std::vector<GotoInfo> findGotos();

		void eliminateGoto(const GotoInfo& info);

		GcnToken* moveOutward(GcnToken* gotoToken);
		GcnToken* moveOutwardIf(GcnToken* gotoToken, const Scope& scope);
		GcnToken* moveOutwardLoop(GcnToken* gotoToken, const Scope& scope);

		bool      isBackGoto(GcnToken* gotoToken);
		bool      isInRange(GcnToken* begin,
							GcnToken* end,
							GcnToken* target);
		bool      isInRange(GcnTokenList::iterator begin,
							GcnTokenList::iterator end,
							GcnTokenList::iterator target);
		Scope     findOuterScope(GcnToken* gotoToken);
		GcnToken* findScopeEnd(GcnToken* target);


	private:
		const GcnControlFlowGraph& m_cfg;
		GcnTokenFactory&           m_factory;
		GcnTokenList*              m_tokens = nullptr;
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
	 */

	class GcnStackifier
	{
	public:
		GcnStackifier(
			const GcnControlFlowGraph& cfg);
		~GcnStackifier();

		GcnTokenList generate();

	private:
		bool isIrreducible();
		bool isMultiEntryLoop(
			const std::unordered_set<GcnCfgVertex>& loop);
	private:
		const GcnControlFlowGraph& m_cfg;

		util::ObjectBank<GcnToken> m_pool;
		GcnTokenFactory            m_factory;

		GcnTokenListVerifier  m_verifier;
		GcnTokenListBuilder   m_builder;
		GcnTokenListOptimizer m_optimizer;
		GcnGotoEliminator     m_eliminator;
	};

}  // namespace sce::gcn