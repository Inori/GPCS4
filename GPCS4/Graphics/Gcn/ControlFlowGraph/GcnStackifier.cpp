#include "GcnStackifier.h"
#include <unordered_set>

#include <boost/graph/strong_components.hpp>
#include <iostream>

LOG_CHANNEL(Graphic.Gcn.GcnStackifier);

namespace sce::gcn
{
	GcnTokenListVerifier::GcnTokenListVerifier()
	{
	}

	bool GcnTokenListVerifier::verify(const GcnTokenList& tokens) const
	{
		std::vector<const GcnToken*>        scopeStack;
		std::unordered_set<const GcnToken*> activeScopes;

		for (const GcnToken* token : tokens)
		{
			switch (token->kind())
			{
				case GcnTokenKind::Code:
					break;
				case GcnTokenKind::Loop:
				case GcnTokenKind::Block:
				case GcnTokenKind::If:
				case GcnTokenKind::IfNot:
				{
					scopeStack.push_back(token);
					activeScopes.insert(token);
				}
					break;
				case GcnTokenKind::Else:
				{
					if (scopeStack.empty())
					{
						LOG_ERR("Scope stack empty but ELSE Token found");
						return false;
					}
					if (token->getMatch()->kind() != GcnTokenKind::End)
					{
						LOG_ERR("Match for ELSE Token is not a END Token");
						return false;
					}
					if (token->getMatch()->getMatch()->kind() != GcnTokenKind::If)
					{
						LOG_ERR("Match for END after ELSE Token is not a IF Token");
						return false;
					}
					const GcnToken* match = scopeStack.back();
					if (match->getMatch() != token)
					{
						LOG_ERR("ELSE Token is not the match of the current Token in the stack");
						return false;
					}
					break;
				}
				case GcnTokenKind::End:
				{
					if (scopeStack.empty())
					{
						LOG_ERR("Scope stack empty but END Token found");
						return false;
					}
					const GcnToken* match = scopeStack.back();
					if (match != token->getMatch())
					{
						LOG_ERR("Top Token in the stack is not the match for current END Token:");
						LOG_ERR("Current: %s", token->dump().c_str());
						LOG_ERR("Top: %s", match->dump().c_str());
						return false;
					}
					scopeStack.pop_back();
					activeScopes.erase(match);
					break;
				}
				case GcnTokenKind::Branch:
				{
					const GcnToken* match = token->getMatch()->kind() == GcnTokenKind::End
											 ? token->getMatch()->getMatch()
											 : token->getMatch();
					if (!activeScopes.count(match))
					{
						LOG_ERR("BRANCH Token is jumping to a non-active scope");
						return false;
					}
					break;
				}
				case GcnTokenKind::Variable:
				case GcnTokenKind::SetValue:
					break;
				case GcnTokenKind::Invalid:
					LOG_ERR("INVALID Token found");
					return false;
					break;
			}
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	GcnTokenListBuilder::GcnTokenListBuilder(
		const GcnControlFlowGraph& cfg,
		GcnTokenFactory&           factory) :
		m_cfg(cfg),
		m_factory(factory),
		m_loopInfo(cfg),
		m_domTree(cfg)
	{
	}

	GcnTokenListBuilder::~GcnTokenListBuilder()
	{
	}

	void GcnTokenListBuilder::build(GcnTokenList& tokens)
	{
		m_insertPtr = tokens.begin();
		m_tokens    = &tokens;

		auto entry = boost::vertex(0, m_cfg);
		
		m_visitedCounts[entry] = 0;
		m_visitStack.emplace_back(entry, StackElement::Delayed);
		m_visitStack.emplace_back(entry, StackElement::Immediate);

		while (!m_visitStack.empty())
		{
			auto& element = m_visitStack.back();
			if (element.m_kind == StackElement::Immediate)
			{
				m_visitStack.pop_back();
				processVertex(element.m_vertex);
			}
			else
			{
				auto& delayedVertices = m_delayedQueues[element.m_vertex];
				if (delayedVertices.empty())
				{
					m_visitStack.pop_back();
					continue;
				}

				auto vtx = delayedVertices.back();
				delayedVertices.pop_back();
				processVertex(vtx);
			}
		}

		popScopes(entry);

		LOG_ASSERT(m_scopeStack.empty(), "scope not all closed.");
	}

	void GcnTokenListBuilder::processVertex(GcnCfgVertex vtx)
	{
		//LOG_DEBUG("process vertex %d", static_cast<size_t>(vtx));
		
		// Check if we need to close some loop and/or if/else scopes
		popScopes(vtx);

		// If CurBB is the target of one or more breaks, instantiate the blocks now
		auto iter = m_blockScopes.find(vtx);
		if (iter != m_blockScopes.end())
		{
			const auto& branches = iter->second;
			processBlockScopes(branches);
			m_blockScopes.erase(vtx);
		}

		// Update the loop information and start a new one if needed
		processLoopScopes(vtx);

		// Create the token for this code block and add it to the list
		auto codeToken = makeCode(vtx);
		m_insertPtr    = m_tokens->insertAfter(m_insertPtr, codeToken);
		
		// Process the successors of this block
		// First, add the delayed node to the visit stack
		m_visitStack.emplace_back(vtx, StackElement::Delayed);

		// Then, create the successor tokens for the block (direct branch, an if/else/end chain)
		processCodeTerminator(codeToken, vtx);
		// Enqueue successors on the visit stack
		// (in reverse order so they are popped in the correct order)
		auto& term = m_cfg[vtx].terminator;
		for (int i = term.successors.size() - 1; i >= 0; --i)
		{
			enqueueSuccessor(vtx, term.successors[i]);
		}
	}

	void GcnTokenListBuilder::enqueueSuccessor(GcnCfgVertex vtx, GcnCfgVertex succ)
	{
		do
		{
			// Ignore backedges
			if (isBackedge(vtx, succ))
			{
				break;
			}

			// Check if it is ready to visit
			uint32_t succForwardPredCount = getNumForwardPreds(succ);
			if (++m_visitedCounts[succ] != succForwardPredCount)
			{
				break;
			}

			// If succ is a branch, add it to the visit stack
			if (getUniqueForwardPredecessor(succ) != GcnControlFlowGraph::null_vertex())
			{
				m_visitStack.emplace_back(succ, StackElement::Immediate);
				break;
			}

			// Otherwise, add it to to the delayed list
			auto idom = m_domTree.getImmDominator(succ);
			m_delayedQueues[idom].push_back(succ);
		} while (false);
	}

	void GcnTokenListBuilder::processCodeTerminator(GcnToken* codeToken, GcnCfgVertex vtx)
	{
		LOG_ASSERT(codeToken->kind() == GcnTokenKind::Code, "token is not code.");
		auto& terminator = m_cfg[vtx].terminator;
		if (terminator.kind == GcnBlockTerminator::Unconditional)
		{
			auto  succ        = terminator.successors[0];
			bool  nested      = (vtx == getUniqueForwardPredecessor(succ));
			Scope directScope = { Scope::Direct, succ, m_tokens->end(), nested };
			m_scopeStack.push_back(directScope);
		}
		else if (terminator.kind == GcnBlockTerminator::Conditional)
		{
			auto ifToken   = makeIf(vtx);
			auto ifPtr     = m_tokens->insertAfter(m_insertPtr, ifToken);

			auto elseToken = m_factory.createElse(ifToken);
			auto elsePtr   = m_tokens->insertAfter(ifPtr, elseToken);

			auto endToken = m_factory.createIfEnd(ifToken, elseToken);
			auto endPtr   = m_tokens->insertAfter(elsePtr, endToken);

			auto ifVertex   = terminator.successors[0];
			auto elseVertex = terminator.successors[1];
			bool ifNested   = (vtx == getUniqueForwardPredecessor(ifVertex));
			bool elseNested = (vtx == getUniqueForwardPredecessor(elseVertex));

			m_insertPtr = ifPtr;

			Scope ifScope   = { Scope::If, ifVertex, elsePtr, ifNested };
			Scope elseScope = { Scope::If, elseVertex, endPtr, elseNested };
			m_scopeStack.push_back(elseScope);
			m_scopeStack.push_back(ifScope);
		}
	}

	void GcnTokenListBuilder::processLoopScopes(GcnCfgVertex vtx)
	{
		do 
		{
			const auto loop = m_loopInfo.getLoop(vtx);
			if (!loop)
			{
				break;
			}

			if (loop->getHeader() == vtx)
			{
				auto loopToken    = m_factory.createLoop();
				auto loopEndToken = m_factory.createLoopEnd(loopToken);

				auto loopPtr = m_tokens->insertAfter(m_insertPtr, loopToken);
				auto endPtr  = m_tokens->insertAfter(loopPtr, loopEndToken);
				m_insertPtr  = loopPtr;

				Scope loopScope = { Scope::Loop, vtx, endPtr, true };
				m_scopeStack.push_back(loopScope);

				m_loopCounts.insert(std::make_pair(loop, loop->getNumVertices()));
				m_loopHeaders.insert(std::make_pair(vtx, loopToken));
			}

			// Decrement the loop count
			auto iter = m_loopCounts.find(loop);
			LOG_ASSERT(iter != m_loopCounts.end(), "loop not recorded.");
			--iter->second;

			// If the loop has parent, 
			// then loop header is one of the vertices of parent loop,
			// decrement parent by one when count is 0.
			while (iter->second == 0)
			{
				auto parent = iter->first->getParentLoop();
				iter        = m_loopCounts.find(parent);
				if (iter == m_loopCounts.end())
				{
					break;
				}
				--iter->second;
			}
	
		} while (false);
	}

	void GcnTokenListBuilder::processBlockScopes(const std::vector<GcnToken*>& branches)
	{
		// We will insert all the ends at this point
		auto endPtr = m_insertPtr;
		LOG_ASSERT(endPtr != m_tokens->end(), "reach token list end.");

		// We will resume the normal insertion after all the end blocks
		++m_insertPtr;
		for (auto& branch : branches)
		{
			LOG_ASSERT(branch->kind() == GcnTokenKind::Branch, "branch token kind is not branch");

			auto block    = m_factory.createBlock();
			auto blockEnd = m_factory.createBlockEnd(block);

			branch->setMatch(blockEnd);

			auto targetPtr = m_tokens->insertAfter(endPtr, blockEnd);
			auto blockPtr  = findBlockBegin(targetPtr, branch->getIterator());
			m_tokens->insert(blockPtr, block);
		}
		--m_insertPtr;
	}

	void GcnTokenListBuilder::popScopes(GcnCfgVertex vtx)
	{
		// Check if we need to close some scopes
		while (!m_scopeStack.empty())
		{
			auto& scope = m_scopeStack.back();
			switch (scope.m_kind)
			{
				case Scope::If:
				case Scope::Direct:
				{
					if (scope.m_nested && m_domTree.dominates(scope.m_vertex, vtx))
					{
						return;
					}
					
					// If the scope is not nested, add the branch token
					if (!scope.m_nested)
					{
						auto      iter   = m_loopHeaders.find(scope.m_vertex);
						GcnToken* branch = iter == m_loopHeaders.end()
											   ? m_factory.createBranch(nullptr)
											   : m_factory.createBranch(iter->second);

						m_blockScopes[scope.m_vertex].push_back(branch);
						m_insertPtr = m_tokens->insertAfter(m_insertPtr, branch);
					}
				}
					break;
				case Scope::Loop:
				{
					const auto loop = m_loopInfo.getLoop(scope.m_vertex);
					auto       iter = m_loopCounts.find(loop);
					LOG_ASSERT(iter != m_loopCounts.end(), "can't find loop.");
					uint32_t countLeft = iter->second;
					if (countLeft > 0)
					{
						return;
					}
				}
					break;
			}

			if (scope.m_endPtr != m_tokens->end())
			{
				m_insertPtr = scope.m_endPtr;
			}
		
			m_scopeStack.pop_back();
		}
	}

	GcnToken* GcnTokenListBuilder::makeCode(GcnCfgVertex vtx)
	{
		const auto& block = m_cfg[vtx];

		GcnTokenCode code;
		code.vertexId = vtx;
		code.pc       = block.pcBegin;
		code.insList  = block.insList;
		return m_factory.createCode(std::move(code));
	}

	GcnToken* GcnTokenListBuilder::makeIf(GcnCfgVertex vtx)
	{
		const auto& block    = m_cfg[vtx];
		const auto& lastInst = block.insList.back();
		auto        op       = GcnToken::getConditionOp(lastInst);
		return m_factory.createIf(op);
	}

	GcnTokenList::iterator GcnTokenListBuilder::findBlockBegin(
		GcnTokenList::iterator target, GcnTokenList::iterator candidate)
	{
		// Walk from the candidate to the target, and keep
		// track of the last closed scope we don't see opened
		GcnToken* lastUnmatchedScope = nullptr;
	
		for (auto it = candidate; it != target; ++it)
		{
			LOG_ASSERT(it != m_tokens->end(), "iterate reach end.");
			switch ((*it)->kind())
			{
				case GcnTokenKind::End:
					lastUnmatchedScope = (*it)->getMatch();
					break;
				case GcnTokenKind::Else:
					lastUnmatchedScope = (*it)->getMatch()->getMatch();
					it                 = (*it)->getMatch()->getIterator();
					break;
				case GcnTokenKind::If:
				case GcnTokenKind::Loop:
				case GcnTokenKind::Block:
					while ((*it)->kind() != GcnTokenKind::End)
						it = (*it)->getMatch()->getIterator();
					break;
				default:
					break;
			}
		}
		// If we have an unopened scope, move the candidate to it
		if (lastUnmatchedScope)
		{
			candidate = lastUnmatchedScope->getIterator();
		}
		
		return candidate;
	}

	GcnCfgVertex GcnTokenListBuilder::getUniqueForwardPredecessor(GcnCfgVertex vtx)
	{
		auto loop = m_loopInfo.isLoopHeader(vtx)
						? m_loopInfo.getLoop(vtx)
						: nullptr;

		auto uniquePred = GcnControlFlowGraph::null_vertex();
		const auto& predecessors = m_cfg[vtx].predecessors;
		for (const auto& pred : predecessors)
		{
			if (loop && loop->contains(pred))
			{
				continue;
			}

			if (uniquePred != GcnControlFlowGraph::null_vertex() &&
				uniquePred != pred)
			{
				return GcnControlFlowGraph::null_vertex();
			}

			uniquePred = pred;
		}
		return uniquePred;
	}

	bool GcnTokenListBuilder::isBackedge(GcnCfgVertex from, GcnCfgVertex to)
	{
		bool isToHeader  = m_loopInfo.isLoopHeader(to);
		bool containFrom = false;

		if (isToHeader)
		{
			auto loop   = m_loopInfo.getLoop(to);
			containFrom = loop->contains(from);
		}
	
		return isToHeader && containFrom;
	}

	uint32_t GcnTokenListBuilder::getNumForwardPreds(GcnCfgVertex vtx)
	{
		uint32_t    result       = 0;
		const auto& predecessors = m_cfg[vtx].predecessors;
		for (const auto& pred : predecessors)
		{
			if (isBackedge(pred, vtx))
			{
				continue;
			}
			++result;
		}
		return result;
	}




	//////////////////////////////////////////////////////////////////////////
	GcnTokenListOptimizer::GcnTokenListOptimizer(
		const GcnControlFlowGraph& cfg,
		GcnTokenFactory&           factory) :
		m_cfg(cfg),
		m_factory(factory)

	{
	}

	GcnTokenListOptimizer::~GcnTokenListOptimizer()
	{
	}

	void GcnTokenListOptimizer::optimize(GcnTokenList& tokens)
	{
		m_tokens = &tokens;

		removeRedundantLoops();
		removeEmptyIfs();
		mergeBlocks();
		removeRedundantNesting();
		adjustLoopEnds();
		removeRedundantBlocks();

		// TODO:
		// more optimizations.
		// there is a removeRedundantBranches in cheerp's implementation,
		// I'm not very clear about it, so didn't port to here.
	}

	template <uint32_t Kind, typename FnType>
	void GcnTokenListOptimizer::for_each_kind(FnType func)
	{
		for_each_kind<Kind>(m_tokens->begin(), m_tokens->end(), func);
	}

	template <uint32_t Kind, typename FnType>
	void GcnTokenListOptimizer::for_each_kind(
		GcnTokenList::iterator begin, GcnTokenList::iterator end, FnType func)
	{
		uint32_t depth = m_iterStack.size();

		m_iterStack.push_back(begin);
		m_removeFlagStack.push_back(false);

		GcnTokenList::iterator& iter      = m_iterStack[depth];
		uint32_t&               isRemoved = m_removeFlagStack[depth];
		while (iter != end)
		{
			if ( ((uint32_t)(*iter)->kind() & Kind) == 0)
			{
				++iter;
				continue;
			}

			func(*iter);

			if (isRemoved)
			{
				isRemoved = false;
			}
			else
			{
				++iter;
			}
		}

		m_iterStack.pop_back();
		m_removeFlagStack.pop_back();
	}

	void GcnTokenListOptimizer::erase(GcnToken* token)
	{
		uint32_t depth = m_iterStack.size();
		for (uint32_t i = 0; i != depth; ++i)
		{
			if (token->getIterator() == m_iterStack[i])
			{
				m_removeFlagStack[i] = true;
				++m_iterStack[i];
			}
		}
		m_tokens->erase(token);
	}

	void GcnTokenListOptimizer::removeEmptyIfs()
	{
		for_each_kind<(uint32_t)GcnTokenKind::Else | (uint32_t)GcnTokenKind::End>([&](GcnToken* token)
		{
			GcnToken* prev = token->getPrevNode();
			do 
			{
				if (prev->getMatch() != token)
				{
					break;
				}

				if ((uint32_t)prev->kind() & ((uint32_t)GcnTokenKind::If | (uint32_t)GcnTokenKind::IfNot))
				{
					GcnToken* emptyIf = prev;
					if (token->kind() == GcnTokenKind::Else)
					{
						/*
						 * if (cond)
						 * {}
						 * else
						 * {
						 *   statement
						 *   ...
						 * }
						 */
						GcnToken* tokenElse = token;
						GcnToken* tokenEnd  = tokenElse->getMatch();

						GcnToken* tokenIfNot = m_factory.createIfNot(emptyIf->getCondition());
						tokenIfNot->setMatch(tokenEnd);
						tokenEnd->setMatch(tokenIfNot);

						m_tokens->insertAfter(emptyIf->getIterator(), tokenIfNot);

						erase(emptyIf);
						erase(tokenElse);
					}
					else  // token kind is End
					{
						/*
						 * if (cond)
						 * {}
						 */
						erase(emptyIf);
						erase(token);
					}
					break;
				}

				if (prev->kind() == GcnTokenKind::Else)
				{
					/*
					 * if (cond)
					 * {
					 *   statement
					 *   ...
					 * }
					 * else
					 * {}
					 */
					LOG_ASSERT(token->kind() == GcnTokenKind::End, "token kind is not End");
					GcnToken* emptyElse = prev;
					GcnToken* tokenEnd  = token;
					GcnToken* tokenIf   = tokenEnd->getMatch();
					tokenIf->setMatch(tokenEnd);

					erase(emptyElse);

					break;
				}

			} while (false);
		});

	}

	void GcnTokenListOptimizer::mergeBlocks()
	{
		for_each_kind<(uint32_t)GcnTokenKind::Branch>([&](GcnToken* branch)
		{
			// Look for branches that target Block Tokens
			GcnToken* oldEnd = branch->getMatch();
			if (oldEnd->kind() != GcnTokenKind::End)
			{
				return;
			}

			if (oldEnd->getMatch()->kind() != GcnTokenKind::Block)
			{
				return;
			}
			// If we have an outer Block Token that ends here, we can branch to that,
			// and remove the current one
			GcnToken* newEnd = oldEnd;
			
			while (newEnd->getNextNode()->kind() == GcnTokenKind::End &&
				   newEnd->getNextNode()->getMatch()->kind() == GcnTokenKind::Block)
			{
				newEnd = newEnd->getNextNode();
				if (newEnd == nullptr || newEnd->getNextNode() == nullptr)
				{
					break;
				}
			}

			if (newEnd == oldEnd)
			{
				return;
			}
				
			branch->setMatch(newEnd);

			GcnToken* oldBegin = oldEnd->getMatch();
			erase(oldBegin);
			erase(oldEnd); 
		});
	}

	bool GcnTokenListOptimizer::canFallThrough(GcnToken* T)
	{
		LOG_ASSERT((uint32_t)T->kind() & ((uint32_t)GcnTokenKind::If | (uint32_t)GcnTokenKind::Else),
				   "Unexpeted test token.");

		bool result = false;
		do 
		{
			GcnToken* end  = T->getMatch();
			GcnToken* last = end->getPrevNode();
			// If the if is empty, it does not matter that it technically falls through
			if (last == T)
			{
				break;
			}

			switch (last->kind())
			{
				case GcnTokenKind::Code:
					result = !isSinkToken(last);
					break;
				case GcnTokenKind::Branch:
					result = false;
					break;
				case GcnTokenKind::End:
				case GcnTokenKind::Variable:
				case GcnTokenKind::SetValue:
					result = true;
					break;
				case GcnTokenKind::If:
				case GcnTokenKind::IfNot:
				case GcnTokenKind::Else:
				case GcnTokenKind::Loop:
				case GcnTokenKind::Block:
				case GcnTokenKind::Invalid:
				default:
					LOG_ASSERT(false, "Unexpeted token");
					break;
			}
			
		} while (false);
		return result;
	}

	void GcnTokenListOptimizer::removeRedundantNesting()
	{
		// Iterate the End Tokens, so we handle the innermost Ifs first
		for_each_kind<(uint32_t)GcnTokenKind::End>([&](GcnToken* tokenEnd)
		{
			// Find construct which has both if and else
			GcnToken* tokenIf = tokenEnd->getMatch();
			if (tokenIf->kind() != GcnTokenKind::If)
			{
				return;
			}
			GcnToken* tokenElse = tokenIf->getMatch();
			if (tokenElse->kind() != GcnTokenKind::Else)
			{
				return;
			}

			/*
			* Convert:
			* 
			* if (cond)
			* { somecode; }
			* else
			* { othercode; }
			* 
			* to
			* 
			* if (cond)
			* { somecode; }
			* othercode;
			*/
			auto removeElse = [&]()
			{
				GcnToken* newEnd = m_factory.createIfEnd(tokenIf, nullptr);
				m_tokens->insert(tokenElse->getIterator(), newEnd);
				erase(tokenElse);
				erase(tokenEnd);
			};

			/*
				* Convert:
				*
				* if (cond)
				* { somecode; }
				* else
				* { othercode; }
				*
				* to
				*
				* if (!cond)
				* { othercode; }
				* somecode;
				*/
			auto removeIf = [&]()
			{
				m_tokens->moveAfter(tokenEnd->getIterator(), std::next(tokenIf->getIterator()), tokenElse->getIterator());
				GcnToken* tokenIfNot = m_factory.createIfNot(tokenIf->getCondition());
				tokenIfNot->setMatch(tokenEnd);
				tokenEnd->setMatch(tokenIfNot);
				m_tokens->insert(tokenElse->getIterator(), tokenIfNot);
				erase(tokenIf);
				erase(tokenElse);
			};

			auto isBranchIf = [](GcnToken* T)
			{
				GcnToken* match = T->getMatch();
				GcnToken* inner = T->getNextNode();
				return inner->kind() == GcnTokenKind::Branch &&
						inner->getNextNode() == match;
			};

			// Candidates for removing
			bool elseIsCandidate = !canFallThrough(tokenIf);
			bool ifIsCandidate   = !canFallThrough(tokenElse);
			if (ifIsCandidate && elseIsCandidate)
			{
				// TODO: if both the If and the Else cannot fall through, decide which
				// one is s better to remove. Right now, we only look if one of the two
				// contains branch, and remove the other one
				if (isBranchIf(tokenIf))
					removeElse();
				else if (isBranchIf(tokenElse))
					removeIf();
				else
					removeElse();
			}
			else if (ifIsCandidate)
			{
				removeIf();
			}
			else if (elseIsCandidate)
			{
				removeElse();
			} 
		});
	}

	void GcnTokenListOptimizer::adjustLoopEnds()
	{
		for_each_kind<(uint32_t)GcnTokenKind::Loop>([&](GcnToken* Loop)
		{
			GcnToken* tokenEnd = Loop->getMatch();
			GcnToken* tokenCur = tokenEnd->getPrevNode();
			GcnToken* lastZeroDepth = nullptr;

			int depth = 0;
			while (tokenCur != Loop)
			{
				if ((uint32_t)tokenCur->kind() & ((uint32_t)GcnTokenKind::If |
												  (uint32_t)GcnTokenKind::IfNot |
												  (uint32_t)GcnTokenKind::Loop |
												  (uint32_t)GcnTokenKind::Block))
				{
					--depth;
				}
				else if (tokenCur->kind() == GcnTokenKind::End)
				{
					if (depth == 0)
					{
						lastZeroDepth = tokenCur;
					}
					++depth;
				}
				else if (tokenCur->kind() == GcnTokenKind::Branch && tokenCur->getMatch() == Loop)
				{
					break;
				}
				tokenCur = tokenCur->getPrevNode();
			}
			if (lastZeroDepth != nullptr && lastZeroDepth != tokenEnd->getPrevNode())
			{
				m_tokens->moveAfter(lastZeroDepth->getIterator(), tokenEnd->getIterator(), std::next(tokenEnd->getIterator()));
			} 
		});
	}

	void GcnTokenListOptimizer::removeRedundantLoops()
	{
		for_each_kind<(uint32_t)GcnTokenKind::Loop>([&](GcnToken* loop)
		{
			std::unordered_set<GcnToken*> extraLoops;
			auto iter = loop->getIterator();
			while ((*(++iter))->kind() == GcnTokenKind::Loop)
			{
				extraLoops.insert(*iter);
			}
			if (extraLoops.empty())
				return;
			GcnTokenList::iterator searchEnd = loop->getNextNode()->getMatch()->getIterator();
			assert((*searchEnd)->kind() == GcnTokenKind::End);
			for_each_kind<(uint32_t)GcnTokenKind::Branch>(iter, searchEnd, [&](GcnToken* branch)
			{
				if (extraLoops.count(branch->getMatch()))
				{
					branch->setMatch(loop);
				}
			});
			for (GcnToken* extraLoop : extraLoops)
			{
				GcnToken* loopEnd = extraLoop->getMatch();
				erase(loopEnd);
				erase(extraLoop);
			} 
		});
	}

	void GcnTokenListOptimizer::removeRedundantBlocks()
	{
		const uint32_t blockLikeToken = blockLikeTokens();

		for_each_kind<(uint32_t)GcnTokenKind::End>([&](GcnToken* blockEnd)
		{
			GcnToken* block = blockEnd->getMatch();
			if (block->kind() != GcnTokenKind::Block)
			{
				// skip non-block
				return;
			}

			GcnToken* inner = block->getNextNode();
			LOG_ASSERT(inner->kind() != GcnTokenKind::End, "empty block found.");
			if (!((uint32_t)inner->kind() & blockLikeToken))
			{
				return;
			}
			
			GcnToken* innerEnd = inner->getMatch();
			// mainly for if else end construct
			while (innerEnd->kind() != GcnTokenKind::End)
			{
				innerEnd = innerEnd->getMatch();
			}
			
			if (innerEnd->getNextNode() != blockEnd)
			{
				return;
			}
			
			for_each_kind<(uint32_t)GcnTokenKind::Branch>(block->getIterator(),
														  blockEnd->getIterator(),
			[&](GcnToken* branch)
			{
				if (branch->getMatch() == blockEnd)
				{
					if ((uint32_t)inner->kind() & ((uint32_t)GcnTokenKind::If|
												   (uint32_t)GcnTokenKind::IfNot))
					{
						return;
					}
					branch->setMatch(innerEnd);
				}
			});
			erase(block);
			erase(blockEnd); 
		});
	}

	uint32_t GcnTokenListOptimizer::blockLikeTokens()
	{
		return (uint32_t)GcnTokenKind::Block |
			   (uint32_t)GcnTokenKind::Loop |
			   (uint32_t)GcnTokenKind::If |
			   (uint32_t)GcnTokenKind::IfNot;
	}

	bool GcnTokenListOptimizer::isSinkToken(GcnToken* token)
	{
		LOG_ASSERT(token->kind() == GcnTokenKind::Code, "called on non-code tokens.");
		const auto& code     = token->getCode();
		const auto& lastInst = code.insList.back();
		return lastInst.opcode == GcnOpcode::S_ENDPGM;
	}

	//////////////////////////////////////////////////////////////////////////
	GcnGotoEliminator::GcnGotoEliminator(const GcnControlFlowGraph& cfg,
										 GcnTokenFactory&           factory) :
		m_cfg(cfg),
		m_factory(factory)
	{
	}

	GcnGotoEliminator::~GcnGotoEliminator()
	{
	}

	void GcnGotoEliminator::eliminate(GcnTokenList& tokens)
	{
		m_tokens   = &tokens;
		auto gotos = findGotos();

		for (const auto& info : gotos)
		{
			eliminateGoto(info);
		}
	}

	std::vector<GcnGotoEliminator::GotoInfo> 
		GcnGotoEliminator::findGotos()
	{
		std::vector<GotoInfo>        result;
		std::vector<const GcnToken*> scopeStack;
		
		for (const auto& token : *m_tokens)
		{
			switch (token->kind())
			{
				case GcnTokenKind::Code:
				case GcnTokenKind::If:
				case GcnTokenKind::IfNot:
				case GcnTokenKind::Else:
				case GcnTokenKind::Variable:
				case GcnTokenKind::SetValue:
					break;
				case GcnTokenKind::Block:
				case GcnTokenKind::Loop:
					scopeStack.push_back(token);
					break;
				case GcnTokenKind::Branch:
				{
					GcnToken* target = token->getMatch();
					if (target->kind() == GcnTokenKind::End)
					{
						// A block, replace with block header.
						target = target->getMatch();
					}
					
					auto iter     = std::find(scopeStack.rbegin(), scopeStack.rend(), target);
					auto distance = std::distance(scopeStack.rbegin(), iter);
					if (distance != 0)
					{
						GotoInfo info = { token, static_cast<uint32_t>(distance) };
						result.push_back(info);
					}
				}
					break;
				case GcnTokenKind::End:
				{
					auto headerKind = token->getMatch()->kind();
					if ((uint32_t)headerKind & ((uint32_t)GcnTokenKind::Loop |
												(uint32_t)GcnTokenKind::Block))
					{
						scopeStack.pop_back();
					}
				}
					break;
				case GcnTokenKind::Invalid:
					LOG_ASSERT(false, "Invalid token found");
					break;
			}
		}
		LOG_ASSERT(scopeStack.empty(), "scope not all closed.");
		return result;
	}

	void GcnGotoEliminator::eliminateGoto(const GotoInfo& info)
	{
		auto      distance  = info.levelDistance;
		GcnToken* gotoToken = info.branch;
		// We don't need to do inward-movement transformation
		while (distance != 0)
		{
			gotoToken = moveOutward(gotoToken);
			--distance;
		}

		// We don't need to do anything more,
		// because we don't need to resolve arbitrary gotos.
		// There will be only 2 kinds of gotos in our situation:
		// multi-level break from loops/blocks (labeled break in JavaScript)
		// multi-level continue for loops (labeled continue in JavaScript)
		// in both cases the code produced by outward-movement step
		// already fits.
	}

	GcnToken* GcnGotoEliminator::moveOutward(GcnToken* gotoToken)
	{
		// perform outward-movement transformation

		GcnToken* newGoto = nullptr;
		auto      scope   = findOuterScope(gotoToken);
		if ((uint32_t)scope.head->kind() & ((uint32_t)GcnTokenKind::If |
											(uint32_t)GcnTokenKind::IfNot |
											(uint32_t)GcnTokenKind::Else))
		{
			newGoto = moveOutwardIf(gotoToken, scope);
		}
		else if ((uint32_t)scope.head->kind() & ((uint32_t)GcnTokenKind::Loop |
												 (uint32_t)GcnTokenKind::Block))
		{
			// we use do {} while(false); to implement Block token,
			// so a Block is also a loop in general.
			newGoto = moveOutwardLoop(gotoToken, scope);
		}
		else
		{
			LOG_ASSERT(false, "expected goto scope.");
		}

		return newGoto;
	}

	GcnToken* GcnGotoEliminator::moveOutwardIf(GcnToken* gotoToken, const Scope& scope)
	{
		auto gotoEnd = findScopeEnd(gotoToken);
		// first, replace the original goto with a value set.
		
		// create a flag variable, initialize to false
		// all variables are global so no need to worry about redefinition.
		auto var = m_factory.createVariable({ 0, GcnScalarType::Bool });
		// set the variable to true, marking the goto happened
		auto setValue = m_factory.createSetValue({ 1, GcnScalarType::Bool }, var);
		auto ptr      = m_tokens->insert(gotoToken->getIterator(), var);
		ptr           = m_tokens->insertAfter(ptr, setValue);
		m_tokens->erase(gotoToken);

		// second, bracket the statement between [next(goto), prev[scope.end]]
		// with a inverse condition
		
		// create the inverse condition if
		auto tokenIf    = m_factory.createIf(GcnConditionOp::NeBool, 1, var);
		auto tokenIfEnd = m_factory.createIfEnd(tokenIf, nullptr);
		// bracket
		m_tokens->insertAfter(gotoEnd->getIterator(), tokenIf);
		m_tokens->insert(scope.end->getIterator(), tokenIfEnd);

		// third, add the original conditional goto at the scope end
		auto tokenIfOut    = m_factory.createIf(GcnConditionOp::EqBool, 1, var);
		auto tokenIfEndOut = m_factory.createIfEnd(tokenIfOut, nullptr);
		auto newGoto       = m_factory.createBranch(gotoToken->getMatch());
		auto resetVar      = m_factory.createSetValue({ 0, GcnScalarType::Bool }, var);

		auto wholeScopeEnd = findScopeEnd(scope.head);  // find the end of entire if else endif construct
		ptr                = m_tokens->insertAfter(wholeScopeEnd->getIterator(), tokenIfOut);
		ptr                = m_tokens->insertAfter(ptr, resetVar);  // deactive the goto condition
		ptr                = m_tokens->insertAfter(ptr, newGoto);
		m_tokens->insertAfter(ptr, tokenIfEndOut);

		return newGoto;
	}

	GcnToken* GcnGotoEliminator::moveOutwardLoop(GcnToken* gotoToken, const Scope& scope)
	{
		// first, replace the original goto with a value set and insert a break.

		// create a flag variable, initialize to false
		// all variables are global so no need to worry about redefinition.
		auto var = m_factory.createVariable({ 0, GcnScalarType::Bool });
		// set the variable to true, marking the goto happened
		auto setValue = m_factory.createSetValue({ 1, GcnScalarType::Bool }, var);
		// create the break
		auto branch = m_factory.createBranch(scope.end);
		// replace the old goto
		auto ptr = m_tokens->insert(gotoToken->getIterator(), var);
		ptr      = m_tokens->insertAfter(ptr, setValue);
		ptr      = m_tokens->insertAfter(ptr, branch);
		m_tokens->erase(gotoToken);

		// second, move the original conditional goto to the scope end
		auto tokenIf    = m_factory.createIf(GcnConditionOp::EqBool, 1, var);
		auto tokenIfEnd = m_factory.createIfEnd(tokenIf, nullptr);
		auto newGoto    = m_factory.createBranch(gotoToken->getMatch());
		auto resetVar   = m_factory.createSetValue({ 0, GcnScalarType::Bool }, var);

		ptr = m_tokens->insertAfter(scope.end->getIterator(), tokenIf);
		ptr = m_tokens->insertAfter(ptr, resetVar);  // deactive the goto condition
		ptr = m_tokens->insertAfter(ptr, newGoto);
		m_tokens->insertAfter(ptr, tokenIfEnd);

		return newGoto;
	}

	bool GcnGotoEliminator::isBackGoto(GcnToken* gotoToken)
	{
		auto label = gotoToken->getMatch();
		return label->kind() == GcnTokenKind::Loop;
	}

	bool GcnGotoEliminator::isInRange(
		GcnToken* begin,
		GcnToken* end,
		GcnToken* target)
	{
		return isInRange(begin->getIterator(),
						 end->getIterator(),
						 target->getIterator());
	}

	bool GcnGotoEliminator::isInRange(
		GcnTokenList::iterator begin,
		GcnTokenList::iterator end,
		GcnTokenList::iterator target)
	{
		LOG_ASSERT(end != m_tokens->end(), "end token is out of token list range.");

		bool inRange = false;
		auto postEnd = std::next(end);
		for (auto it = begin; it != postEnd; ++it)
		{
			if (it == m_tokens->end())
			{
				break;
			}

			if (target == it)
			{
				inRange = true;
				break;
			}
		}
		return inRange;
	}

	GcnGotoEliminator::Scope GcnGotoEliminator::findOuterScope(GcnToken* gotoToken)
	{
		// An outer scope could be Block, Loop, If/IfNot and Else
		Scope scope = {};

		auto end = findScopeEnd(gotoToken);
		LOG_ASSERT(end != nullptr, "can not find next end token");

		if ((uint32_t)end->getMatch()->kind() & ((uint32_t)GcnTokenKind::If |
												 (uint32_t)GcnTokenKind::IfNot))
		{
			// Skip the first if/else scope,
			// as it is a part of the condition goto.
			end = findScopeEnd(end->getNextNode());
		}

		auto head = end->getMatch();
		if (head->getMatch()->kind() == GcnTokenKind::Else)
		{
			if (isInRange(head,              // if
						  head->getMatch(),  // else
						  gotoToken))        // goto
			{
				// goto is within if and else
				// mark the end to else
				end = head->getMatch();
			}
			else
			{
				// goto is within else and end
				// mark the head to else
				head = head->getMatch();
			}
		}

		scope.head = head;
		scope.end  = end;
		return scope;
	}

	GcnToken* GcnGotoEliminator::findScopeEnd(GcnToken* target)
	{
		// find the end of inner most scope within which the target sits in.
		// if the target is scope head or end token, return the scope constructed by the head/end
		GcnToken* result = nullptr;

		auto it = target->getIterator();
		while (it != m_tokens->end())
		{
			if ((*it)->kind() == GcnTokenKind::End)
			{
				auto end  = *it;
				auto head = end->getMatch();
				if (isInRange(head, end, target))
				{
					result = end;
					break;
				}
			}
			++it;
		}
		return result;
	}


	//////////////////////////////////////////////////////////////////////////
	GcnStackifier::GcnStackifier(const GcnControlFlowGraph& cfg) :
		m_cfg(cfg),
		m_factory(m_pool),
		m_builder(cfg, m_factory),
		m_optimizer(cfg, m_factory),
		m_eliminator(cfg, m_factory),
		m_diverger(m_factory)
	{
	}

	GcnStackifier::~GcnStackifier()
	{
	}

	GcnTokenList GcnStackifier::generate()
	{
		// TODO:
		// A big todo though, stackifier algorithm doesn't support
		// irreducible cfg, we need to convert the irreducible
		// cfg to a reducible one manually by dispatch loops
		// using some variables.
		LOG_ASSERT(isIrreducible() == false, "TODO: support irreducible cfg.");

		GcnTokenList tokenList;

		m_builder.build(tokenList);
		LOG_ASSERT(m_verifier.verify(tokenList), "token list not valid");
		//LOG_DEBUG("%s", tokenList.dump().c_str());

		m_optimizer.optimize(tokenList);
		LOG_ASSERT(m_verifier.verify(tokenList), "token list not valid");
		//LOG_DEBUG("%s", tokenList.dump().c_str());

		m_eliminator.eliminate(tokenList);
		LOG_ASSERT(m_verifier.verify(tokenList), "token list not valid");

		m_diverger.diverge(tokenList);
		LOG_ASSERT(m_verifier.verify(tokenList), "token list not valid");

		return tokenList;
	}

	bool GcnStackifier::isIrreducible()
	{
		using SccLabel = GcnControlFlowGraph::vertices_size_type;
		const uint32_t vertexNum = boost::num_vertices(m_cfg);

		// calculate scc component map.
		std::vector<SccLabel> component(vertexNum);
		const auto&           indexMap     = boost::get(boost::vertex_index, m_cfg);
		auto                  componentMap = boost::make_iterator_property_map(component.begin(), indexMap);
		uint32_t              sccNum       = boost::strong_components(m_cfg, componentMap);

		// build a new scc map using scc label as the key.
		std::unordered_map<
			SccLabel, std::unordered_set<GcnCfgVertex>>
			sccMap;
		sccMap.reserve(sccNum);
		for (uint32_t i = 0; i != vertexNum; ++i)
		{
			SccLabel scc = component[i];
			sccMap[scc].insert(i);
		}
		
		// identify multiple-entry loops
		bool foundMultiEntryLoop = false;
		for (const auto& pair : sccMap)
		{
			const auto& scc = pair.second;
			if (scc.size() <= 1)
			{
				// skip non-loops
				// SCC with more than one element is loop
				continue;
			}

			if(isMultiEntryLoop(scc))
			{
				foundMultiEntryLoop = true;
				break;
			}
		}
		return foundMultiEntryLoop;
	}

	bool GcnStackifier::isMultiEntryLoop(const std::unordered_set<GcnCfgVertex>& loop)
	{
		// We identify all the loop headers,
		// which are the nodes reachable from outside of the SCC.
		// If there is more than one, 
		// we have a multiple-entry loop.
		uint32_t headerCount = 0;
		for (auto vtx : loop)
		{
			for (const auto& edge : boost::make_iterator_range(boost::in_edges(vtx, m_cfg)))
			{
				auto src = boost::source(edge, m_cfg);
				if (loop.find(src) == loop.end())
				{
					++headerCount;
					break;
				}
			}
		}
		return headerCount > 1;
	}

}  // namespace sce::gcn