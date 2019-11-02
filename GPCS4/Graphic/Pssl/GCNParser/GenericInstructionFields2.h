//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//=============================================================

/// In SI ISA different instructions ,can define for different field`s names the set of
/// the (same) generic instructions fields bits values.
/// For example :
//        Generic bits values ,which an instruction SOP2 defines for the field SSRC0.
//        Generic bits values ,which an instruction SOPK defines for the field SIMM16.
///
/// GenericInstructionFiels2.h is used to generate instruction and instruction`s field specific generic bits values.
/// It uses two macros X_INIT and X macros for that purpose.The macros are defined in the appropriate instruction`s class.
/// For example :
///       For instruction SOP2 ,SOP2Instruction defines the field SSRC in the following way -
///                            #define X(SYM) SSRC##SYM
///                            #define X_INIT(SYM,VAL) SSRC##SYM = VAL
///                            enum SSRC
///                            {
///                                 //some definitions
///                                 #include "GenericInstructionFields2.h"
///                                 //some definitions
///                             };
///                            #undef X_INIT
///                            #undef X
///       For instruction SOPK ,SOPKInstruction defines the field SIMM16 in the following way -
///                            #define X(SYM) SIMM16##SYM
///                            #define X_INIT(SYM,VAL) SIMM16##SYM = VAL
///                            enum SIMM16
///                            {
///                                 //some definitions
///                                 #include "GenericInstructionFields2.h"
///                                 //some definitions
///                             };
///                            #undef X_INIT
///                            #undef X
/// As a result of the preprocessing the following is generated:
///       For SOP2Instruction -
///       enum SSRC
///       {
///           //some definitions
///           SSRCReserved209 = 209,
///           SSRCReserved239 = 239,
///           SSRCReserved248 = 248,
///           SSRCReserved249,
///           SSRCReserved250,
///           SSRCVCCZ,
///           SSRCEXECZ,
///           SSRCSCC,
///           SSRCReserved254,
///           //some definitions
///        };
///        For SOPKInstruction -
///         enum SIMM16
///         {
///           //some definitions
///           SIMM16Reserved209 = 209,
///           SIMM16Reserved239 = 239,
///           SIMM16Reserved248 = 248,
///           SIMM16Reserved249,
///           SIMM16Reserved250,
///           SIMM16VCCZ,
///           SIMM16EXECZ,
///           SIMM16SCC,
///           SIMM16Reserved254,
///           //some definitions
///        };

/// 209 ?239 reserved.
X_INIT(Reserved209, 209),
       X_INIT(Reserved239, 239),
       /// 248 ?250 reserved.
       X_INIT(Reserved248, 248),
       X(Reserved249),
       X(Reserved250),
       /// VCCZ.
       X(VCCZ),
       /// EXECZ.
       X(EXECZ),
       /// SCC.
       X(SCC),
       /// 254 reserved.
       X(LdsDirect),
	   /// 9 bits SRC vgprs
	   X_INIT(VectorGPRMin, 256),
	   X_INIT(VectorGPRMax, 511),
