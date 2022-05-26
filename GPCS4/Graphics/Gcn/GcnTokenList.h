#pragma once

#include "GcnCommon.h"
#include "GcnControlFlowGraph.h"
#include "UtilObjectBank.h"

#include <list>

namespace sce::gcn
{
	enum class GcnTokenKind : uint32_t
	{
		Invalid   = 0,
		Code      = 1 << 0,
		Loop      = 1 << 1,
		Block     = 1 << 2,
		If        = 1 << 3,
		IfNot     = 1 << 4,
		Else      = 1 << 5,
		Branch    = 1 << 6,
		End       = 1 << 7,
		Condition = 1 << 8,
	};

	class GcnToken
	{
		friend class GcnTokenFactory;
	public:
		GcnToken(GcnTokenKind kind,
				 GcnCfgVertex vertex,
				 GcnToken*    match);
		~GcnToken();

	private:
		GcnTokenKind m_kind;
		GcnCfgVertex m_vertex;
		// A related token, for example,
		// the match of a If token is End
		GcnToken* m_match;
	};

	class GcnTokenFactory
	{
	public:
		GcnTokenFactory(
			util::ObjectBank<GcnToken>& tokenPool);
		~GcnTokenFactory();

		GcnToken* createCode(GcnCfgVertex vertex)
		{
			return m_pool.allocate(GcnTokenKind::Code, vertex, nullptr);
		}

		GcnToken* createLoop()
		{
			return m_pool.allocate(
				GcnTokenKind::Loop, GcnControlFlowGraph::null_vertex(), nullptr);
		}

		GcnToken* createLoopEnd(GcnToken* loopBegin)
		{
			assert(loopBegin && loopBegin->m_kind == GcnTokenKind::Loop);
			auto loopEnd       = m_pool.allocate(
				GcnTokenKind::End, GcnControlFlowGraph::null_vertex(), loopBegin);
			loopBegin->m_match = loopEnd;
			return loopEnd;
		}

		GcnToken* createBlock()
		{
			return m_pool.allocate(
				GcnTokenKind::Block, GcnControlFlowGraph::null_vertex(), nullptr);
		}

		GcnToken* createBlockEnd(GcnToken* blockBegin)
		{
			assert(blockBegin && blockBegin->m_kind == GcnTokenKind::Block);
			auto blockEnd       = m_pool.allocate(
				GcnTokenKind::End, GcnControlFlowGraph::null_vertex(), blockBegin);
			blockBegin->m_match = blockEnd;
			return blockEnd;
		}

		GcnToken* createIf(GcnCfgVertex condition)
		{
			return m_pool.allocate(GcnTokenKind::If, condition, nullptr);
		}

		GcnToken* createIfNot(GcnCfgVertex condition)
		{
			return m_pool.allocate(GcnTokenKind::IfNot, condition, nullptr);
		}

		GcnToken* createElse(GcnToken* tokenIf)
		{
			assert(!tokenIf || tokenIf->m_kind == GcnTokenKind::If);
			auto tokenElse = m_pool.allocate(
				GcnTokenKind::Else, GcnControlFlowGraph::null_vertex(), nullptr);
			if (tokenIf)
			{
				tokenIf->m_match = tokenElse;
			}
			return tokenElse;
		}

		GcnToken* createIfEnd(GcnToken* tokenIf, GcnToken* tokenElse)
		{
			assert(!tokenIf || tokenIf->m_kind == GcnTokenKind::If);
			assert(!tokenElse || tokenElse->m_kind == GcnTokenKind::Else);
			assert(tokenIf || tokenElse);

			auto tokenEnd = m_pool.allocate(
				GcnTokenKind::End, GcnControlFlowGraph::null_vertex(), tokenIf);
			if (tokenElse)
			{
				tokenElse->m_match = tokenEnd;
			}
			else
			{
				tokenIf->m_match = tokenEnd;
			}

			return tokenEnd;
		}

		GcnToken* createBranch(GcnToken* target)
		{
			return m_pool.allocate(
				GcnTokenKind::Branch, GcnControlFlowGraph::null_vertex(), target);
		}

		GcnToken* createCondition(GcnCfgVertex condition)
		{
			return m_pool.allocate(GcnTokenKind::Condition, condition, nullptr);
		}

	private:
		util::ObjectBank<GcnToken>& m_pool;
	};


	class GcnTokenList
	{
		typedef std::list<GcnToken*> TokenListType;
	public:
		GcnTokenList();
		~GcnTokenList();

	public:
		/// Token iterators...
		typedef TokenListType::iterator               iterator;
		typedef TokenListType::const_iterator         const_iterator;
		typedef TokenListType::reverse_iterator       reverse_iterator;
		typedef TokenListType::const_reverse_iterator const_reverse_iterator;

		/// Token iterator methods
		// clang-format off
		inline iterator                begin()       { return m_list.begin(); }
		inline const_iterator          begin() const { return m_list.begin(); }
		inline iterator                end  ()       { return m_list.end();   }
		inline const_iterator          end  () const { return m_list.end();   }

		inline reverse_iterator        rbegin()       { return m_list.rbegin(); }
		inline const_reverse_iterator  rbegin() const { return m_list.rbegin(); }
		inline reverse_iterator        rend  ()       { return m_list.rend();   }
		inline const_reverse_iterator  rend  () const { return m_list.rend();   }

		inline size_t                   size() const { return m_list.size();  }
		inline bool                    empty() const { return m_list.empty(); }
		inline const GcnToken      &front() const { return *m_list.front(); }
		inline       GcnToken      &front()       { return *m_list.front(); }
		inline const GcnToken       &back() const { return *m_list.back();  }
		inline       GcnToken       &back()       { return *m_list.back();  }
		// clang-format on

		void append(GcnToken* token)
		{
			m_list.push_back(token);
		}

		iterator insert(iterator where, GcnToken* token)
		{
			return m_list.insert(where, token);
		}

		iterator insertAfter(iterator where, GcnToken* token)
		{
			iterator iter = {};
			if (empty())
			{
				iter = m_list.insert(begin(), token);
			}
			else
			{
				iter = m_list.insert(++where, token);
			}
			return iter;
		}

		void moveAfter(iterator where, iterator first, iterator last)
		{
			return m_list.splice(std::next(where), m_list, first, last);
		}

		void erase(GcnToken* token)
		{
			std::erase(m_list, token);
		}

	private:
		std::list<GcnToken*> m_list;
	};




}  // namespace sce::gcn