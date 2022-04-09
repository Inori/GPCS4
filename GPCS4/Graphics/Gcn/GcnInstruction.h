#pragma once

#include "GcnEnum.h"

namespace sce::gcn
{

	constexpr uint32_t GcnMaxSrcCount = 4;
	constexpr uint32_t GcnMaxDstCount = 2;

	struct GcnInstFormat
	{
		// Instruction class
		GcnInstClass instructionClass = GcnInstClass::Undefined;

		uint32_t srcCount = 0;
		uint32_t dstCount = 0;

		GcnNumericType srcType = GcnNumericType::Undefined;
		GcnNumericType dstType = GcnNumericType::Undefined;
	};

	struct GcnInstOperand
	{
		GcnOperandField field       = GcnOperandField::Undefined;
		GcnNumericType  numericType = GcnNumericType::Undefined;
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
		GcnInstControl  control;

		uint32_t srcCount;
		uint32_t dstCount;

		GcnInstOperand src[GcnMaxSrcCount];
		GcnInstOperand dst[GcnMaxDstCount];
	};

	GcnInstFormat gcnInstructionFormat(GcnInstEncoding encoding, uint32_t opcode);

}  // namespace sce::gcn
