//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//=============================================================

/// In SI ISA different instructions ,can define for different field`s names the set of
/// the (same) generic instructions fields bits values.
/// For example :
//        Generic bits values ,which an instruction SMRD defines for the field SDST.
//        Generic bits values ,which an instruction SOPK defines for the field SIMM16.
///
/// GenericInstructionFiels1.h is used to generate instruction and instruction`s field specific generic bits values.
/// It uses two macros X_INIT and X macros for that purpose.The macros are defined in the appropriate instruction`s class.
/// For example :
///       For instruction SMRD ,SMRDInstruction defines the field SDST in the following way -
///                            #define X(SYM) SDST##SYM
///                            #define X_INIT(SYM,VAL) SDST##SYM = VAL
///                            enum SDST
///                            {
///                                 #include "GenericInstructionFields1.h"
///                                 //some definitions
///                             };
///                            #undef X_INIT
///                            #undef X
///       For instruction SOPK ,SOPKInstruction defines the field SIMM16 in the following way -
///                            #define X(SYM) SIMM16##SYM
///                            #define X_INIT(SYM,VAL) SIMM16##SYM = VAL
///                            enum SIMM16
///                            {
///                                 #include "GenericInstructionFields1.h"
///                                 //some definitions
///                             };
///                            #undef X_INIT
///                            #undef X
/// As a result of the preprocessing the following is generated:
///       For SMRDInstruction -
///       enum SDST
///       {
///           SDSTScalarGPRMin = 0,
///           SDSTScalarGPR,
///           SDSTScalarGPRMax = 103,
///           SDSTReserved104,
///           SDSTReserved105,
///           SDSTReserved = 2,
///           SDSTVccLo = 106,
///           SDSTVccHi,
///           SDSTTbaLo,
///           SDSTTbaHi,
///           SDSTTmaLo,
///           SDSTTmaHi,
///           SDSTTtmpMin = 112,
///           SDSTTtmp,
///           SDSTTtmpMax = 123,
///           SDSTM0,
///           SDSTReserved125,
///           SDSTExecLo = 126,
///           SDSTExecHi,
///           //some definitions
///        };
///        For SOPKInstruction -
///         enum SIMM16
///         {
///           SIMM16ScalarGPRMin = 0,
///           SIMM16ScalarGPR,
///           SIMM16ScalarGPRMax = 103,
///           SIMM16Reserved104,
///           SIMM16Reserved105,
///           SIMM16Reserved = 2,
///           SIMM16VccLo = 106,
///           SIMM16VccHi,
///           SIMM16TbaLo,
///           SIMM16TbaHi,
///           SIMM16TmaLo,
///           SIMM16TmaHi,
///           SIMM16TtmpMin = 112,
///           SIMM16Ttmp,
///           SIMM16TtmpMax = 123,
///           SIMM16M0,
///           SIMM16Reserved125,
///           SIMM16ExecLo = 126,
///           SIMM16ExecHi,
///           //some definitions
///        };

/// SGPR0 to SGPR103: Scalar general-purpose registers.
X_INIT(ScalarGPRMin, 0),
       X(ScalarGPR),
       X_INIT(ScalarGPRMax, 103),
       /// 104 ?105 reserved.
       X(Reserved104),
       X(Reserved105),
       X_INIT(Reserved, 2),
       /// vcc[31:0].
       X_INIT(VccLo, 106),
       /// vcc[63:32].
       X(VccHi),
       /// Trap handler base address [31:0].
       X(TbaLo),
       /// Trap handler base address [63:32].
       X(TbaHi),
       /// Pointer to data in memory used by trap handler.
       X(TmaLo),
       /// Pointer to data in memory used by trap handler.
       X(TmaHi),
       /// TTMP0 to TTMP11: Trap handler temporary registers (privileged).
       X_INIT(TtmpMin, 112),
       X(Ttmp),
       X_INIT(TtmpMax, 123),
       /// Memory register 0.
       X(M0),
       /// 125 reserved.
       X(Reserved125),
       /// exec[31:0].
       X_INIT(ExecLo, 126),
       /// exec[63:32].
       X(ExecHi),
