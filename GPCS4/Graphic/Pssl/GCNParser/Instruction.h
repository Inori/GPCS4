//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//=============================================================

#ifndef __INSTRUCTION_H
#define __INSTRUCTION_H

#ifdef _WIN32
    #include <cstdint>
#endif

#ifndef _WIN32
    #include <stdint.h>
#endif

// C++.
#include <math.h>
#include <string>
#include <unordered_map>

#include "DeviceInfo.h"


const int NO_LABEL = -1;

#define GENERIC_INSTRUCTION_FIELDS_1(in,val) \
    X_RANGE(ScalarGPRMin,ScalarGPRMax,ScalarGPR,in,val) \
    X_RANGE(Reserved104,Reserved105,Reserved,in,val) \
    X(VccLo,in) \
    X(VccHi,in) \
    X(TbaLo,in) \
    X(TbaHi,in) \
    X(TmaLo,in) \
    X(TmaHi,in) \
    X_RANGE(TtmpMin,TtmpMax,Ttmp,in,val) \
    X(M0,in) \
    X_RANGE(Reserved125,Reserved125,Reserved,in,val) \
    X(ExecLo,in) \
    X(ExecHi,in) \

#define GENERIC_INSTRUCTION_FIELDS_2(in) \
    X_RANGE(Reserved209,Reserved239,Reserved,in) \
    X_RANGE(Reserved248,Reserved250,Reserved,in) \
    X(VCCZ,in)\
    X(EXECZ,in)\
    X(SCC,in) \
    X_RANGE(LdsDirect,LdsDirect,Reserved,in) \

#define SCALAR_INSTRUCTION_FIELDS(in) \
    X(ConstZero,in) \
    X_RANGE(SignedConstIntPosMin,SignedConstIntPosMax,SignedConstIntPos,in) \
    X_RANGE(SignedConstIntNegMin,SignedConstIntNegMax,SignedConstIntNeg,in) \
    X(ConstFloatPos_0_5,in) \
    X(ConstFloatNeg_0_5,in) \
    X(ConstFloatPos_1_0,in) \
    X(ConstFloatNeg_1_0,in) \
    X(ConstFloatPos_2_0,in) \
    X(ConstFloatNeg_2_0,in) \
    X(ConstFloatPos_4_0,in) \
    X(ConstFloatNeg_4_0,in) \
    X(LiteralConst,in)

#define INSTRUCTION_FIELD(hexInstruction, insName, fieldName, fieldOffset) \
            ((hexInstruction & insName##Mask_##fieldName) >> fieldOffset)

#define INSTRUCTION32_FIELD(hexInstruction, insName, fieldName, fieldOffset) \
            ((hexInstruction & static_cast<Instruction::instruction32bit>(insName##Mask_##fieldName)) >> fieldOffset)

#define INSTRUCTION64_FIELD(hexInstruction, insName, fieldName, fieldOffset) \
            ((hexInstruction & (static_cast<Instruction::instruction64bit>(insName##Mask_##fieldName) << 32)) >> fieldOffset)

#define EXTRACT_INSTRUCTION32_FIELD(hexInstruction,genName,insName,fieldVar,fieldName,fieldOffset) \
    genName##insName##Instruction::fieldName fieldVar = static_cast<genName##insName##Instruction::fieldName>(INSTRUCTION32_FIELD(hexInstruction, insName, fieldName, fieldOffset));

#define EXTRACT_INSTRUCTION64_FIELD(hexInstruction,insName,fieldVar,fieldName,fieldOffset) \
    insName##Instruction::fieldName fieldVar = static_cast<insName##Instruction::fieldName>(INSTRUCTION64_FIELD(hexInstruction, insName, fieldName, fieldOffset));

#define RETURN_EXTRACT_INSTRUCTION(fieldVar) \
    return fieldVar






    /// ISA Instruction
    /// Defines all possible instruction categories [InstructionCategory] and
    /// defines general data/functionality for all instruction`s kinds
    class Instruction
    {
    public:


#define X(SYM) SRC##SYM
#define X_INIT(SYM,VAL) SRC##SYM = VAL
		/// Destination for scalar memory read instruction
		enum class OperandSRC : unsigned short
		{
#include "GenericInstructionFields1.h"
#include "ScalarInstructionFields.h"
#include "GenericInstructionFields2.h"
			X(Illegal)
		};
#undef X_INIT
#undef X

#define X(SYM) SDST##SYM
#define X_INIT(SYM,VAL) SDST##SYM = VAL
		/// Destination for SDST
		enum class OperandSDST : unsigned short
		{
#include "GenericInstructionFields1.h"
			X(Illegal)
		};
#undef X_INIT
#undef X

        /// SI instruction`s microcode formats
        enum InstructionSet
        {
            InstructionSet_SOP2,
            InstructionSet_SOPK,
            InstructionSet_SOP1,
            InstructionSet_SOPC,
            InstructionSet_SOPP,
            InstructionSet_VOP,
            InstructionSet_VOP2,
            InstructionSet_VOP1,
            InstructionSet_VOPC,
            InstructionSet_VOP3,
            InstructionSet_SMRD, //SI+CI Only
            InstructionSet_VINTRP,
            InstructionSet_DS,
            InstructionSet_MUBUF,
            InstructionSet_MTBUF,
            InstructionSet_MIMG,
            InstructionSet_EXP,
            InstructionSet_SMEM, // VI Only, was SMRD
            InstructionSet_FLAT
        };

        /// Instruction`s format kinds
        enum InstructionCategory
        {
			CategoryUnknown,
			/// Scalar ALU Operation
			ScalarALU,
            /// Scalar Instruction Memory
            ScalarMemory,
			/// Vector ALU Operation
			VectorALU,
            /// Vector Instruction Memory Read
            VectorMemory,
			/// Scalar Program Flow Control and Special Operations
			FlowControl,
            /// LDS and GDS
            DataShare,
			/// Vector Interpolation Operations
			VectorInterpolation,
			/// Export
			Export,
			/// Debug and Profiling Operations
			DebugProfile,
            /// Amount of type
            InstructionsCategoriesCount
        };

		/// Instruction`s format kinds
		enum InstructionClass
		{
			InstructionClassUnknown,

			ScalarArith,
			ScalarAbs,
			ScalarMov,
			ScalarCmp,
			ScalarSelect,
			ScalarBitLogic,
			ScalarBitManip,
			ScalarBitField,
			ScalarConv,
			ScalarExecMask,
			ScalarQuadMask,
			
			VectorRegMov,
			VectorLane,
			VectorBitLogic,
			VectorBitField32,
			VectorThreadMask,
			VectorBitField64,
			VectorFpArith32,
			VectorFpRound32,
			VectorFpField32,
			VectorFpTran32,
			VectorFpCmp32,
			VectorFpArith64,
			VectorFpRound64,
			VectorFpField64,
			VectorFpTran64,
			VectorFpCmp64,
			VectorIntArith32,
			VectorIntArith64,
			VectorIntCmp32,
			VectorIntCmp64,
			VectorConv,
			VectorFpGraph32,
			VectorIntGraph,
			VectorMisc,

			ScalarProgFlow,
			ScalarSync,
			ScalarWait,
			ScalarCache,
			ScalarPrior,
			ScalarRegAccess,
			ScalarMsg,

			ScalarMemRd,
			ScalarMemUt,

			VectorMemBufNoFmt,
			VectorMemBufFmt,
			VectorMemImgNoSmp,
			VectorMemImgSmp,
			VectorMemImgUt,
			VectorMemL1Cache,

			DsIdxRd,
			DsIdxWr,
			DsIdxWrXchg,
			DsIdxCondXchg,
			DsIdxWrap,
			DsAtomicArith32,
			DsAtomicArith64,
			DsAtomicMinMax32,
			DsAtomicMinMax64,
			DsAtomicCmpSt32,
			DsAtomicCmpSt64,
			DsAtomicLogic32,
			DsAtomicLogic64,
			DsAppendCon,
			DsDataShareUt,
			DsDataShareMisc,
			GdsSync,
			GdsOrdCnt,

			VectorInterpFpCache,

			Exp,

			DbgProf,

			InstructionClassCount
		};

		enum OperandType
		{
			TypeNone,
			TypeB8,
			TypeB16,
			TypeB32,
			TypeB64,
			TypeB96,
			TypeB128,
			TypeF16,
			TypeF32,
			TypeF64,
			TypeU8,
			TypeU16,
			TypeU24,
			TypeU32,
			TypeU64,
			TypeI4,
			TypeI8,
			TypeI16,
			TypeI24,
			TypeI32,
			TypeI64,
		};

        /// 32 bit instructions
        typedef uint32_t instruction32bit;

        /// 64 bit instruction
        typedef uint64_t instruction64bit;

        //
        // Public member functions
        //

        /// ctor
        Instruction(unsigned int instructionWidth, 
			InstructionCategory instructionFormatKind, 
			InstructionSet instructionFormat, 
			InstructionClass instructionClass = InstructionClassUnknown,
			OperandType instructionOperandType = TypeNone);

        /// dtor
        virtual ~Instruction() {}

        /// Get an instruction`s width in bits.
        /// \returns            a instruction`s width in bits.
        unsigned int GetInstructionWidth() const { return m_instructionWidth; }

		/// Get an instruction`s length in bytes.
		/// \returns            a instruction`s length in bytes.
		unsigned int GetInstructionLength() const { return m_instructionWidth / 8; }

        /// -----------------------------------------------------------------------------------------------
        /// \brief Name:        GetInstructionFormatKind
        /// \brief Description: Get Instruction`s format kind
        /// \return InstructionFormatKind
        /// -----------------------------------------------------------------------------------------------
        InstructionCategory GetInstructionCategory() const { return m_instructionCategory; }

		/// -----------------------------------------------------------------------------------------------
		/// \brief Name:        GetInstructionClass
		/// \brief Description: Get Instruction`s class type
		/// \return InstructionClass
		/// -----------------------------------------------------------------------------------------------
		InstructionClass GetInstructionClass() const;

		/// -----------------------------------------------------------------------------------------------
		/// \brief Name:        GetInstructionOperandWidth
		/// \brief Description: Get Instruction operand's width,
		///						e.g. xxx_b32 is 32, xxx_i64 is 64
		/// \return Instruction operand's width
		/// -----------------------------------------------------------------------------------------------
		OperandType GetInstructionOperandType() const;

        /// -----------------------------------------------------------------------------------------------
        /// \brief Name:        GetInstructionFormat
        /// \brief Description: Get Instruction`s Format
        /// \return InstructionSet
        /// -----------------------------------------------------------------------------------------------
		InstructionSet GetInstructionFormat() const { return m_instructionFormat; }

	protected:
		//
		// Data.
		//

		/// Instruction`s width in bits.
		unsigned int m_instructionWidth;

		/// Instruction`s format kind
		InstructionCategory m_instructionCategory;

		/// Instruction`s format
		InstructionSet m_instructionFormat;

		GDT_HW_GENERATION m_HwGen;

		/// Instruction`s class
		InstructionClass m_instructionClass;

		OperandType m_instructionOperandType;
    };


#endif //__INSTRUCTION_H

