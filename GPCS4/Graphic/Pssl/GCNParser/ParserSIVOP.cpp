#include "ParserSIVOP.h"


VOPInstruction::SRC ParserSIVOP::GetSRC(Instruction::instruction64bit hexInstruction, unsigned int& ridx, unsigned int idxSRC)
{
	VOPInstruction::SRC src = (VOPInstruction::SRC)0;
	switch (idxSRC)
	{
	case 0:
		src	= static_cast<VOPInstruction::SRC>(hexInstruction & static_cast<Instruction::instruction32bit>(VOPMask_SRC0));
		break;
	case 1:
		src = static_cast<VOPInstruction::SRC>((hexInstruction & static_cast<Instruction::instruction32bit>(VOP3Mask_SRC1)) >> 9);
		break;
	case 2:
		src = static_cast<VOPInstruction::SRC>((hexInstruction & static_cast<Instruction::instruction32bit>(VOP3Mask_SRC2)) >> 18);
		break;
	default:
		break;
	}

#define X_RANGE(FIELD_MIN,FIELD_MAX,FIELD,IN,VAL)\
    if ((IN >= VOPInstruction::SRC##FIELD_MIN) && (IN <= VOPInstruction::SRC##FIELD_MAX)) \
    { \
        VAL = IN; \
        return VOPInstruction::SRC##FIELD; \
    }
#define X(FIELD,IN) \
    if (IN == VOPInstruction::SRC##FIELD) \
    { \
        return VOPInstruction::SRC##FIELD; \
    }
	GENERIC_INSTRUCTION_FIELDS_1(src, ridx);
#undef X
#undef X_RANGE

#define X_RANGE(FIELD_MIN,FIELD_MAX,FIELD,IN)\
    if ((IN >= VOPInstruction::SRC##FIELD_MIN) && (IN <= VOPInstruction::SRC##FIELD_MAX)) \
    { \
        return VOPInstruction::SRC##FIELD; \
    }
#define X(FIELD,IN) \
    if (IN == VOPInstruction::SRC##FIELD) \
    { \
        return VOPInstruction::SRC##FIELD; \
    }
	SCALAR_INSTRUCTION_FIELDS(src);
	GENERIC_INSTRUCTION_FIELDS_2(src);
#undef X
#undef X_RANGE

	// VGPR
	if (src >= 256 && src <= 511)
	{
		ridx = src - 256;
		return src;
	}

	return VOPInstruction::SRCIllegal;
}


VOPInstruction::VSRC ParserSIVOP::GetVSRC1(Instruction::instruction64bit hexInstruction, unsigned int& ridx)
{
	// Only VOPC and VOP2 have vsrc
	VOPInstruction::VSRC vsrc1 = static_cast<VOPInstruction::VSRC>((hexInstruction & static_cast<Instruction::instruction32bit>(VOPMask_VSRC1)) >> 9);
	ridx = vsrc1;
	return vsrc1;
}


VOPInstruction::VDST ParserSIVOP::GetVDST(Instruction::instruction64bit hexInstruction, unsigned int& ridx, VOPInstruction::Encoding enc)
{
	VOPInstruction::VDST vdst = 0;
	switch (enc)
	{
	case VOPInstruction::Encoding_VOP1:
	case VOPInstruction::Encoding_VOP2:
		vdst = static_cast<VOPInstruction::VDST>((hexInstruction & static_cast<Instruction::instruction32bit>(VOP1Mask_VDST)) >> 17);
		break;

	case VOPInstruction::Encoding_VOP3:
		vdst = static_cast<VOPInstruction::VDST>((hexInstruction & static_cast<Instruction::instruction64bit>(VOP3Mask_VDST)) >> 32);
		break;
	case VOPInstruction::Encoding_VOP3P:
	case VOPInstruction::Encoding_Illegal:
	default:
		break;
	}

	ridx = vdst;
	return vdst;
}


// Only for VOP3B
VOPInstruction::SDST ParserSIVOP::GetSDST(Instruction::instruction64bit hexInstruction, unsigned int& ridx)
{
	VOPInstruction::SDST sdst = static_cast<VOPInstruction::SDST>((hexInstruction & static_cast<Instruction::instruction64bit>(VOP3Mask_SDST)) >> 40);

#define X_RANGE(FIELD_MIN,FIELD_MAX,FIELD,IN,VAL)\
    if ((IN >= VOPInstruction::SDST##FIELD_MIN) && (IN <= VOPInstruction::SDST##FIELD_MAX)) \
    { \
        VAL = IN; \
        return VOPInstruction::SDST##FIELD; \
    }
#define X(FIELD,IN) \
    if (IN == VOPInstruction::SDST##FIELD) \
    { \
        return VOPInstruction::SDST##FIELD; \
    }
	GENERIC_INSTRUCTION_FIELDS_1(sdst, ridx);
#undef X
#undef X_RANGE

	return VOPInstruction::SDSTIllegal;
}


VOPInstruction::OMOD ParserSIVOP::GetOMOD(Instruction::instruction64bit hexInstruction)
{
	VOPInstruction::OMOD omod = static_cast<VOPInstruction::OMOD>((hexInstruction & static_cast<Instruction::instruction64bit>(VOP3Mask_OMOD)) >> 27);
	return omod;
}


VOPInstruction::NEG ParserSIVOP::GetNEG(Instruction::instruction64bit hexInstruction)
{
	VOPInstruction::NEG neg = static_cast<VOPInstruction::NEG>((hexInstruction & static_cast<Instruction::instruction64bit>(VOP3Mask_NEG)) >> 29);
	return neg;
}


VOPInstruction::ABS ParserSIVOP::GetABS(Instruction::instruction64bit hexInstruction)
{
	VOPInstruction::ABS abs = static_cast<VOPInstruction::ABS>((hexInstruction & static_cast<Instruction::instruction64bit>(VOP3Mask_ABS)) >> 40);
	return abs;
}


VOPInstruction::CLMP ParserSIVOP::GetCLMP(Instruction::instruction64bit hexInstruction)
{
	VOPInstruction::CLMP clmp = static_cast<VOPInstruction::CLMP>((hexInstruction & static_cast<Instruction::instruction64bit>(VOP3Mask_CLMP)) >> 43);
	return clmp;
}


ParserSI::kaStatus ParserSIVOP::Parse(GDT_HW_GENERATION hwGen, Instruction::instruction32bit hexInstruction, std::unique_ptr<Instruction>& instruction, bool& hasLiteral)
{
    kaStatus retStatus =   ParserSI::Status_32BitInstructionNotSupported;
    VOPInstruction::Encoding encoding = GetInstructionType(hexInstruction);

	unsigned int ridx0 = 0;
	VOPInstruction::SRC src0 = GetSRC(hexInstruction, ridx0, 0);

    if ((hwGen == GDT_HW_GENERATION_SEAISLAND) || (hwGen == GDT_HW_GENERATION_SOUTHERNISLAND))
    {
        if (VOPInstruction::Encoding_VOP1 == encoding)
        {
            uint64_t hexInstTem = hexInstruction << 15;
            hexInstTem = hexInstTem >> 24;
            SIVOP1Instruction::VOP1_OP op1 = static_cast<SIVOP1Instruction::VOP1_OP>(hexInstTem);
			unsigned int vdstRidx = 0;
			SIVOP1Instruction::VDST vdst = GetVDST(hexInstruction, vdstRidx, VOPInstruction::Encoding_VOP1);
			auto& meta                     = GetSIVOP1Meta(op1);

			hasLiteral = (src0 == SIVOP1Instruction::SRCLiteralConst) || IsLiteralConstInstruction(op1);
			instruction = std::make_unique<SIVOP1Instruction>(src0, vdst, ridx0, vdstRidx, 32, encoding, op1, meta.insClass, meta.operandType);
            retStatus = ParserSI::Status_SUCCESS;
        }
        else if (VOPInstruction::Encoding_VOP2 == encoding)
        {
            uint64_t hexInstTem = hexInstruction << 1;
            hexInstTem = hexInstTem >> 26;
            SIVOP2Instruction::VOP2_OP op2 = static_cast<SIVOP2Instruction::VOP2_OP>(hexInstTem);
			unsigned int vidx1 = 0 ,vdstRidx = 0;;
			VOPInstruction::VSRC vsrc1 = GetVSRC1(hexInstruction, vidx1);
			SIVOP1Instruction::VDST vdst = GetVDST(hexInstruction, vdstRidx, VOPInstruction::Encoding_VOP2);
			auto& meta                   = GetSIVOP2Meta(op2);
		
			hasLiteral = (src0 == SIVOP1Instruction::SRCLiteralConst) || IsLiteralConstInstruction(op2);
			instruction = std::make_unique<SIVOP2Instruction>(src0, vsrc1, vdst, ridx0, vidx1, vdstRidx, 32, encoding, op2, meta.insClass, meta.operandType);
            retStatus = ParserSI::Status_SUCCESS;
        }

        else if (VOPInstruction::Encoding_VOPC == encoding)
        {
            uint64_t hexInstTem = hexInstruction << 7;
            hexInstTem = hexInstTem >> 24;
            SIVOPCInstruction::VOPC_OP opc = static_cast<SIVOPCInstruction::VOPC_OP>(hexInstTem);
			unsigned int vidx1 = 0;
			VOPInstruction::VSRC vsrc1 = GetVSRC1(hexInstruction, vidx1);
			auto& meta                     = GetSIVOPCMeta(opc);

			hasLiteral = (src0 == SIVOPCInstruction::SRCLiteralConst) || IsLiteralConstInstruction(opc);
			instruction = std::make_unique<SIVOPCInstruction>(src0, vsrc1, ridx0, vidx1, 32, opc, encoding, meta.insClass, meta.operandType);
            retStatus = ParserSI::Status_SUCCESS;
        }
    }
    else if (hwGen == GDT_HW_GENERATION_VOLCANICISLAND)
    {
        if (VOPInstruction::Encoding_VOP1 == encoding)
        {
            uint64_t hexInstTem = hexInstruction << 15;
            hexInstTem = hexInstTem >> 24;
            VIVOP1Instruction::VOP1_OP op1 = static_cast<VIVOP1Instruction::VOP1_OP>(hexInstTem);
			hasLiteral = (src0 == VIVOP1Instruction::SRCLiteralConst);
            instruction = std::make_unique<VIVOP1Instruction>(32, encoding, op1);
            retStatus = ParserSI::Status_SUCCESS;
        }
        else if (VOPInstruction::Encoding_VOP2 == encoding)
        {
            uint64_t hexInstTem = hexInstruction << 1;
            hexInstTem = hexInstTem >> 26;
            VIVOP2Instruction::VOP2_OP op2 = static_cast<VIVOP2Instruction::VOP2_OP>(hexInstTem);
			// TODO:
			// Not sure if there's some special opcode which use literal const while src != SRCLiteralConst
			hasLiteral = (src0 == VIVOP2Instruction::SRCLiteralConst);
            instruction = std::make_unique<VIVOP2Instruction>(32, encoding, op2);
            retStatus = ParserSI::Status_SUCCESS;
        }

        else if (VOPInstruction::Encoding_VOPC == encoding)
        {
            uint64_t hexInstTem = hexInstruction << 7;
            hexInstTem = hexInstTem >> 24;
            VIVOPCInstruction::VOPC_OP opc = static_cast<VIVOPCInstruction::VOPC_OP>(hexInstTem);
			hasLiteral = (src0 == VIVOPCInstruction::SRCLiteralConst);
            instruction = std::make_unique<VIVOPCInstruction>(32, encoding, opc);
            retStatus = ParserSI::Status_SUCCESS;
        }
    }
    else if (hwGen == GDT_HW_GENERATION_GFX9)
    {
        if (VOPInstruction::Encoding_VOP1 == encoding)
        {
            uint64_t hexInstTem = hexInstruction << 15;
            hexInstTem = hexInstTem >> 24;
            G9VOP1Instruction::VOP1_OP op1 = static_cast<G9VOP1Instruction::VOP1_OP>(hexInstTem);
			hasLiteral = (src0 == G9VOP1Instruction::SRCLiteralConst);
            instruction = std::make_unique<G9VOP1Instruction>(32, encoding, op1);
            retStatus = ParserSI::Status_SUCCESS;
        }
        else if (VOPInstruction::Encoding_VOP2 == encoding)
        {
            uint64_t hexInstTem = hexInstruction << 1;
            hexInstTem = hexInstTem >> 26;
            G9VOP2Instruction::VOP2_OP op2 = static_cast<G9VOP2Instruction::VOP2_OP>(hexInstTem);
			// TODO:
			// Not sure if there's some special opcode which use literal const while src != SRCLiteralConst
			hasLiteral = (src0 == G9VOP2Instruction::SRCLiteralConst);
            instruction = std::make_unique<G9VOP2Instruction>(32, encoding, op2);
            retStatus = ParserSI::Status_SUCCESS;
        }

        else if (VOPInstruction::Encoding_VOPC == encoding)
        {
            uint64_t hexInstTem = hexInstruction << 7;
            hexInstTem = hexInstTem >> 24;
            VIVOPCInstruction::VOPC_OP opc = static_cast<VIVOPCInstruction::VOPC_OP>(hexInstTem);
			hasLiteral = (src0 == VIVOPCInstruction::SRCLiteralConst);
            instruction = std::make_unique<VIVOPCInstruction>(32, encoding, opc);
            retStatus = ParserSI::Status_SUCCESS;
        }
    }
    else
    {
        retStatus = ParserSI::Status_UnexpectedHWGeneration;
    }

    return retStatus;
}

ParserSI::kaStatus
ParserSIVOP::Parse(GDT_HW_GENERATION hwGen, Instruction::instruction64bit hexInstruction, std::unique_ptr<Instruction>& instruction)
{
    kaStatus retStatus =   ParserSI::Status_64BitInstructionNotSupported;
	// Note:
	// This is bug of AMD's GCN Parser, for all other 64 bits encodings except for VOP3,
	// Parsers assume that the higher dword of encoding is placed in the lower dword of value.
	// But for VOP3, ParserSIVOP take the reverse, so we need to flip the higher dword and lower dword before parsing.
	hexInstruction = (hexInstruction << 32) | (hexInstruction >> 32);

    VOPInstruction::Encoding encoding = GetInstructionType(hexInstruction);

	unsigned int ridx0 = 0;
	VOPInstruction::SRC src0 = GetSRC(hexInstruction, ridx0, 0);

    if (hwGen == GDT_HW_GENERATION_SEAISLAND || hwGen == GDT_HW_GENERATION_SOUTHERNISLAND || hwGen == GDT_HW_GENERATION_VOLCANICISLAND)
    {
        if (VOPInstruction::Encoding_VOP3 == encoding)
        {
            uint64_t hexInstTem = hexInstruction << 6;
            hexInstTem = hexInstTem >> 55;
            SIVOP3Instruction::VOP3_OP op3 = static_cast<SIVOP3Instruction::VOP3_OP>(hexInstTem);

			unsigned int ridx1 = 0, ridx2 = 0;
			unsigned int vdstRidx = 0, sdstRidx = 0;
			VOPInstruction::SRC src1 = GetSRC(hexInstruction, ridx1, 1);
			VOPInstruction::SRC src2 = GetSRC(hexInstruction, ridx2, 2);
			VOPInstruction::VDST vdst = GetVDST(hexInstruction, vdstRidx, VOPInstruction::Encoding_VOP3);
			VOPInstruction::SDST sdst = GetSDST(hexInstruction, sdstRidx);
			VOPInstruction::OMOD omod = GetOMOD(hexInstruction);
			VOPInstruction::NEG neg = GetNEG(hexInstruction);
			VOPInstruction::ABS abs = GetABS(hexInstruction);
			VOPInstruction::CLMP clmp = GetCLMP(hexInstruction);

			auto& meta = GetSIVOP3Meta(op3);

            instruction = std::make_unique<SIVOP3Instruction>(src0, src1, src2,
				vdst, sdst,
				ridx0, ridx1, ridx2,
				vdstRidx, sdstRidx,
				clmp, abs, neg, omod,
				64, op3, encoding, meta.insClass, meta.operandType);
            retStatus =  ParserSI::Status_SUCCESS;
        }
    }
    else if (hwGen == GDT_HW_GENERATION_GFX9)
    {
        if (VOPInstruction::Encoding_VOP3P == encoding)
        {
            uint64_t hexInstTem = (hexInstruction >> 16) & 0x7F;
            G9VOP3Instruction::VOP3_OP op3 = static_cast<G9VOP3Instruction::VOP3_OP>(hexInstTem);
            instruction = std::make_unique<G9VOP3Instruction>(64, encoding, op3);
            retStatus =  ParserSI::Status_SUCCESS;
        }
        else if (VOPInstruction::Encoding_VOP3 == encoding)
        {
            uint64_t hexInstTem = hexInstruction << 15;
            hexInstTem = hexInstTem >> 24;
            G9VOP3Instruction::VOP3_OP op3 = static_cast<G9VOP3Instruction::VOP3_OP>(hexInstTem);
            instruction = std::make_unique<G9VOP3Instruction>(64, encoding, op3);
            retStatus =  ParserSI::Status_SUCCESS;
        }
    }
    else
    {
        retStatus = ParserSI::Status_UnexpectedHWGeneration;
    }

    return retStatus;
}


VOPInstruction::Encoding
ParserSIVOP::GetInstructionType(Instruction::instruction32bit hexInstruction)
{
    uint64_t hexInstTemp = hexInstruction >> 25;

    if (hexInstTemp == VOPInstruction::Encoding_VOP1)
    {
        return VOPInstruction::Encoding_VOP1;
    }
    else if (hexInstTemp == VOPInstruction::Encoding_VOPC)
    {
        return VOPInstruction::Encoding_VOPC;
    }

    hexInstTemp = hexInstruction >> 31;

    if (hexInstTemp == VOPInstruction::Encoding_VOP2)
    {
        return VOPInstruction::Encoding_VOP2;
    }

    return VOPInstruction::Encoding_Illegal;
}

VOPInstruction::Encoding
ParserSIVOP::GetInstructionType(Instruction::instruction64bit hexInstruction)
{
	Instruction::instruction32bit higherDword = static_cast<Instruction::instruction32bit>(hexInstruction >> 32);
    if ((higherDword & VOPInstruction::VOPMask_VOP3) >> 26 == VOPInstruction::Encoding_VOP3)
    {
        return VOPInstruction::Encoding_VOP3;
    }
    return VOPInstruction::Encoding_Illegal;
}

const GCNInstructionFormat& ParserSIVOP::GetSIVOP1Meta(SIVOP1Instruction::VOP1_OP op)
{
	return g_instructionFormatMapVOP1[op];
}

const GCNInstructionFormat& ParserSIVOP::GetSIVOP2Meta(SIVOP2Instruction::VOP2_OP op)
{
	return g_instructionFormatMapVOP2[op];
}

const GCNInstructionFormat& ParserSIVOP::GetSIVOPCMeta(SIVOPCInstruction::VOPC_OP op)
{
	return g_instructionFormatMapVOPC[op];
}

const GCNInstructionFormat& ParserSIVOP::GetSIVOP3Meta(SIVOP3Instruction::VOP3_OP op)
{
	return g_instructionFormatMapVOP3[op];
}

bool ParserSIVOP::IsLiteralConstInstruction(uint32_t op)
{
	bool hasLiteral = false;

	SIVOP2Instruction::VOP2_OP op2 = static_cast<SIVOP2Instruction::VOP2_OP>(op);
	// TODO:
	// Complete this list
	switch (op2)
	{
	case SIVOP2Instruction::V_MADMK_F32:
	case SIVOP2Instruction::V_MADAK_F32:
		hasLiteral = true;
		break;
	default:
		break;
	}

	return hasLiteral;
}
