#include "GcnDecoder.h"

#include "UtilBit.h"

#include <algorithm>

using namespace util;

LOG_CHANNEL(Graphic.Gcn.GcnDecoder);

namespace sce::gcn
{

	uint32_t GcnCodeSlice::at(uint32_t id) const
	{
		return m_ptr[id];
	}

	uint32_t GcnCodeSlice::readu32()
	{
		return *(m_ptr++);
	}

	uint64_t GcnCodeSlice::readu64()
	{
		uint64_t value = *(uint64_t*)m_ptr;
		m_ptr += 2;
		return value;
	}

	///

	GcnDecodeContext::GcnDecodeContext()
	{
	}

	GcnDecodeContext::~GcnDecodeContext()
	{
	}

	void GcnDecodeContext::decodeInstruction(GcnCodeSlice& code)
	{
		const uint32_t token = code.at(0);

		GcnInstEncoding encoding = getInstructionEncoding(token);
		LOG_ASSERT(encoding != GcnInstEncoding::ILLEGAL, "illegal encoding %d", encoding);
		uint32_t encodingLen = getEncodingLength(encoding);

		// Clear the instruction
		m_instruction = GcnShaderInstruction();
		// Decode
		if (encodingLen == sizeof(uint32_t))
		{
			decodeInstruction32(encoding, code);
		}
		else
		{
			decodeInstruction64(encoding, code);
		}

		// Update instruction meta info.
		updateInstructionMeta(encoding);

		// Detect literal constant.
		// Only 32 bits instructions may have literal constant.
		// Note:
		// Literal constant decode must be performed after meta info updated.
		if (encodingLen == sizeof(uint32_t))
		{
			decodeLiteralConstant(encoding, code);
		}
	}

	GcnInstEncoding GcnDecodeContext::getInstructionEncoding(uint32_t hexInstruction)
	{
		GcnInstEncoding instructionEncoding = GcnInstEncoding::ILLEGAL;
		/// Try all instructions encoding masks from longest to shortest until
		/// legal GcnInstEncoding is found.

		/// GcnEncodingMask::Mask9bit
		instructionEncoding = static_cast<GcnInstEncoding>(hexInstruction & (uint32_t)GcnEncodingMask::MASK_9bit);

		switch (instructionEncoding)
		{
		case GcnInstEncoding::SOP1:
		case GcnInstEncoding::SOPP:
		case GcnInstEncoding::SOPC:
			return instructionEncoding;

		default:
			break;
		}

		/// GcnEncodingMask::Mask7bit
		instructionEncoding = static_cast<GcnInstEncoding>(hexInstruction & (uint32_t)GcnEncodingMask::MASK_7bit);

		switch (instructionEncoding)
		{
		case GcnInstEncoding::VOP1:
		case GcnInstEncoding::VOPC:
			return instructionEncoding;

		default:
			break;
		}

		/// GcnEncodingMask::Mask6bit
		instructionEncoding = static_cast<GcnInstEncoding>(hexInstruction & (uint32_t)GcnEncodingMask::MASK_6bit);

		switch (instructionEncoding)
		{
		case GcnInstEncoding::VOP3:
		case GcnInstEncoding::EXP:
		case GcnInstEncoding::VINTRP:
		case GcnInstEncoding::DS:
		case GcnInstEncoding::MUBUF:
		case GcnInstEncoding::MTBUF:
		case GcnInstEncoding::MIMG:
			return instructionEncoding;

		default:
			break;
		}

		/// GcnEncodingMask::Mask5bit
		instructionEncoding = static_cast<GcnInstEncoding>(hexInstruction & (uint32_t)GcnEncodingMask::MASK_5bit);

		switch (instructionEncoding)
		{
		case GcnInstEncoding::SMRD:
			return instructionEncoding;

		default:
			break;
		}

		/// GcnEncodingMask::Mask4bit
		instructionEncoding = static_cast<GcnInstEncoding>(hexInstruction & (uint32_t)GcnEncodingMask::MASK_4bit);

		switch (instructionEncoding)
		{
		case GcnInstEncoding::SOPK:
			return instructionEncoding;

		default:
			break;
		}

		/// GcnEncodingMask::Mask2bit
		instructionEncoding = static_cast<GcnInstEncoding>(hexInstruction & (uint32_t)GcnEncodingMask::MASK_2bit);

		switch (instructionEncoding)
		{
		case GcnInstEncoding::SOP2:
			return instructionEncoding;

		default:
			break;
		}

		/// GcnEncodingMask::Mask1bit
		instructionEncoding = static_cast<GcnInstEncoding>(hexInstruction & (uint32_t)GcnEncodingMask::MASK_1bit);

		switch (instructionEncoding)
		{
		case GcnInstEncoding::VOP2:
			return instructionEncoding;

		default:
			break;
		}

		/// If no legal GcnInstEncoding found return GcnInstEncoding::ILLEGAL
		return GcnInstEncoding::ILLEGAL;
	}

	uint32_t GcnDecodeContext::getEncodingLength(GcnInstEncoding encoding)
	{
		uint32_t instLength = 0;

		switch (encoding)
		{
		case GcnInstEncoding::SOP1:
		case GcnInstEncoding::SOPP:
		case GcnInstEncoding::SOPC:
		case GcnInstEncoding::SOPK:
		case GcnInstEncoding::SOP2:
		case GcnInstEncoding::VOP1:
		case GcnInstEncoding::VOPC:
		case GcnInstEncoding::VOP2:
		case GcnInstEncoding::SMRD:
		case GcnInstEncoding::VINTRP:
			instLength = sizeof(uint32_t);
			break;

		case GcnInstEncoding::VOP3:
		case GcnInstEncoding::MUBUF:
		case GcnInstEncoding::MTBUF:
		case GcnInstEncoding::MIMG:
		case GcnInstEncoding::DS:
		case GcnInstEncoding::EXP:
			instLength = sizeof(uint64_t);
			break;
		}
		return instLength;
	}

	uint32_t GcnDecodeContext::getOpMapOffset(GcnInstEncoding encoding)
	{
		uint32_t offset = 0;
		switch (encoding)
		{
		case GcnInstEncoding::SOP1:
			offset = (uint32_t)GcnOpcodeMap::OP_MAP_SOP1;
			break;
		case GcnInstEncoding::SOPP:
			offset = (uint32_t)GcnOpcodeMap::OP_MAP_SOPP;
			break;
		case GcnInstEncoding::SOPC:
			offset = (uint32_t)GcnOpcodeMap::OP_MAP_SOPC;
			break;
		case GcnInstEncoding::VOP1:
			offset = (uint32_t)GcnOpcodeMap::OP_MAP_VOP1;
			break;
		case GcnInstEncoding::VOPC:
			offset = (uint32_t)GcnOpcodeMap::OP_MAP_VOPC;
			break;
		case GcnInstEncoding::VOP3:
			offset = (uint32_t)GcnOpcodeMap::OP_MAP_VOP3;
			break;
		case GcnInstEncoding::EXP:
			offset = (uint32_t)GcnOpcodeMap::OP_MAP_EXP;
			break;
		case GcnInstEncoding::VINTRP:
			offset = (uint32_t)GcnOpcodeMap::OP_MAP_VINTRP;
			break;
		case GcnInstEncoding::DS:
			offset = (uint32_t)GcnOpcodeMap::OP_MAP_DS;
			break;
		case GcnInstEncoding::MUBUF:
			offset = (uint32_t)GcnOpcodeMap::OP_MAP_MUBUF;
			break;
		case GcnInstEncoding::MTBUF:
			offset = (uint32_t)GcnOpcodeMap::OP_MAP_MTBUF;
			break;
		case GcnInstEncoding::MIMG:
			offset = (uint32_t)GcnOpcodeMap::OP_MAP_MIMG;
			break;
		case GcnInstEncoding::SMRD:
			offset = (uint32_t)GcnOpcodeMap::OP_MAP_SMRD;
			break;
		case GcnInstEncoding::SOPK:
			offset = (uint32_t)GcnOpcodeMap::OP_MAP_SOPK;
			break;
		case GcnInstEncoding::SOP2:
			offset = (uint32_t)GcnOpcodeMap::OP_MAP_SOP2;
			break;
		case GcnInstEncoding::VOP2:
			offset = (uint32_t)GcnOpcodeMap::OP_MAP_VOP2;
			break;
		}
		return offset;
	}

	uint32_t GcnDecodeContext::mapEncodingOp(GcnInstEncoding encoding, GcnOpcode opcode)
	{
		// Map from uniform opcode to encoding specific opcode.

		uint32_t encodingOp = 0;

		if (encoding == GcnInstEncoding::VOP3)
		{
			if (opcode >= GcnOpcode::V_CMP_F_F32 && opcode <= GcnOpcode::V_CMPX_T_U64)
			{
				uint32_t op = static_cast<uint32_t>(opcode) - static_cast<uint32_t>(GcnOpcodeMap::OP_MAP_VOPC);
				encodingOp  = op + static_cast<uint32_t>(GcnOpMapVOP3VOPX::VOP3_TO_VOPC);
			}
			else if (opcode >= GcnOpcode::V_CNDMASK_B32 && opcode <= GcnOpcode::V_CVT_PK_I16_I32)
			{
				uint32_t op = static_cast<uint32_t>(opcode) - static_cast<uint32_t>(GcnOpcodeMap::OP_MAP_VOP2);
				encodingOp  = op + static_cast<uint32_t>(GcnOpMapVOP3VOPX::VOP3_TO_VOP2);
			}
			else if (opcode >= GcnOpcode::V_NOP && opcode <= GcnOpcode::V_MOVRELSD_B32)
			{
				uint32_t op = static_cast<uint32_t>(opcode) - static_cast<uint32_t>(GcnOpcodeMap::OP_MAP_VOP1);
				encodingOp  = op + static_cast<uint32_t>(GcnOpMapVOP3VOPX::VOP3_TO_VOP1);
			}
			else
			{
				encodingOp = static_cast<uint32_t>(opcode) - static_cast<uint32_t>(GcnOpcodeMap::OP_MAP_VOP3);
			}
		}
		else
		{
			uint32_t mapOffset = getOpMapOffset(encoding);
			encodingOp         = static_cast<uint32_t>(opcode) - mapOffset;
		}

		return encodingOp;
	}

	bool GcnDecodeContext::hasAdditionalLiteral(GcnInstEncoding encoding, uint32_t opcode)
	{
		// Some instructions require an additional literal constant following the binary stream
		// even if LiteralConst is not specified in src operand.

		bool hasLiteral = false;
		switch (encoding)
		{
		case GcnInstEncoding::SOPK:
		{
			if (opcode == (uint32_t)GcnOpcodeSOPK::S_SETREG_IMM32_B32)
			{
				hasLiteral = true;
			}
		}
		break;
		case GcnInstEncoding::VOP2:
		{
			if (opcode == (uint32_t)GcnOpcodeVOP2::V_MADMK_F32 ||
				opcode == (uint32_t)GcnOpcodeVOP2::V_MADAK_F32)
			{
				hasLiteral = true;
			}
		}
		break;
		}
		return hasLiteral;
	}

	void GcnDecodeContext::updateInstructionMeta(GcnInstEncoding encoding)
	{
		uint32_t      encodingOp = mapEncodingOp(encoding, m_instruction.opcode);
		GcnInstFormat instFormat = gcnInstructionFormat(encoding, encodingOp);

		m_instruction.opClass  = instFormat.instructionClass;
		m_instruction.encoding = encoding;
		m_instruction.srcCount = instFormat.srcCount;
		m_instruction.length   = getEncodingLength(encoding);

		// Update src operand numeric type.
		// Dst operand's type is set during instruction decoding.
		auto setOperandNumType = [&instFormat](GcnInstOperand& src)
		{
			// Only update uninitialized numeric type.
			if (src.numericType == GcnNumericType::Undefined)
			{
				src.numericType = instFormat.srcType;
			}
		};

		std::for_each_n(std::begin(m_instruction.src), m_instruction.srcCount, setOperandNumType);
	}

	GcnOperandField GcnDecodeContext::getOperandField(uint32_t code)
	{
		GcnOperandField field = {};

		if (code >= ScalarGPRMin && code <= ScalarGPRMax)
		{
			field = GcnOperandField::ScalarGPR;
		}
		else if (code >= SignedConstIntPosMin && code <= SignedConstIntPosMax)
		{
			field = GcnOperandField::SignedConstIntPos;
		}
		else if (code >= SignedConstIntNegMin && code <= SignedConstIntNegMax)
		{
			field = GcnOperandField::SignedConstIntNeg;
		}
		else if (code >= VectorGPRMin && code <= VectorGPRMax)
		{
			field = GcnOperandField::VectorGPR;
		}
		else
		{
			field = static_cast<GcnOperandField>(code);
		}

		return field;
	}

	void GcnDecodeContext::decodeInstruction32(GcnInstEncoding encoding, GcnCodeSlice& code)
	{
		uint32_t hexInstruction = code.readu32();
		switch (encoding)
		{
		case GcnInstEncoding::SOP1:
			decodeInstructionSOP1(hexInstruction);
			break;
		case GcnInstEncoding::SOPP:
			decodeInstructionSOPP(hexInstruction);
			break;
		case GcnInstEncoding::SOPC:
			decodeInstructionSOPC(hexInstruction);
			break;
		case GcnInstEncoding::SOPK:
			decodeInstructionSOPK(hexInstruction);
			break;
		case GcnInstEncoding::SOP2:
			decodeInstructionSOP2(hexInstruction);
			break;
		case GcnInstEncoding::VOP1:
			decodeInstructionVOP1(hexInstruction);
			break;
		case GcnInstEncoding::VOPC:
			decodeInstructionVOPC(hexInstruction);
			break;
		case GcnInstEncoding::VOP2:
			decodeInstructionVOP2(hexInstruction);
			break;
		case GcnInstEncoding::SMRD:
			decodeInstructionSMRD(hexInstruction);
			break;
		case GcnInstEncoding::VINTRP:
			decodeInstructionVINTRP(hexInstruction);
			break;
		}
	}

	void GcnDecodeContext::decodeInstruction64(GcnInstEncoding encoding, GcnCodeSlice& code)
	{
		uint64_t hexInstruction = code.readu64();
		switch (encoding)
		{
		case GcnInstEncoding::VOP3:
			decodeInstructionVOP3(hexInstruction);
			break;
		case GcnInstEncoding::MUBUF:
			decodeInstructionMUBUF(hexInstruction);
			break;
		case GcnInstEncoding::MTBUF:
			decodeInstructionMTBUF(hexInstruction);
			break;
		case GcnInstEncoding::MIMG:
			decodeInstructionMIMG(hexInstruction);
			break;
		case GcnInstEncoding::DS:
			decodeInstructionDS(hexInstruction);
			break;
		case GcnInstEncoding::EXP:
			decodeInstructionEXP(hexInstruction);
			break;
		}
	}

	void GcnDecodeContext::decodeLiteralConstant(GcnInstEncoding encoding, GcnCodeSlice& code)
	{
		bool hasLiteral = false;
		do
		{
			// Detect if it's a special instruction.
			hasLiteral = hasAdditionalLiteral(
				encoding, mapEncodingOp(encoding, m_instruction.opcode));

			if (hasLiteral)
			{
				uint32_t literalCount                                    = code.readu32();
				m_instruction.src[m_instruction.srcCount++].literalConst = literalCount;
				break;
			}

			// Find if the instruction contains a literal constant
			auto isLiteralSrc = [](GcnInstOperand& src)
			{
				return src.field == GcnOperandField::LiteralConst;
			};

			auto iter = std::find_if(
				std::begin(m_instruction.src),
				std::end(m_instruction.src),
				isLiteralSrc);

			hasLiteral = (iter != std::end(m_instruction.src));

			if (hasLiteral)
			{
				uint32_t literalCount = code.readu32();
				iter->literalConst    = literalCount;
				break;
			}

		} while (false);

		// Increase instruction length by 4
		// if literal constant append.
		if (hasLiteral)
		{
			m_instruction.length += sizeof(uint32_t);
		}
	}

	void GcnDecodeContext::decodeInstructionSOP1(uint32_t hexInstruction)
	{
		uint32_t ssrc0 = bit::extract(hexInstruction, 7, 0);
		uint32_t op    = bit::extract(hexInstruction, 15, 8);
		uint32_t sdst  = bit::extract(hexInstruction, 22, 16);

		m_instruction.opcode = static_cast<GcnOpcode>(op + static_cast<uint32_t>(GcnOpcodeMap::OP_MAP_SOP1));

		m_instruction.src[0].field = getOperandField(ssrc0);
		m_instruction.src[0].code  = ssrc0;
		m_instruction.dst[0].field = getOperandField(sdst);
		m_instruction.dst[0].code  = sdst;
		m_instruction.dstCount     = 1;
	}

	void GcnDecodeContext::decodeInstructionSOPP(uint32_t hexInstruction)
	{
		uint32_t op = bit::extract(hexInstruction, 22, 16);

		m_instruction.opcode = static_cast<GcnOpcode>(op + static_cast<uint32_t>(GcnOpcodeMap::OP_MAP_SOPP));

		m_instruction.control.sopp = *reinterpret_cast<GcnInstControlSOPP*>(&hexInstruction);
	}

	void GcnDecodeContext::decodeInstructionSOPC(uint32_t hexInstruction)
	{
		uint32_t ssrc0 = bit::extract(hexInstruction, 7, 0);
		uint32_t ssrc1 = bit::extract(hexInstruction, 15, 8);
		uint32_t op    = bit::extract(hexInstruction, 22, 16);

		m_instruction.opcode = static_cast<GcnOpcode>(op + static_cast<uint32_t>(GcnOpcodeMap::OP_MAP_SOPC));

		m_instruction.src[0].field = getOperandField(ssrc0);
		m_instruction.src[0].code  = ssrc0;
		m_instruction.src[1].field = getOperandField(ssrc1);
		m_instruction.src[1].code  = ssrc1;
	}

	void GcnDecodeContext::decodeInstructionSOPK(uint32_t hexInstruction)
	{
		uint32_t sdst = bit::extract(hexInstruction, 22, 16);
		uint32_t op   = bit::extract(hexInstruction, 27, 23);

		m_instruction.opcode = static_cast<GcnOpcode>(op + static_cast<uint32_t>(GcnOpcodeMap::OP_MAP_SOPK));

		m_instruction.dst[0].field = getOperandField(sdst);
		m_instruction.dst[0].code  = sdst;
		m_instruction.dstCount     = 1;

		m_instruction.control.sopk = *reinterpret_cast<GcnInstControlSOPK*>(&hexInstruction);
	}

	void GcnDecodeContext::decodeInstructionSOP2(uint32_t hexInstruction)
	{
		uint32_t ssrc0 = bit::extract(hexInstruction, 7, 0);
		uint32_t ssrc1 = bit::extract(hexInstruction, 15, 8);
		uint32_t sdst  = bit::extract(hexInstruction, 22, 16);
		uint32_t op    = bit::extract(hexInstruction, 29, 23);

		m_instruction.opcode = static_cast<GcnOpcode>(op + static_cast<uint32_t>(GcnOpcodeMap::OP_MAP_SOP2));

		m_instruction.src[0].field = getOperandField(ssrc0);
		m_instruction.src[0].code  = ssrc0;
		m_instruction.src[1].field = getOperandField(ssrc1);
		m_instruction.src[1].code  = ssrc1;
		m_instruction.dst[0].field = getOperandField(sdst);
		m_instruction.dst[0].code  = sdst;
		m_instruction.dstCount     = 1;
	}

	void GcnDecodeContext::decodeInstructionVOP1(uint32_t hexInstruction)
	{
		uint32_t src0 = bit::extract(hexInstruction, 8, 0);
		uint32_t op   = bit::extract(hexInstruction, 16, 9);
		uint32_t vdst = bit::extract(hexInstruction, 24, 17);

		m_instruction.opcode = static_cast<GcnOpcode>(op + static_cast<uint32_t>(GcnOpcodeMap::OP_MAP_VOP1));

		m_instruction.src[0].field = getOperandField(src0);
		m_instruction.src[0].code  = src0;
		m_instruction.dst[0].field = GcnOperandField::VectorGPR;
		m_instruction.dst[0].code  = vdst;
		m_instruction.dstCount     = 1;
	}

	void GcnDecodeContext::decodeInstructionVOPC(uint32_t hexInstruction)
	{
		uint32_t src0  = bit::extract(hexInstruction, 8, 0);
		uint32_t vsrc1 = bit::extract(hexInstruction, 16, 9);
		uint32_t op    = bit::extract(hexInstruction, 24, 17);

		m_instruction.opcode = static_cast<GcnOpcode>(op + static_cast<uint32_t>(GcnOpcodeMap::OP_MAP_VOPC));

		m_instruction.src[0].field = getOperandField(src0);
		m_instruction.src[0].code  = src0;
		m_instruction.src[1].field = GcnOperandField::VectorGPR;
		m_instruction.src[1].code  = vsrc1;
	}

	void GcnDecodeContext::decodeInstructionVOP2(uint32_t hexInstruction)
	{
		uint32_t src0  = bit::extract(hexInstruction, 8, 0);
		uint32_t vsrc1 = bit::extract(hexInstruction, 16, 9);
		uint32_t vdst  = bit::extract(hexInstruction, 24, 17);
		uint32_t op    = bit::extract(hexInstruction, 30, 25);

		m_instruction.opcode = static_cast<GcnOpcode>(op + static_cast<uint32_t>(GcnOpcodeMap::OP_MAP_VOP2));

		m_instruction.src[0].field = getOperandField(src0);
		m_instruction.src[0].code  = src0;
		m_instruction.src[1].field = GcnOperandField::VectorGPR;
		m_instruction.src[1].code  = vsrc1;
		m_instruction.dst[0].field = GcnOperandField::VectorGPR;
		m_instruction.dst[0].code  = vdst;
		m_instruction.dstCount     = 1;
	}

	void GcnDecodeContext::decodeInstructionSMRD(uint32_t hexInstruction)
	{
		uint32_t sbase = bit::extract(hexInstruction, 14, 9);
		uint32_t sdst  = bit::extract(hexInstruction, 21, 15);
		uint32_t op    = bit::extract(hexInstruction, 26, 22);

		m_instruction.opcode = static_cast<GcnOpcode>(op + static_cast<uint32_t>(GcnOpcodeMap::OP_MAP_SMRD));

		m_instruction.src[0].field = GcnOperandField::ScalarGPR;
		m_instruction.src[0].code  = sbase;
		m_instruction.dst[0].field = GcnOperandField::ScalarGPR;
		m_instruction.dst[0].code  = sdst;
		m_instruction.dstCount     = 1;

		m_instruction.control.smrd = *reinterpret_cast<GcnInstControlSMRD*>(&hexInstruction);

		if (op <= static_cast<uint32_t>(GcnOpcodeSMRD::S_LOAD_DWORDX16))
		{
			m_instruction.control.smrd.count = 1 << op;
		}
		else if (op >= static_cast<uint32_t>(GcnOpcodeSMRD::S_BUFFER_LOAD_DWORD) &&
				 op <= static_cast<uint32_t>(GcnOpcodeSMRD::S_BUFFER_LOAD_DWORDX16))
		{
			m_instruction.control.smrd.count = 1 << (op - 8);
		}
	}

	void GcnDecodeContext::decodeInstructionVINTRP(uint32_t hexInstruction)
	{
		uint32_t vsrc = bit::extract(hexInstruction, 7, 0);
		uint32_t op   = bit::extract(hexInstruction, 17, 16);
		uint32_t vdst = bit::extract(hexInstruction, 25, 18);

		m_instruction.opcode = static_cast<GcnOpcode>(op + static_cast<uint32_t>(GcnOpcodeMap::OP_MAP_VINTRP));

		m_instruction.src[0].field = GcnOperandField::VectorGPR;
		m_instruction.src[0].code  = vsrc;
		m_instruction.dst[0].field = GcnOperandField::VectorGPR;
		m_instruction.dst[0].code  = vdst;
		m_instruction.dstCount     = 1;

		m_instruction.control.vintrp = *reinterpret_cast<GcnInstControlVINTRP*>(&hexInstruction);
	}

	void GcnDecodeContext::decodeInstructionVOP3(uint64_t hexInstruction)
	{
		uint32_t vdst = bit::extract(hexInstruction, 7, 0);
		uint32_t sdst = bit::extract(hexInstruction, 14, 8);  // For VOP3B
		uint32_t op   = bit::extract(hexInstruction, 25, 17);
		uint32_t src0 = bit::extract(hexInstruction, 40, 32);
		uint32_t src1 = bit::extract(hexInstruction, 49, 41);
		uint32_t src2 = bit::extract(hexInstruction, 58, 50);

		if (op >= static_cast<uint32_t>(GcnOpcodeVOP3::V_CMP_F_F32) &&
			op <= static_cast<uint32_t>(GcnOpcodeVOP3::V_CMPX_T_U64))
		{
			// Map from VOP3 to VOPC
			uint32_t vopcOp      = op - static_cast<uint32_t>(GcnOpMapVOP3VOPX::VOP3_TO_VOPC);
			m_instruction.opcode = static_cast<GcnOpcode>(vopcOp + static_cast<uint32_t>(GcnOpcodeMap::OP_MAP_VOPC));
		}
		else if (op >= static_cast<uint32_t>(GcnOpcodeVOP3::V_CNDMASK_B32) &&
				 op <= static_cast<uint32_t>(GcnOpcodeVOP3::V_CVT_PK_I16_I32))
		{
			// Map from VOP3 to VOP2
			uint32_t vop2Op      = op - static_cast<uint32_t>(GcnOpMapVOP3VOPX::VOP3_TO_VOP2);
			m_instruction.opcode = static_cast<GcnOpcode>(vop2Op + static_cast<uint32_t>(GcnOpcodeMap::OP_MAP_VOP2));
		}
		else if (op >= static_cast<uint32_t>(GcnOpcodeVOP3::V_NOP) &&
				 op <= static_cast<uint32_t>(GcnOpcodeVOP3::V_MOVRELSD_B32))
		{
			// Map from VOP3 to VOP1
			uint32_t vop1Op      = op - static_cast<uint32_t>(GcnOpMapVOP3VOPX::VOP3_TO_VOP1);
			m_instruction.opcode = static_cast<GcnOpcode>(vop1Op + static_cast<uint32_t>(GcnOpcodeMap::OP_MAP_VOP1));
		}
		else
		{
			// VOP3 encoding, do not map.
			m_instruction.opcode = static_cast<GcnOpcode>(op + static_cast<uint32_t>(GcnOpcodeMap::OP_MAP_VOP3));
		}

		m_instruction.src[0].field = getOperandField(src0);
		m_instruction.src[0].code  = src0;
		m_instruction.src[1].field = getOperandField(src1);
		m_instruction.src[1].code  = src1;
		m_instruction.src[2].field = getOperandField(src2);
		m_instruction.src[2].code  = src2;
		m_instruction.dst[0].field = GcnOperandField::VectorGPR;
		m_instruction.dst[0].code  = vdst;
		m_instruction.dst[1].field = GcnOperandField::ScalarGPR;
		m_instruction.dst[1].code  = sdst;

		if (op >= static_cast<uint32_t>(GcnOpcodeVOP3::V_ADD_I32) &&
			op <= static_cast<uint32_t>(GcnOpcodeVOP3::V_DIV_SCALE_F64))
		{
			// VOP3B has a sdst operand.
			m_instruction.dstCount = 2;
		}
		else
		{
			m_instruction.dstCount = 1;
		}

		m_instruction.control.vop3 = *reinterpret_cast<GcnInstControlVOP3*>(&hexInstruction);
	}

	void GcnDecodeContext::decodeInstructionMUBUF(uint64_t hexInstruction)
	{
		uint32_t op      = bit::extract(hexInstruction, 24, 18);
		uint32_t vaddr   = bit::extract(hexInstruction, 39, 32);
		uint32_t vdata   = bit::extract(hexInstruction, 47, 40);
		uint32_t srsrc   = bit::extract(hexInstruction, 52, 48);
		uint32_t soffset = bit::extract(hexInstruction, 63, 56);

		m_instruction.opcode = static_cast<GcnOpcode>(op + static_cast<uint32_t>(GcnOpcodeMap::OP_MAP_MUBUF));

		m_instruction.src[0].field = GcnOperandField::VectorGPR;
		m_instruction.src[0].code  = vaddr;
		m_instruction.src[1].field = GcnOperandField::VectorGPR;
		m_instruction.src[1].code  = vdata;
		m_instruction.src[2].field = GcnOperandField::ScalarGPR;
		m_instruction.src[2].code  = srsrc;
		m_instruction.src[3].field = GcnOperandField::ScalarGPR;
		m_instruction.src[3].code  = soffset;

		m_instruction.control.mubuf = *reinterpret_cast<GcnInstControlMUBUF*>(&hexInstruction);

		if (op >= static_cast<uint32_t>(GcnOpcodeMUBUF::BUFFER_LOAD_FORMAT_X) &&
			op <= static_cast<uint32_t>(GcnOpcodeMUBUF::BUFFER_LOAD_FORMAT_XYZW))
		{
			m_instruction.control.mubuf.count = op + 1;
		}
		else if (op >= static_cast<uint32_t>(GcnOpcodeMUBUF::BUFFER_STORE_FORMAT_X) &&
				 op <= static_cast<uint32_t>(GcnOpcodeMUBUF::BUFFER_STORE_FORMAT_XYZW))
		{
			m_instruction.control.mubuf.count = op - 3;
		}
		else if (op >= static_cast<uint32_t>(GcnOpcodeMUBUF::BUFFER_LOAD_DWORD) &&
				 op <= static_cast<uint32_t>(GcnOpcodeMUBUF::BUFFER_LOAD_DWORDX4))
		{
			m_instruction.control.mubuf.count = 1 << (op - 12);
		}
		else if (op >= static_cast<uint32_t>(GcnOpcodeMUBUF::BUFFER_STORE_DWORD) &&
				 op <= static_cast<uint32_t>(GcnOpcodeMUBUF::BUFFER_STORE_DWORDX4))
		{
			m_instruction.control.mubuf.count = 1 << (op - 28);
		}
	}

	void GcnDecodeContext::decodeInstructionMTBUF(uint64_t hexInstruction)
	{
		uint32_t op      = bit::extract(hexInstruction, 18, 16);
		uint32_t vaddr   = bit::extract(hexInstruction, 39, 32);
		uint32_t vdata   = bit::extract(hexInstruction, 47, 40);
		uint32_t srsrc   = bit::extract(hexInstruction, 52, 48);
		uint32_t soffset = bit::extract(hexInstruction, 63, 56);

		m_instruction.opcode = static_cast<GcnOpcode>(op + static_cast<uint32_t>(GcnOpcodeMap::OP_MAP_MTBUF));

		m_instruction.src[0].field = GcnOperandField::VectorGPR;
		m_instruction.src[0].code  = vaddr;
		m_instruction.src[1].field = GcnOperandField::VectorGPR;
		m_instruction.src[1].code  = vdata;
		m_instruction.src[2].field = GcnOperandField::ScalarGPR;
		m_instruction.src[2].code  = srsrc;
		m_instruction.src[3].field = GcnOperandField::ScalarGPR;
		m_instruction.src[3].code  = soffset;

		m_instruction.control.mtbuf = *reinterpret_cast<GcnInstControlMTBUF*>(&hexInstruction);

		if (op >= static_cast<uint32_t>(GcnOpcodeMTBUF::TBUFFER_LOAD_FORMAT_X) &&
			op <= static_cast<uint32_t>(GcnOpcodeMTBUF::TBUFFER_LOAD_FORMAT_XYZW))
		{
			m_instruction.control.mtbuf.count = op + 1;
		}
		else if (op >= static_cast<uint32_t>(GcnOpcodeMTBUF::TBUFFER_STORE_FORMAT_X) &&
				 op <= static_cast<uint32_t>(GcnOpcodeMTBUF::TBUFFER_STORE_FORMAT_XYZW))
		{
			m_instruction.control.mtbuf.count = op - 3;
		}
	}

	void GcnDecodeContext::decodeInstructionMIMG(uint64_t hexInstruction)
	{
		uint32_t op    = bit::extract(hexInstruction, 24, 18);
		uint32_t vaddr = bit::extract(hexInstruction, 39, 32);
		uint32_t vdata = bit::extract(hexInstruction, 47, 40);
		uint32_t srsrc = bit::extract(hexInstruction, 52, 48);
		uint32_t ssamp = bit::extract(hexInstruction, 57, 53);

		m_instruction.opcode = static_cast<GcnOpcode>(op + static_cast<uint32_t>(GcnOpcodeMap::OP_MAP_MIMG));

		m_instruction.src[0].field = GcnOperandField::VectorGPR;
		m_instruction.src[0].code  = vaddr;
		m_instruction.src[1].field = GcnOperandField::VectorGPR;
		m_instruction.src[1].code  = vdata;
		m_instruction.src[2].field = GcnOperandField::ScalarGPR;
		m_instruction.src[2].code  = srsrc;
		m_instruction.src[3].field = GcnOperandField::ScalarGPR;
		m_instruction.src[3].code  = ssamp;

		m_instruction.control.mimg = *reinterpret_cast<GcnInstControlMIMG*>(&hexInstruction);
	}

	void GcnDecodeContext::decodeInstructionDS(uint64_t hexInstruction)
	{
		uint32_t op    = bit::extract(hexInstruction, 25, 18);
		uint32_t addr  = bit::extract(hexInstruction, 39, 32);
		uint32_t data0 = bit::extract(hexInstruction, 47, 40);
		uint32_t data1 = bit::extract(hexInstruction, 55, 48);
		uint32_t vdst  = bit::extract(hexInstruction, 63, 56);

		m_instruction.opcode = static_cast<GcnOpcode>(op + static_cast<uint32_t>(GcnOpcodeMap::OP_MAP_DS));

		m_instruction.src[0].field = GcnOperandField::VectorGPR;
		m_instruction.src[0].code  = addr;
		m_instruction.src[1].field = GcnOperandField::VectorGPR;
		m_instruction.src[1].code  = data0;
		m_instruction.src[2].field = GcnOperandField::VectorGPR;
		m_instruction.src[2].code  = data1;
		m_instruction.dst[0].field = GcnOperandField::VectorGPR;
		m_instruction.dst[0].code  = vdst;
		m_instruction.dstCount     = 1;

		m_instruction.control.ds = *reinterpret_cast<GcnInstControlDS*>(&hexInstruction);
	}

	void GcnDecodeContext::decodeInstructionEXP(uint64_t hexInstruction)
	{
		uint32_t vsrc0 = bit::extract(hexInstruction, 39, 32);
		uint32_t vsrc1 = bit::extract(hexInstruction, 47, 40);
		uint32_t vsrc2 = bit::extract(hexInstruction, 55, 48);
		uint32_t vsrc3 = bit::extract(hexInstruction, 63, 56);

		m_instruction.opcode = GcnOpcode::EXP;

		m_instruction.src[0].field = GcnOperandField::VectorGPR;
		m_instruction.src[0].code  = vsrc0;
		m_instruction.src[1].field = GcnOperandField::VectorGPR;
		m_instruction.src[1].code  = vsrc1;
		m_instruction.src[2].field = GcnOperandField::VectorGPR;
		m_instruction.src[2].code  = vsrc2;
		m_instruction.src[3].field = GcnOperandField::VectorGPR;
		m_instruction.src[3].code  = vsrc3;

		m_instruction.control.exp = *reinterpret_cast<GcnInstControlEXP*>(&hexInstruction);
	}

	///////////////////////////////////////////////////////////////

		inline GcnShaderInstSOP1 gcnInstCastToSOP1(const GcnShaderInstruction& ins)
	{
		GcnShaderInstSOP1 result = {};
		result.opcode            = ins.opcode;
		result.length            = ins.length;
		result.opClass           = ins.opClass;

		result.ssrc0 = ins.src[0];
		result.sdst  = ins.dst[0];

		return result;
	}

	inline GcnShaderInstSOP2 gcnInstCastToSOP2(const GcnShaderInstruction& ins)
	{
		GcnShaderInstSOP2 result = {};
		result.opcode            = ins.opcode;
		result.length            = ins.length;
		result.opClass           = ins.opClass;

		result.ssrc0 = ins.src[0];
		result.ssrc1 = ins.src[1];
		result.sdst  = ins.dst[0];

		return result;
	}

	inline GcnShaderInstSOPK gcnInstCastToSOPK(const GcnShaderInstruction& ins)
	{
		GcnShaderInstSOPK result = {};
		result.opcode            = ins.opcode;
		result.length            = ins.length;
		result.opClass           = ins.opClass;

		result.control = ins.control.sopk;
		result.sdst    = ins.dst[0];

		return result;
	}

	inline GcnShaderInstSOPC gcnInstCastToSOPC(const GcnShaderInstruction& ins)
	{
		GcnShaderInstSOPC result = {};
		result.opcode            = ins.opcode;
		result.length            = ins.length;
		result.opClass           = ins.opClass;

		result.ssrc0 = ins.src[0];
		result.ssrc1 = ins.src[1];

		return result;
	}

	inline GcnShaderInstSOPP gcnInstCastToSOPP(const GcnShaderInstruction& ins)
	{
		GcnShaderInstSOPP result = {};
		result.opcode            = ins.opcode;
		result.length            = ins.length;
		result.opClass           = ins.opClass;

		result.control = ins.control.sopp;

		return result;
	}

	inline GcnShaderInstVOP1 gcnInstCastToVOP1(const GcnShaderInstruction& ins)
	{
		GcnShaderInstVOP1 result = {};
		result.opcode            = ins.opcode;
		result.length            = ins.length;
		result.opClass           = ins.opClass;

		result.src0 = ins.src[0];
		result.vdst = ins.dst[0];

		return result;
	}

	inline GcnShaderInstVOP2 gcnInstCastToVOP2(const GcnShaderInstruction& ins)
	{
		GcnShaderInstVOP2 result = {};
		result.opcode            = ins.opcode;
		result.length            = ins.length;
		result.opClass           = ins.opClass;

		result.src0  = ins.src[0];
		result.vsrc1 = ins.src[1];
		result.vdst  = ins.dst[0];

		return result;
	}

	inline GcnShaderInstVOP3 gcnInstCastToVOP3(const GcnShaderInstruction& ins)
	{
		GcnShaderInstVOP3 result = {};
		result.opcode            = ins.opcode;
		result.length            = ins.length;
		result.opClass           = ins.opClass;

		result.control = ins.control.vop3;
		result.src0    = ins.src[0];
		result.src1    = ins.src[1];
		result.src2    = ins.src[2];
		result.vdst    = ins.dst[0];
		result.sdst    = ins.dst[1];

		return result;
	}

	inline GcnShaderInstVOPC gcnInstCastToVOPC(const GcnShaderInstruction& ins)
	{
		GcnShaderInstVOPC result = {};
		result.opcode            = ins.opcode;
		result.length            = ins.length;
		result.opClass           = ins.opClass;

		result.src0  = ins.src[0];
		result.vsrc1 = ins.src[1];

		return result;
	}

	inline GcnShaderInstSMRD gcnInstCastToSMRD(const GcnShaderInstruction& ins)
	{
		GcnShaderInstSMRD result = {};
		result.opcode            = ins.opcode;
		result.length            = ins.length;
		result.opClass           = ins.opClass;

		result.control = ins.control.smrd;

		// TODO:
		// Not sure how to interpret control.offset when imm is not 1
		// since I haven't met such an instruction yet.
		// But when we got one, complete the code then remove this assert.
		LOG_ASSERT(result.control.imm == 1, "TODO: sgpr offset not supported yet.");
		if (!result.control.imm)
		{
			// TODO:
			// Maybe not correct.
			result.offset.field = GcnOperandField::ScalarGPR;
			result.offset.code  = result.control.offset;
		}

		result.sbase = ins.src[0];
		result.sdst  = ins.dst[0];

		return result;
	}

	inline GcnShaderInstMUBUF gcnInstCastToMUBUF(const GcnShaderInstruction& ins)
	{
		GcnShaderInstMUBUF result = {};
		result.opcode             = ins.opcode;
		result.length             = ins.length;
		result.opClass            = ins.opClass;

		result.control = ins.control.mubuf;
		result.vaddr   = ins.src[0];
		result.vdata   = ins.src[1];
		result.srsrc   = ins.src[2];
		result.soffset = ins.src[3];

		return result;
	}

	inline GcnShaderInstMTBUF gcnInstCastToMTBUF(const GcnShaderInstruction& ins)
	{
		GcnShaderInstMTBUF result = {};
		result.opcode             = ins.opcode;
		result.length             = ins.length;
		result.opClass            = ins.opClass;

		result.control = ins.control.mtbuf;
		result.vaddr   = ins.src[0];
		result.vdata   = ins.src[1];
		result.srsrc   = ins.src[2];
		result.soffset = ins.src[3];

		return result;
	}

	inline GcnShaderInstMIMG gcnInstCastToMIMG(const GcnShaderInstruction& ins)
	{
		GcnShaderInstMIMG result = {};
		result.opcode            = ins.opcode;
		result.length            = ins.length;
		result.opClass           = ins.opClass;

		result.control = ins.control.mimg;
		result.vaddr   = ins.src[0];
		result.vdata   = ins.src[1];
		result.srsrc   = ins.src[2];
		result.ssamp   = ins.src[3];

		return result;
	}

	inline GcnShaderInstVINTRP gcnInstCastToVINTRP(const GcnShaderInstruction& ins)
	{
		GcnShaderInstVINTRP result = {};
		result.opcode              = ins.opcode;
		result.length              = ins.length;
		result.opClass             = ins.opClass;

		result.control = ins.control.vintrp;
		result.vsrc    = ins.src[0];
		result.vdst    = ins.dst[0];

		return result;
	}

	inline GcnShaderInstDS gcnInstCastToDS(const GcnShaderInstruction& ins)
	{
		GcnShaderInstDS result = {};
		result.opcode          = ins.opcode;
		result.length          = ins.length;
		result.opClass         = ins.opClass;

		result.control = ins.control.ds;
		result.addr    = ins.src[0];
		result.data0   = ins.src[1];
		result.data1   = ins.src[2];
		result.vdst    = ins.dst[0];

		return result;
	}

	inline GcnShaderInstEXP gcnInstCastToEXP(const GcnShaderInstruction& ins)
	{
		GcnShaderInstEXP result = {};
		result.opcode           = ins.opcode;
		result.length           = ins.length;
		result.opClass          = ins.opClass;

		result.control = ins.control.exp;
		result.vsrc0   = ins.src[0];
		result.vsrc1   = ins.src[1];
		result.vsrc2   = ins.src[2];
		result.vsrc3   = ins.src[3];

		return result;
	}

	GcnInstructionVariant gcnInstructionConvert(const GcnShaderInstruction& ins)
	{
		GcnInstructionVariant result = {};
		switch (ins.encoding)
		{
		case GcnInstEncoding::SOP1:
			result = gcnInstCastToSOP1(ins);
			break;
		case GcnInstEncoding::SOPP:
			result = gcnInstCastToSOPP(ins);
			break;
		case GcnInstEncoding::SOPC:
			result = gcnInstCastToSOPC(ins);
			break;
		case GcnInstEncoding::VOP1:
			result = gcnInstCastToVOP1(ins);
			break;
		case GcnInstEncoding::VOPC:
			result = gcnInstCastToVOPC(ins);
			break;
		case GcnInstEncoding::VOP3:
			result = gcnInstCastToVOP3(ins);
			break;
		case GcnInstEncoding::EXP:
			result = gcnInstCastToEXP(ins);
			break;
		case GcnInstEncoding::VINTRP:
			result = gcnInstCastToVINTRP(ins);
			break;
		case GcnInstEncoding::DS:
			result = gcnInstCastToDS(ins);
			break;
		case GcnInstEncoding::MUBUF:
			result = gcnInstCastToMUBUF(ins);
			break;
		case GcnInstEncoding::MTBUF:
			result = gcnInstCastToMTBUF(ins);
			break;
		case GcnInstEncoding::MIMG:
			result = gcnInstCastToMIMG(ins);
			break;
		case GcnInstEncoding::SMRD:
			result = gcnInstCastToSMRD(ins);
			break;
		case GcnInstEncoding::SOPK:
			result = gcnInstCastToSOPK(ins);
			break;
		case GcnInstEncoding::SOP2:
			result = gcnInstCastToSOP2(ins);
			break;
		case GcnInstEncoding::VOP2:
			result = gcnInstCastToVOP2(ins);
			break;
		}
		return result;
	}

}  // namespace sce::gcn
