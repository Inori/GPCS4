#include "GcnDivergentFlow.h"
#include "Gcn/GcnInstructionUtil.h"

namespace sce::gcn
{
	GcnDivergentFlow::GcnDivergentFlow(GcnTokenFactory& factory) :
		m_factory(factory)
	{
	}

	GcnDivergentFlow::~GcnDivergentFlow()
	{
	}

	void GcnDivergentFlow::diverge(GcnTokenList& tokens)
	{
		m_tokens = &tokens;

		// make a copy of code tokens.
		// because we are going to insert
		// new code tokens to the token list,
		// iterating through the original token
		// list will result in infinite loop.
		std::vector<GcnToken*> codeTokens;
		codeTokens.reserve(tokens.size() / 3);

		for (auto& token : tokens)
		{
			if (token->kind() != GcnTokenKind::Code)
			{
				continue;
			}
			codeTokens.push_back(token);
		}

		for (auto& code : codeTokens)
		{
			if (!needDiverge(code))
			{
				continue;
			}

			divergeCode(code);
		}
	}

	bool GcnDivergentFlow::needDiverge(GcnToken* token)
	{
		bool        allExecute = true;
		const auto& insList    = token->getCode().insList;
		
		for (const auto& ins : insList)
		{
			auto dvAction = getDivergentAction(ins);
			allExecute &= (dvAction == GcnDivergentAction::Execute);
		}

		return !allExecute;
	}

	void GcnDivergentFlow::divergeCode(GcnToken* token)
	{
		// There's should be at least two ways to
		// implement branch divergent:
		// 
		// 1. Create a new block which contains
		// only instructions that will execute
		// when thread is not active. The original
		// block is executed when thread is active.
		// This will produce many repeated instructions,
		// but reduce the count of ifs. I think this will
		// result in better performance.
		//
		// 2. Split the original instruction list into
		// many small groups, each with same executing
		// strategy. (execute or not execute)
		// Then bracket them using ifs.
		// This will produce many ifs, but no repeating
		// instructions. I think this is better for
		// debugging purpose.
		//
		// TODO:
		// The best should be implement both of them
		// and choose one of them for proper situation.
		// By now I only implement the first.

		auto& code           = token->getCode();
		auto& insList        = code.insList;
		auto  zeroScalarInst = makeClearInstruction();

		// erase branch instruction,
		// it's no longer needed.
		const auto& lastInst = insList.back();
		if (isBranchInstruction(lastInst))
		{
			insList.pop_back();
		}

		GcnTokenCode inactiveCode = {};
		inactiveCode.vertexId     = code.vertexId;
		inactiveCode.insList.reserve(insList.size());

		for (const auto& ins : insList)
		{
			auto dvAction = getDivergentAction(ins);
			switch (dvAction)
			{
				case GcnDivergentAction::Nop:
					// simply skip 
					break;
				case GcnDivergentAction::ZeroScalar:
				{
					zeroScalarInst.dst[0] = ins.dst[1];
					inactiveCode.insList.push_back(zeroScalarInst);
				}
					break;
				case GcnDivergentAction::Execute:
					inactiveCode.insList.push_back(ins);
					break;
			}
		}

		auto tokenIf           = m_factory.createIf(GcnConditionOp::Divergence);
		auto tokenElse         = m_factory.createElse(tokenIf);
		auto tokenEnd          = m_factory.createIfEnd(tokenIf, tokenElse);

		if (!inactiveCode.insList.empty())
		{
			auto tokenInactiveCode = m_factory.createCode(std::move(inactiveCode));

			m_tokens->insert(token->getIterator(), tokenIf);
			auto ptr = m_tokens->insertAfter(token->getIterator(), tokenElse);
			ptr      = m_tokens->insertAfter(ptr, tokenInactiveCode);
			m_tokens->insertAfter(ptr, tokenEnd);
		}
		else
		{
			// when all instructions are nops when thread is not active,
			// we don't need to insert an else block.
			m_tokens->insert(token->getIterator(), tokenIf);
			m_tokens->insertAfter(token->getIterator(), tokenEnd);
			tokenIf->setMatch(tokenEnd);
		}
	}

	GcnShaderInstruction GcnDivergentFlow::makeClearInstruction()
	{
		// construct a 
		// s_mov_b64 s[0:1], 0
		// instruction.
		// the destination sgpr pair will be
		// changed by the caller to some
		// cost on instruction structure constructing.
		GcnShaderInstruction result = {};

		result.opcode = GcnOpcode::S_MOV_B64;
		result.length = 4;
		result.encoding = GcnInstEncoding::SOP1;
		result.opClass  = GcnInstClass::ScalarMov;
		result.category = GcnInstCategory::ScalarALU;
		result.control  = {};
		result.srcCount = 1;
		result.dstCount = 1;

		result.src[0].field          = GcnOperandField::ConstZero;
		result.src[0].type           = GcnScalarType::Uint64;
		result.src[0].inputModifier  = {};
		result.src[0].outputModifier = {};
		result.src[0].code           = 128;

		result.dst[0].field          = GcnOperandField::ScalarGPR;
		result.dst[0].type           = GcnScalarType::Uint64;
		result.dst[0].inputModifier  = {};
		result.dst[0].outputModifier = {};
		result.dst[0].code           = 0;

		return result;
	}

	GcnDivergentAction GcnDivergentFlow::getDivergentAction(const GcnShaderInstruction& ins)
	{
		GcnDivergentAction action = GcnDivergentAction::Execute;
		switch (ins.encoding)
		{
			case GcnInstEncoding::SOP1:
			case GcnInstEncoding::SOP2:
			case GcnInstEncoding::SOPP:
			case GcnInstEncoding::SOPC:
			case GcnInstEncoding::SOPK:
			case GcnInstEncoding::SMRD:
				action = GcnDivergentAction::Execute;
				break;
			case GcnInstEncoding::VOP1:
			case GcnInstEncoding::VOP2:
			case GcnInstEncoding::VOP3:
			case GcnInstEncoding::VOPC:
			case GcnInstEncoding::VINTRP:
			case GcnInstEncoding::MUBUF:
			case GcnInstEncoding::MTBUF:
			case GcnInstEncoding::MIMG:
			case GcnInstEncoding::DS:
			case GcnInstEncoding::EXP:
				action = GcnDivergentAction::Nop;
				break;
			default:
				break;
		}

		auto isVCmpInst = [](GcnOpcode op) 
		{
			return op >= GcnOpcode::V_CMP_F_F32 && op <= GcnOpcode::V_CMPX_T_U64;
		};

		// fix some exceptions
		auto op = ins.opcode;
		if (op == GcnOpcode::V_READFIRSTLANE_B32 ||
			op == GcnOpcode::V_READLANE_B32 ||
			op == GcnOpcode::V_WRITELANE_B32)
		{
			action = GcnDivergentAction::Execute;
		}
		else if (isVCmpInst(op) || isVop3BEncoding(op))  
		{
			// some VOP2 instructions using the same opcode as VOP3B
			// use VCC as the default SGPC pair,
			// so the condition should be fine.
			action = GcnDivergentAction::ZeroScalar;
		}

		return action;
	}



}  // namespace sce::gcn