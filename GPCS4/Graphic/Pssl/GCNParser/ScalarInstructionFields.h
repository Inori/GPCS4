//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//=============================================================

/// In SI ISA different instructions ,can define for different field`s names the set of
/// the (same) scalar constant bits values.
/// For example :
//        Constant bits values ,which an instruction SOPC defines for the field SSRC0.
//        Constant bits values ,which an instruction SOPK defines for the field SIMM16.
///
/// ScalarInstructionFiels.h is used to generate instruction and instruction`s field specific constant bits values.
/// It uses two macros X_INIT and X macros for that purpose.The macros are defined in the appropriate instruction`s class.
/// For example :
///       For instruction SOPC ,SOPCInstruction defines the field SSRC in the following way -
///                            #define X(SYM) SSRC##SYM
///                            #define X_INIT(SYM,VAL) SSRC##SYM = VAL
///                            enum SSRC
///                            {
///                                 //some defintions
///                                 #include "ScalarInstructionFields.h"
///                                 //some definitions
///                             };
///                            #undef X_INIT
///                            #undef X
///       For instruction SOPK ,SOPKInstruction defines the field SIMM16 in the following way -
///                            #define X(SYM) SIMM16##SYM
///                            #define X_INIT(SYM,VAL) SIMM16##SYM = VAL
///                            enum SIMM16
///                            {
///                                 //some defintions
///                                 #include "ScalarInstructionFields.h"
///                                 //some definitions
///                             };
///                            #undef X_INIT
///                            #undef X
/// As a result of the preprocessing the following is generated:
///       For SOPCInstruction -
///       enum SSRC
///       {
///            //some defintions
///            SSRCConstZero = 128,
///            SSRCSignedConstIntPosMin = 129,
///            SSRCSignedConstIntPos,
///            SSRCSignedConstIntPosMax = 192,
///            SSRCSignedConstIntNegMin = 193,
///            SSRCSignedConstIntNeg,
///            SSRCSignedConstIntNegMax = 208,
///            SSRCConstFloatPos_0_5 = 240,
///            SSRCConstFloatNeg_0_5,
///            SSRCConstFloatPos_1_0,
///            SSRCConstFloatNeg_1_0,
///            SSRCConstFloatPos_2_0,
///            SSRCConstFloatNeg_2_0,
///            SSRCConstFloatPos_4_0,
///            SSRCConstFloatNeg_4_0,
///            SSRCLiteralConst = 255,
///            //some definitions
///        };
///        For SOPKInstruction -
///         enum SIMM16
///         {
///            //some defintions
///            SIMM16ConstZero = 128,
///            SIMM16SignedConstIntPosMin = 129,
///            SIMM16SignedConstIntPos,
///            SIMM16SignedConstIntPosMax = 192,
///            SIMM16SignedConstIntNegMin = 193,
///            SIMM16SignedConstIntNeg,
///            SIMM16SignedConstIntNegMax = 208,
///            SIMM16ConstFloatPos_0_5 = 240,
///            SIMM16ConstFloatNeg_0_5,
///            SIMM16ConstFloatPos_1_0,
///            SIMM16ConstFloatNeg_1_0,
///            SIMM16ConstFloatPos_2_0,
///            SIMM16ConstFloatNeg_2_0,
///            SIMM16ConstFloatPos_4_0,
///            SIMM16ConstFloatNeg_4_0,
///            SIMM16LiteralConst = 255,
///            ///some definitions
///        };

/// 0
X_INIT(ConstZero, 128),
       /// Signed integer 1 to 64.
       X_INIT(SignedConstIntPosMin, 129),
       X(SignedConstIntPos),
       X_INIT(SignedConstIntPosMax, 192),
       /// Signed integer -1 to -16.
       X_INIT(SignedConstIntNegMin, 193),
       X(SignedConstIntNeg),
       X_INIT(SignedConstIntNegMax, 208),
       /// 0.5.
       X_INIT(ConstFloatPos_0_5, 240),
       /// -0.5.
       X(ConstFloatNeg_0_5),
       /// 1.0.
       X(ConstFloatPos_1_0),
       /// -1.0.
       X(ConstFloatNeg_1_0),
       /// 2.0.
       X(ConstFloatPos_2_0),
       /// -2.0.
       X(ConstFloatNeg_2_0),
       /// 4.0.
       X(ConstFloatPos_4_0),
       /// -4.0
       X(ConstFloatNeg_4_0),
       /// Literal constant.
       X_INIT(LiteralConst, 255),


