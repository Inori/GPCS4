#include "GcnStackifier.h"
#include <unordered_set>

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
				case GcnTokenKind::Condition:
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
		// LOG_DEBUG("process vertex %d", static_cast<size_t>(vtx));
		
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
		auto codeToken = m_factory.createCode(vtx);
		m_insertPtr    = m_tokens->insertAfter(m_insertPtr, codeToken);
		
		// Process the successors of this block
		// First, add the delayed node to the visit stack
		m_visitStack.emplace_back(vtx, StackElement::Delayed);

		// Then, create the successor tokens for the block (direct branch, an if/else/end chain)
		processCodeTerminator(codeToken, vtx);
		// Enqueue successors on the visit stack
		// (in reverse order so they are popped in the correct order)
		auto& term = m_cfg[vtx].terminator;

		for (int i = 0; i != term.successors.size(); ++i)
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
			auto condOp    = GcnToken::getConditionOp(m_cfg, vtx);
			// Why do we need to inverse the condition?
			// see comments in processVertex
			auto inverseOp = GcnToken::getInversePredicate(condOp);
			auto condition = m_factory.createCondition(inverseOp);
			auto ifToken   = m_factory.createIf(condition);
			auto ifPtr     = m_tokens->insertAfter(m_insertPtr, ifToken);

			auto elseToken = m_factory.createElse(ifToken);
			auto elsePtr   = m_tokens->insertAfter(ifPtr, elseToken);

			auto endToken = m_factory.createIfEnd(ifToken, elseToken);
			auto endPtr   = m_tokens->insertAfter(elsePtr, endToken);

			auto ifVertex   = terminator.successors[1];
			auto elseVertex = terminator.successors[0];
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

		removeEmptyIfs();
		mergeBlocks();
		removeRedundantNesting();
		adjustLoopEnds();
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
			do 
			{
				// Look for branches that target Block Tokens
				GcnToken* oldEnd = branch->getMatch();
				if (oldEnd->kind() != GcnTokenKind::End)
				{
					break;
				}
				// If we have an outer Block Token that ends here, we can branch to that,
				// and remove the current one
				GcnToken* newEnd    = oldEnd;
				GcnToken* nextToken = newEnd->getNextNode();
				while (nextToken->kind() == GcnTokenKind::End &&
					   nextToken->getMatch()->kind() == GcnTokenKind::Block)
				{
					newEnd = nextToken;
				}

				if (newEnd == oldEnd)
				{
					break;
				}
				
				branch->setMatch(newEnd);

				GcnToken* oldBegin = oldEnd->getMatch();
				erase(oldBegin);
				erase(oldEnd); 
			} while (false);
		});
	}

	bool GcnTokenListOptimizer::canFallThrough(GcnToken* T)
	{
		LOG_ASSERT((uint32_t)T->kind() & ((uint32_t)GcnTokenKind::If | (uint32_t)GcnTokenKind::Else),
				   "Unexpeted test token.");

		bool result = false;
		do 
		{
			GcnToken* End  = T->getMatch();
			GcnToken* Last = End->getPrevNode();
			// If the if is empty, it does not matter that it technically falls through
			if (Last == T)
			{
				break;
			}

			switch (Last->kind())
			{
				case GcnTokenKind::Code:
				{
					auto vtx  = Last->getVertex();
					auto term = m_cfg[vtx].terminator;
					result    = term.kind != GcnBlockTerminator::Sink;
				}
					break;
				case GcnTokenKind::Branch:
					result = false;
					break;
				case GcnTokenKind::End:
				case GcnTokenKind::Condition:
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
		m_tokens = &tokens;
		auto gotos = findGotos();
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
				case GcnTokenKind::Condition:
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

	//////////////////////////////////////////////////////////////////////////
	GcnStackifier::GcnStackifier(const GcnControlFlowGraph& cfg) :
		m_cfg(cfg),
		m_factory(m_pool),
		m_builder(cfg, m_factory),
		m_optimizer(cfg, m_factory),
		m_eliminator(cfg, m_factory)
	{
	}

	GcnStackifier::~GcnStackifier()
	{
	}

	GcnTokenList GcnStackifier::generate()
	{
		GcnTokenList tokenList;

		m_builder.build(tokenList);
		LOG_ASSERT(m_verifier.verify(tokenList), "token list not valid");

		m_optimizer.optimize(tokenList);
		LOG_ASSERT(m_verifier.verify(tokenList), "token list not valid");

		m_eliminator.eliminate(tokenList);
		LOG_ASSERT(m_verifier.verify(tokenList), "token list not valid");

		return tokenList;
	}


}  // namespace sce::gcn