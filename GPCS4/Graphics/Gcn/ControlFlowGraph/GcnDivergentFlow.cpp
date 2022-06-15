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
		// Split the original instruction list into
		// many small groups, each with same executing
		// strategy. (divergent action)
		// Then bracket them using ifs.

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

		// record the insert position
		auto insertPtr = token->getIterator();

		// Instruction group
		// instructions in a same group have same divergent action
		struct InstGroup
		{
			uint32_t           pc;
			GcnInstructionList insList;
		};

		InstGroup lastGroup;
		InstGroup lastZsGroup;  // last zero scalar group
		GcnToken* lastIf = nullptr;

		auto insertCodeToken = [&](InstGroup&& group) 
		{
			if (group.insList.empty())
			{
				return;
			}

			GcnTokenCode newCode = {};
			newCode.vertexId     = code.vertexId;
			newCode.pc           = group.pc;
			newCode.insList      = std::move(group.insList);
			auto codeToken       = m_factory.createCode(std::move(newCode));
			insertPtr            = m_tokens->insertAfter(insertPtr, codeToken);
		};

		auto resetPc = [](InstGroup& group, uint32_t pc) 
		{
			if (group.insList.empty())
			{
				group.pc = pc;
			}
		};

		auto openScope = [&]() 
		{
			// clear old group
			insertCodeToken(std::move(lastGroup));
			// insert if token
			auto tokenIf = m_factory.createIf(GcnConditionOp::Divergence);
			insertPtr    = m_tokens->insertAfter(insertPtr, tokenIf);
			return tokenIf;
		};

		auto closeScope = [&](GcnToken* tokenIf) 
		{
			// insert if code
			insertCodeToken(std::move(lastGroup));
			// insert else code if exist
			GcnToken* tokenElse = nullptr;
			if (!lastZsGroup.insList.empty())
			{
				tokenElse = m_factory.createElse(tokenIf);
				insertPtr = m_tokens->insertAfter(insertPtr, tokenElse);
				insertCodeToken(std::move(lastZsGroup));
			}
			auto tokenEnd = m_factory.createIfEnd(tokenIf, tokenElse);
			insertPtr     = m_tokens->insertAfter(insertPtr, tokenEnd);
		};

		bool     open = false;
		uint32_t pc   = code.pc;

		for (const auto& ins : insList)
		{
			if (open && isNonCompileInst(ins))
			{
				// this is an optimization,
				// this will merge two if scope
				// if there are only non-compile instructions
				// between them while keeping the pc
				// value of each code token correct
				insertCodeToken(std::move(lastGroup));
				pc += ins.length;
				continue;
			}

			auto dvAction = getDivergentAction(ins);
			switch (dvAction)
			{
				case GcnDivergentAction::Nop:
				case GcnDivergentAction::ZeroScalar:
				{
					if (!open)
					{
						// open if scope
						lastIf = openScope();

						resetPc(lastGroup, pc);
						lastGroup.insList.push_back(ins);
						if (dvAction == GcnDivergentAction::ZeroScalar)
						{
							resetPc(lastZsGroup, pc);
							zeroScalarInst.dst[0] = ins.dst[1];
							lastZsGroup.insList.push_back(zeroScalarInst);
						}

						open = true;
					}
					else
					{
						lastGroup.insList.push_back(ins);
						if (dvAction == GcnDivergentAction::ZeroScalar)
						{
							resetPc(lastZsGroup, pc);
							zeroScalarInst.dst[0] = ins.dst[1];
							lastZsGroup.insList.push_back(zeroScalarInst);
						}
					}
				}
					break;
				case GcnDivergentAction::Execute:
				{
					if (open)
					{
						// close if scope
						closeScope(lastIf);

						resetPc(lastGroup, pc);
						lastGroup.insList.push_back(ins);
						open = false;
					}
					else
					{
						resetPc(lastGroup, pc);
						lastGroup.insList.push_back(ins);
					}
				}
					break;
			}

			// advance pc
			pc += ins.length;
		}

		if (open)
		{
			closeScope(lastIf);
			open = false;
		}

		if (!lastGroup.insList.empty())
		{
			insertCodeToken(std::move(lastGroup));
		}

		m_tokens->erase(token);
	}

	GcnShaderInstruction GcnDivergentFlow::makeClearInstruction()
	{
		// construct a 
		// s_mov_b64 s[0:1], 0
		// instruction.
		// the destination sgpr pair of the result will be
		// replaced by the caller to save some
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
			// use VCC as the default SGPR pair,
			// so the condition should be fine.
			action = GcnDivergentAction::ZeroScalar;
		}
		else if (op == GcnOpcode::EXP)
		{
			// treat exp instruction as always execute,
			// we need this information to write to color output
			// or kill the pixel no matter what exec value is.
			action = GcnDivergentAction::Execute;
		}

		return action;
	}

	bool GcnDivergentFlow::isNonCompileInst(const GcnShaderInstruction& ins)
	{
		// return true if the instruction is not going to
		// be compiled by compiler
		return ins.opcode == GcnOpcode::S_WAITCNT ||
			   ins.opcode == GcnOpcode::S_NOP;
	}


}  // namespace sce::gcn