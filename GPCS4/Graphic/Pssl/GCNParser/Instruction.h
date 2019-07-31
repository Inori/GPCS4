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


// Used only for debugging purpose.
// If defined, every Instruction class will contains extra strings and tables,
// which will reduce performance

//#define INSTRUCTION_STRING_REPRESENTATION 1



#ifdef INSTRUCTION_STRING_REPRESENTATION
#include "beStringConstants.h"
#endif  // INSTRUCTION_STRING_REPRESENTATION


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
    X_RANGE(Reserved254,Reserved254,Reserved,in) \

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
            /// LDS
            LDS,
            /// GDS
            GDS,
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
			ClassUnknown,

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

			DbgProf
		};

        /// -----------------------------------------------------------------------------------------------
        /// \brief Name:        GetFunctionalUnitAsString
        /// \brief Description: Translates Instruction`s functional unit to user friendly std::string
        /// \return std::string describing instruction's functional unit
        /// -----------------------------------------------------------------------------------------------
        static std::string GetFunctionalUnitAsString(InstructionCategory category);

        /// 32 bit instructions
        typedef uint32_t instruction32bit;

        /// 64 bit instruction
        typedef uint64_t instruction64bit;

        //
        // Public member functions
        //

        /// ctor
        Instruction(unsigned int instructionWidth, InstructionCategory instructionFormatKind, InstructionSet instructionFormat);

        /// dtor
        virtual ~Instruction() {}

        /// Get an instruction`s width in bits.
        /// \returns            a instruction`s width in bits.
        unsigned int GetInstructionWidth() const { return m_instructionWidth; }

        /// -----------------------------------------------------------------------------------------------
        /// \brief Name:        GetInstructionFormatKind
        /// \brief Description: Get Instruction`s format kind
        /// \return InstructionFormatKind
        /// -----------------------------------------------------------------------------------------------
        InstructionCategory GetInstructionCategory() const { return m_instructionCategory; }

        /// -----------------------------------------------------------------------------------------------
        /// \brief Name:        SetInstructionFormatKind
        /// \brief Description: Set the instruction`s format kind
        /// \return InstructionFormatKind
        /// -----------------------------------------------------------------------------------------------
        void SetInstructionCategory(InstructionCategory instructionType) { m_instructionCategory = instructionType; }

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


#ifdef INSTRUCTION_STRING_REPRESENTATION

	public:

		/// ctor for label instruction
		explicit Instruction(const std::string& labelString);

        int GetInstructionClockCount(const std::string& deviceName) const;

        /// The Instruction Asic HW generation. default is SI
        GDT_HW_GENERATION GetHwGen() const { return m_HwGen; }
        void SetHwGen(GDT_HW_GENERATION HwGen) { m_HwGen = HwGen; }

        // String representation of the instruction's opcode.
        const std::string& GetInstructionOpCode() const { return m_instructionOpCode; }

        // String representation of the instruction's parameters.
        const std::string& GetInstructionParameters() const { return m_parameters; }

        // String representation of the instruction's binary representation.
        const std::string& GetInstructionBinaryRep() const { return m_binaryInstruction; }

        // String representation of the instruction's offset within the program.
        const std::string& GetInstructionOffset() const { return m_offsetInBytes; }

        // Sets the string representation of the instruction's opcode.
        void SetInstructionOpCode(const std::string& opCode) { m_instructionOpCode = opCode; }

        // Sets the string representation of the instruction's parameters.
        void SetInstructionParameters(const std::string& params) { m_parameters = params; }

        // Sets the string representation of the instruction's binary representation.
        void SetInstructionBinaryRep(const std::string& binaryRep) { m_binaryInstruction = binaryRep; }

        // Sets the string representation of the instruction's offset within the program.
        void SetInstructionOffset(const std::string& offset) { m_offsetInBytes = offset; }

        // Sets the string representation of the instruction: opcode, parameters, binary representation and offset within the program.
        void SetInstructionStringRepresentation(const std::string& opCode,
                                                const std::string& params, const std::string& binaryRep, const std::string& offset);

        /// Returns pointing label string
        const std::string& GetPointingLabelString() const { return m_pointingLabelString; }

	protected:
		/// String representation of the instruction's opcode.
		std::string m_instructionOpCode;

    private:

        /// Initializes the performance tables.
        static void SetUpPerfTables();

        /// Initializes the hybrid architecture performance tables.
        static void SetUpHybridPerfTables();

        /// Initializes the scalar performance tables.
        static void SetUpScalarPerfTables();

        /// Initializes quarter devices performance tables.
        static void SetUpQuarterDevicesPerfTables();

        /// Initializes half devices performance tables.
        static void SetUpHalfDevicesPerfTables();

        /// String representation of the parameters.
        std::string m_parameters;

        /// String of the binary representation of the instruction (e.g. 0xC2078914).
        std::string m_binaryInstruction;

        /// String representation of the offset in bytes of the current instruction
        /// from the beginning of the program.
        std::string m_offsetInBytes;

        /// If this instruction is being pointed by a label, this member will hold the label.
        std::string m_pointingLabelString;

        /// Corresponding source line.
        std::string m_srcLine;

        /// Corresponding source line number.
        int m_srcLineNum;

        /// Indicates whether the performance tables were initialized or not.
        static bool m_s_IsPerfTablesInitialized;

        /// Holds the cycles per instruction for the 1/2 device architecture.
        static std::unordered_map<std::string, int> m_s_halfDevicePerfTable;

        /// Holds the cycles per instruction for the 1/4 device architecture.
        static std::unordered_map<std::string, int> m_s_quarterDevicePerfTable;

        /// Holds the cycles per instruction for the 1/16 device architecture.
        static std::unordered_map<std::string, int> m_s_hybridDevicePerfTable;

        /// Holds the cycles per instruction for the scalar instructions.
        static std::unordered_map<std::string, int> m_s_scalarPerfTable;

#endif  // INSTRUCTION_STRING_REPRESENTATION
    };


#endif //__INSTRUCTION_H

