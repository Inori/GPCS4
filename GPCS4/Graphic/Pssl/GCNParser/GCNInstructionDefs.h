#pragma once

#include "Instruction.h"
#include "SMRDInstruction.h"
#include "SOPPInstruction.h"
#include "SOPCInstruction.h"
#include "SOP1Instruction.h"
#include "SOPKInstruction.h"
#include "SOP2Instruction.h"
#include "VINTRPInstruction.h"
#include "DSInstruction.h"
#include "MUBUFInstruction.h"
#include "MTBUFInstruction.h"
#include "MIMGInstruction.h"
#include "EXPInstruction.h"
#include "VOPInstruction.h"

#include <unordered_map>


struct GCNInstructionFormat
{
	// From GPU Shader Core ISA manual
	Instruction::InstructionClass insClass;
	// Instruction operand's bit width
	Instruction::OperandType operandType;

	// TODO:
	// Add other meta data if needed.
};


extern std::unordered_map<SISOP1Instruction::OP, GCNInstructionFormat> g_instructionFormatMapSOP1;
extern std::unordered_map<SISOP2Instruction::OP, GCNInstructionFormat> g_instructionFormatMapSOP2;
extern std::unordered_map<SISOPCInstruction::OP, GCNInstructionFormat> g_instructionFormatMapSOPC;
extern std::unordered_map<SISOPKInstruction::OP, GCNInstructionFormat> g_instructionFormatMapSOPK;
extern std::unordered_map<SISOPPInstruction::OP, GCNInstructionFormat> g_instructionFormatMapSOPP;
extern std::unordered_map<SIVOP1Instruction::VOP1_OP, GCNInstructionFormat> g_instructionFormatMapVOP1;
extern std::unordered_map<SIVOP2Instruction::VOP2_OP, GCNInstructionFormat> g_instructionFormatMapVOP2;
extern std::unordered_map<SIVOP3Instruction::VOP3_OP, GCNInstructionFormat> g_instructionFormatMapVOP3;
extern std::unordered_map<SIVOPCInstruction::VOPC_OP, GCNInstructionFormat> g_instructionFormatMapVOPC;
extern std::unordered_map<SIMUBUFInstruction::OP, GCNInstructionFormat> g_instructionFormatMapMUBUF;
extern std::unordered_map<SIMTBUFInstruction::OP, GCNInstructionFormat> g_instructionFormatMapMTBUF;
extern std::unordered_map<SIMIMGInstruction::OP, GCNInstructionFormat> g_instructionFormatMapMIMG;
extern std::unordered_map<SIDSInstruction::OP, GCNInstructionFormat> g_instructionFormatMapDS;
extern std::unordered_map<SISMRDInstruction::OP, GCNInstructionFormat> g_instructionFormatMapSMRD;
extern std::unordered_map<SIVINTRPInstruction::OP, GCNInstructionFormat> g_instructionFormatMapVINTRP;