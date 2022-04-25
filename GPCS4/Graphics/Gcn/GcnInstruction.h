#pragma once

#include "GcnEnum.h"
#include "UtilFlag.h"

#include <variant>

namespace sce::gcn
{

	constexpr uint32_t GcnMaxSrcCount = 4;
	constexpr uint32_t GcnMaxDstCount = 2;

	struct GcnInstFormat
	{
		GcnInstClass    instructionClass    = GcnInstClass::Undefined;
		GcnInstCategory instructionCategory = GcnInstCategory::Undefined;

		uint32_t srcCount = 0;
		uint32_t dstCount = 0;

		GcnScalarType srcType = GcnScalarType::Undefined;
		GcnScalarType dstType = GcnScalarType::Undefined;
	};

	/**
	 * \brief Input modifiers
	 *
	 * These are applied after loading
	 * an operand register.
	 */
	enum class GcnInputModifier : uint32_t
	{
		Neg = 0,
		Abs = 1,
	};

	using GcnInputModifiers = util::Flags<GcnInputModifier>;

	/**
	 * \brief Output modifiers
	 *
	 * These are applied before storing
	 * an operand register.
	 */
	struct GcnOutputModifiers
	{
		bool  clamp      = false;
		float multiplier = std::numeric_limits<float>::quiet_NaN();
	};

	struct GcnInstOperand
	{
		GcnOperandField field = GcnOperandField::Undefined;
		GcnScalarType   type  = GcnScalarType::Undefined;

		GcnInputModifiers  inputModifier  = {};
		GcnOutputModifiers outputModifier = {};

		union
		{
			uint32_t code = 0xFFFFFFFF;
			uint32_t literalConst;
		};
	};

	struct GcnInstControlSOPK
	{
		uint32_t simm : 16;
		uint32_t : 16;
	};

	struct GcnInstControlSOPP
	{
		uint32_t simm : 16;
		uint32_t : 16;
	};

	struct GcnInstControlVOP3
	{
		uint64_t : 8;
		uint64_t abs : 3;
		uint64_t clmp : 1;
		uint64_t : 47;
		uint64_t omod : 2;
		uint64_t neg : 3;
	};

	struct GcnInstControlSMRD
	{
		uint32_t offset : 8;
		uint32_t imm : 1;
		uint32_t count : 5;
		uint32_t : 18;
	};

	struct GcnInstControlMUBUF
	{
		uint64_t offset : 12;
		uint64_t offen : 1;
		uint64_t idxen : 1;
		uint64_t glc : 1;
		uint64_t : 1;
		uint64_t lds : 1;
		uint64_t : 37;
		uint64_t slc : 1;
		uint64_t tfe : 1;
		uint64_t count : 3;
		uint64_t : 5;
	};

	struct GcnInstControlMTBUF
	{
		uint64_t offset : 12;
		uint64_t offen : 1;
		uint64_t idxen : 1;
		uint64_t glc : 1;
		uint64_t : 4;
		uint64_t dfmt : 4;
		uint64_t nfmt : 3;
		uint64_t : 28;
		uint64_t slc : 1;
		uint64_t tfe : 1;
		uint64_t count : 3;
		uint64_t : 5;
	};

	struct GcnInstControlMIMG
	{
		uint64_t : 8;
		uint64_t dmask : 4;
		uint64_t unrm : 1;
		uint64_t glc : 1;
		uint64_t da : 1;
		uint64_t r128 : 1;
		uint64_t tfe : 1;
		uint64_t lwe : 1;
		uint64_t : 7;
		uint64_t slc : 1;
		uint64_t : 38;
	};

	struct GcnInstControlDS
	{
		uint64_t offset0 : 8;
		uint64_t offset1 : 8;
		uint64_t : 1;
		uint64_t gds : 1;
		uint64_t : 46;
	};

	struct GcnInstControlVINTRP
	{
		uint32_t : 8;
		uint32_t chan : 2;
		uint32_t attr : 6;
		uint32_t : 16;
	};

	struct GcnInstControlEXP
	{
		uint64_t en : 4;
		uint64_t target : 6;
		uint64_t compr : 1;
		uint64_t done : 1;
		uint64_t vm : 1;
		uint64_t reserved : 51;
	};

	union GcnInstControl
	{
		GcnInstControlSOPK   sopk;
		GcnInstControlSOPP   sopp;
		GcnInstControlVOP3   vop3;
		GcnInstControlSMRD   smrd;
		GcnInstControlMUBUF  mubuf;
		GcnInstControlMTBUF  mtbuf;
		GcnInstControlMIMG   mimg;
		GcnInstControlDS     ds;
		GcnInstControlVINTRP vintrp;
		GcnInstControlEXP    exp;
	};

	struct GcnShaderInstruction
	{
		GcnOpcode       opcode;
		uint32_t        length;  // in bytes
		GcnInstEncoding encoding;
		GcnInstClass    opClass;
		GcnInstCategory category;
		GcnInstControl  control;

		uint32_t srcCount;
		uint32_t dstCount;

		GcnInstOperand src[GcnMaxSrcCount];
		GcnInstOperand dst[GcnMaxDstCount];
	};

	struct GcnShaderInstSOP1
	{
		GcnOpcode           opcode;
		uint32_t            length;  // in bytes
		GcnInstClass        opClass;

		GcnInstOperand ssrc0;
		GcnInstOperand sdst;
	};

	struct GcnShaderInstSOP2
	{
		GcnOpcode           opcode;
		uint32_t            length;  // in bytes
		GcnInstClass        opClass;

		GcnInstOperand ssrc0;
		GcnInstOperand ssrc1;
		GcnInstOperand sdst;
	};

	struct GcnShaderInstSOPK
	{
		GcnOpcode           opcode;
		uint32_t            length;  // in bytes
		GcnInstClass        opClass;
		GcnInstControlSOPK  control;

		GcnInstOperand sdst;
	};

	struct GcnShaderInstSOPC
	{
		GcnOpcode          opcode;
		uint32_t           length;  // in bytes
		GcnInstClass       opClass;

		GcnInstOperand ssrc0;
		GcnInstOperand ssrc1;
	};

	struct GcnShaderInstSOPP
	{
		GcnOpcode          opcode;
		uint32_t           length;  // in bytes
		GcnInstClass       opClass;
		GcnInstControlSOPP control;
	};

	struct GcnShaderInstVOP1
	{
		GcnOpcode          opcode;
		uint32_t           length;  // in bytes
		GcnInstClass       opClass;

		GcnInstOperand src0;
		GcnInstOperand vdst;
	};

	struct GcnShaderInstVOP2
	{
		GcnOpcode          opcode;
		uint32_t           length;  // in bytes
		GcnInstClass       opClass;

		GcnInstOperand src0;
		GcnInstOperand vsrc1;
		GcnInstOperand vdst;
	};

	struct GcnShaderInstVOP3
	{
		GcnOpcode          opcode;
		uint32_t           length;  // in bytes
		GcnInstClass       opClass;
		GcnInstControlVOP3 control;

		GcnInstOperand src0;
		GcnInstOperand src1;
		GcnInstOperand src2;
		GcnInstOperand vdst;
		GcnInstOperand sdst;
	};

	struct GcnShaderInstVOPC
	{
		GcnOpcode          opcode;
		uint32_t           length;  // in bytes
		GcnInstClass       opClass;

		GcnInstOperand src0;
		GcnInstOperand vsrc1;
	};

	struct GcnShaderInstSMRD
	{
		GcnOpcode           opcode;
		uint32_t            length;  // in bytes
		GcnInstClass        opClass;
		GcnInstControlSMRD  control;

		GcnInstOperand offset;
		GcnInstOperand sbase;
		GcnInstOperand sdst;
	};

	struct GcnShaderInstMUBUF
	{
		GcnOpcode           opcode;
		uint32_t            length;  // in bytes
		GcnInstClass        opClass;
		GcnInstControlMUBUF control;

		GcnInstOperand vaddr;
		GcnInstOperand vdata;
		GcnInstOperand srsrc;
		GcnInstOperand soffset;
	};

	struct GcnShaderInstMTBUF
	{
		GcnOpcode           opcode;
		uint32_t            length;  // in bytes
		GcnInstClass        opClass;
		GcnInstControlMTBUF control;

		GcnInstOperand vaddr;
		GcnInstOperand vdata;
		GcnInstOperand srsrc;
		GcnInstOperand soffset;
	};

	struct GcnShaderInstMIMG
	{
		GcnOpcode           opcode;
		uint32_t            length;  // in bytes
		GcnInstClass        opClass;
		GcnInstControlMIMG  control;

		GcnInstOperand vaddr;
		GcnInstOperand vdata;
		GcnInstOperand srsrc;
		GcnInstOperand ssamp;
	};

	struct GcnShaderInstVINTRP
	{
		GcnOpcode            opcode;
		uint32_t             length;  // in bytes
		GcnInstClass         opClass;
		GcnInstControlVINTRP control;

		GcnInstOperand vsrc;
		GcnInstOperand vdst;
	};

	struct GcnShaderInstDS
	{
		GcnOpcode            opcode;
		uint32_t             length;  // in bytes
		GcnInstClass         opClass;
		GcnInstControlDS     control;

		GcnInstOperand addr;
		GcnInstOperand data0;
		GcnInstOperand data1;
		GcnInstOperand vdst;
	};

	struct GcnShaderInstEXP
	{
		GcnOpcode            opcode;
		uint32_t             length;  // in bytes
		GcnInstClass         opClass;
		GcnInstControlEXP    control;
		GcnExportTarget      target;

		GcnInstOperand vsrc0;
		GcnInstOperand vsrc1;
		GcnInstOperand vsrc2;
		GcnInstOperand vsrc3;
	};

	GcnInstFormat gcnInstructionFormat(GcnInstEncoding encoding, uint32_t opcode);

}  // namespace sce::gcn
