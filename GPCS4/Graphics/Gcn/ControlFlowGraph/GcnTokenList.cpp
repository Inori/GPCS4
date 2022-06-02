#include "GcnTokenList.h"
#include "Gcn/GcnInstructionUtil.h"

LOG_CHANNEL(Graphic.Gcn.GcnTokenList);

namespace sce::gcn
{
	GcnToken::GcnToken(GcnTokenKind kind, GcnCfgVertex vertex, GcnToken* match) :
		m_kind(kind),
		m_vertex(vertex),
		m_match(match)
	{
	}

	GcnToken::GcnToken(GcnTokenKind kind, GcnToken* condition, GcnToken* match):
		m_kind(kind),
		m_condition(condition),
		m_match(match)
	{
	}

	GcnToken::GcnToken(GcnTokenKind kind, const GcnTokenValue& value, GcnToken* match) :
		m_kind(kind),
		m_value(value),
		m_match(match)
	{
	}

	GcnToken::GcnToken(GcnTokenKind kind, const GcnTokenCondition& cond, GcnToken* match) :
		m_kind(kind),
		m_conditionInfo(cond),
		m_match(match)
	{
	}

	GcnToken::~GcnToken()
	{
	}

	std::list<GcnToken*>::iterator GcnToken::getIterator()
	{
		return m_container->find(this);
	}

	GcnToken* GcnToken::getPrevNode()
	{
		auto iter = getIterator();
		return *(--iter);
	}

	GcnToken* GcnToken::getNextNode()
	{
		auto iter = getIterator();
		return *(++iter);
	}

	GcnConditionOp GcnToken::getConditionOp(const GcnControlFlowGraph& cfg,
											GcnCfgVertex               vtx)
	{
		const auto& block = cfg[vtx];
		const auto& lastInst = block.insList.back();
		LOG_ASSERT(isBranchInstruction(lastInst), "expect a conditional block.");
		auto op = lastInst.opcode;
		GcnConditionOp condOp;
		switch (op)
		{
			case GcnOpcode::S_CBRANCH_SCC0:
				condOp = GcnConditionOp::Scc0;
				break;
			case GcnOpcode::S_CBRANCH_SCC1:
				condOp = GcnConditionOp::Scc1;
				break;
			case GcnOpcode::S_CBRANCH_VCCZ:
				condOp = GcnConditionOp::Vccz;
				break;
			case GcnOpcode::S_CBRANCH_VCCNZ:
				condOp = GcnConditionOp::Vccnz;
				break;
			case GcnOpcode::S_CBRANCH_EXECZ:
				condOp = GcnConditionOp::Execz;
				break;
			case GcnOpcode::S_CBRANCH_EXECNZ:
				condOp = GcnConditionOp::Execnz;
				break;
		}
		return condOp;
	}

	std::string GcnToken::dump() const
	{
		auto conditionName = [](GcnToken* condition)
		{
			std::string name;
			switch (condition->m_conditionInfo.op)
			{
				case GcnConditionOp::EqBool: name = "EqBool"; break;
				case GcnConditionOp::NeBool: name = "NeBool"; break;
				case GcnConditionOp::EqU32: name = "EqU32"; break;
				case GcnConditionOp::NeU32: name = "NeU32"; break;
				case GcnConditionOp::GeU32: name = "GeU32"; break;
				case GcnConditionOp::GtU32: name = "GtU32"; break;
				case GcnConditionOp::LeU32: name = "LeU32"; break;
				case GcnConditionOp::LtU32: name = "LtU32"; break;
				case GcnConditionOp::Scc0: name = "Scc0"; break;
				case GcnConditionOp::Scc1: name = "Scc1"; break;
				case GcnConditionOp::Vccz: name = "Vccz"; break;
				case GcnConditionOp::Vccnz: name = "Vccnz"; break;
				case GcnConditionOp::Execz: name = "Execz"; break;
				case GcnConditionOp::Execnz: name = "Execnz"; break;
				case GcnConditionOp::Divergence: name = "Divergence"; break;
			}
			return name;
		};

		auto endTail = [](GcnToken* match) 
		{
			std::string tail;
			switch (match->m_kind)
			{
				case GcnTokenKind::Block:
					tail = "BLOCK";
					break;
				case GcnTokenKind::If:
				case GcnTokenKind::IfNot:
				case GcnTokenKind::Else:
					tail = "IF";
					break;
				case GcnTokenKind::Loop:
					tail = "LOOP";
					break;
				default:
					tail = "";
					break;
			}
			return tail;
		};

		auto vertexName = [](GcnCfgVertex vtx)
		{
			return vtx == GcnControlFlowGraph::null_vertex()
					   ? "null"
					   : fmt::format("{}", vtx);
		};

		std::stringstream ss;
		ss << fmt::format("{}", (void*)this) << " ";
		switch (m_kind)
		{
			case GcnTokenKind::Invalid:
				ss << "!!!!!INVALID!!!!!"
				   << "\n";
				break;
			case GcnTokenKind::Code:
				ss << "CODE "
				   << "V" << vertexName(m_vertex) << "\n";
				break;
			case GcnTokenKind::Loop:
				ss << "LOOP " << fmt::format("{}", (void*)m_match) << "\n";
				break;
			case GcnTokenKind::Block:
				ss << "BLOCK " << fmt::format("{}", (void*)m_match) << "\n";
				break;
			case GcnTokenKind::If:
				ss << "IF " << conditionName(m_condition) << "\n";
				break;
			case GcnTokenKind::IfNot:
				ss << "IF_NOT " << conditionName(m_condition) << "\n";
				break;
			case GcnTokenKind::Else:
				ss << "ELSE"
				   << "\n";
				break;
			case GcnTokenKind::Branch:
				ss << "BRANCH " << fmt::format("{}", (void*)m_match) << "\n";
				break;
			case GcnTokenKind::End:
				ss << "END" << endTail(m_match) << " " << fmt::format("{}", (void*)m_match) << "\n";
				break;
			case GcnTokenKind::Variable:
				ss << "VARIABLE " << m_value.value << "\n";
				break;
			case GcnTokenKind::SetValue:
				ss << "SET_VALUE " << m_value.value << "\n";
				break;
		}
		return ss.str();
	}

	GcnConditionOp GcnToken::getInversePredicate(GcnConditionOp op)
	{
		GcnConditionOp inverse = {};
		switch (op)
		{
			case GcnConditionOp::Scc0: inverse = GcnConditionOp::Scc1; break;
			case GcnConditionOp::Scc1: inverse = GcnConditionOp::Scc0; break;
			case GcnConditionOp::Vccz: inverse = GcnConditionOp::Vccnz; break;
			case GcnConditionOp::Vccnz: inverse = GcnConditionOp::Vccz; break;
			case GcnConditionOp::Execz: inverse = GcnConditionOp::Execnz; break;
			case GcnConditionOp::Execnz: inverse = GcnConditionOp::Execz; break;
		}
		return inverse;
	}

	////////////////////////////////////////////////////////////
	GcnTokenFactory::GcnTokenFactory(util::ObjectBank<GcnToken>& tokenPool):
		m_pool(tokenPool)
	{
	}

	GcnTokenFactory::~GcnTokenFactory()
	{
	}

	////////////////////////////////////////////////////////////
	GcnTokenList::GcnTokenList()
	{
	}

	GcnTokenList::~GcnTokenList()
	{
	}

}  // namespace sce::gcn