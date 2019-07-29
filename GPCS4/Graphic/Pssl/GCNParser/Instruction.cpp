#include "Instruction.h"

// C++.
#include <algorithm>
#include <sstream>


#ifdef INSTRUCTION_STRING_REPRESENTATION
#include "rgaCliDefs.h"

// Static members definition.
std::unordered_map<std::string, int> Instruction::m_s_halfDevicePerfTable;
std::unordered_map<std::string, int> Instruction::m_s_quarterDevicePerfTable;
std::unordered_map<std::string, int> Instruction::m_s_hybridDevicePerfTable;
std::unordered_map<std::string, int> Instruction::m_s_scalarPerfTable;
bool Instruction::m_s_IsPerfTablesInitialized = false;

int Instruction::GetInstructionClockCount(const std::string& deviceName) const
{
    int ret = 0;

    if (!deviceName.empty())
    {
        // Ignore the case.
        std::string opCodeLowerCase(m_instructionOpCode);
        std::transform(opCodeLowerCase.begin(), opCodeLowerCase.end(), opCodeLowerCase.begin(), ::tolower);

        // One quarter-precision-speed devices.
        bool isQuarterDoubleDevice = (deviceName.compare(DEVICE_NAME_CYPRESS) == 0 ||
                                      deviceName.compare(DEVICE_NAME_CEDAR) == 0 ||
                                      deviceName.compare(DEVICE_NAME_TAHITI) == 0);

        // One half double-precision-speed devices.
        bool isHalfDoubleDevice = !isQuarterDoubleDevice && (deviceName.compare(DEVICE_NAME_HAWAII) == 0);

        // First look at the scalar performance table.
        auto devIter = m_s_scalarPerfTable.find(opCodeLowerCase);

        if (devIter != m_s_scalarPerfTable.end())
        {
            ret = devIter->second;
        }
        else
        {
            // If this is not a scalar operation, check the other tables.
            if (isQuarterDoubleDevice)
            {
                devIter = m_s_quarterDevicePerfTable.find(opCodeLowerCase);

                if (devIter != m_s_quarterDevicePerfTable.end())
                {
                    ret = devIter->second;
                }
            }
            else if (isHalfDoubleDevice)
            {
                devIter = m_s_halfDevicePerfTable.find(opCodeLowerCase);

                if (devIter != m_s_halfDevicePerfTable.end())
                {
                    ret = devIter->second;
                }
            }
            else
            {
                devIter = m_s_hybridDevicePerfTable.find(opCodeLowerCase);

                if (devIter != m_s_hybridDevicePerfTable.end())
                {
                    ret = devIter->second;
                }
            }
        }
    }

    return ret;
}

void Instruction::SetInstructionStringRepresentation(const std::string& opCode, const std::string& params, const std::string& binaryRep, const std::string& offset)
{
    m_instructionOpCode = opCode;
    std::transform(m_instructionOpCode.begin(), m_instructionOpCode.end(), m_instructionOpCode.begin(), ::tolower);

    m_parameters = params;
    m_binaryInstruction = binaryRep;
    m_offsetInBytes = offset;

    // Deduce the instruction category.
    if ((m_instructionOpCode.find(SOPP_S_COND_BRANCH_PREFIX) != std::string::npos) ||
        m_instructionOpCode == SOPP_S_BRANCH || m_instructionOpCode == SOPP_S_SETPC ||
        m_instructionOpCode == SOPP_S_SWAPPC)
    {
        m_instructionCategory = InstructionCategory::Branch;
    }
    else if (m_instructionOpCode == SOPP_S_ENDPGM ||
             m_instructionOpCode == OP_S_WAITCNT ||
             m_instructionOpCode == SOPP_S_NOP ||
             m_instructionOpCode == SOPP_S_TRAP ||
             m_instructionOpCode == SOPP_S_RFE ||
             m_instructionOpCode == SOPP_S_SETPRIO        ||
             m_instructionOpCode == SOPP_S_SLEEP          ||
             m_instructionOpCode == SOPP_S_SENDMSG)
    {
        m_instructionCategory = InstructionCategory::Internal;
    }
    else if (m_s_scalarPerfTable.find(m_instructionOpCode) != m_s_scalarPerfTable.end())
    {
        m_instructionCategory = InstructionCategory::ScalarALU;
    }
    else if (m_instructionOpCode.compare(EXPORT_EXP) == 0)
    {
        m_instructionCategory = InstructionCategory::Export;
    }
}

void Instruction::SetUpPerfTables()
{
    if (!m_s_IsPerfTablesInitialized)
    {
        // Scalar instructions.
        SetUpScalarPerfTables();

        // Hybrid architecture.
        SetUpHybridPerfTables();

        // Quarter double devices.
        SetUpQuarterDevicesPerfTables();

        // Half double devices.
        SetUpHalfDevicesPerfTables();

        // The tables have been initialized.
        m_s_IsPerfTablesInitialized = true;
    }
}

void Instruction::SetUpHybridPerfTables()
{
    // Initialize the hybrid architecture device table.
    if (m_s_hybridDevicePerfTable.empty())
    {
        // Full-rate.
        m_s_hybridDevicePerfTable[VOP2_NO_HW_IMPL_V_MAC_F32] = 4;
        m_s_hybridDevicePerfTable[VOP2_NO_HW_IMPL_V_MAC_LEGACY_F32] = 4;
        m_s_hybridDevicePerfTable[VOP2_NO_HW_IMPL_V_SUBBREV_U32] = 4;
        m_s_hybridDevicePerfTable[VOP2_NO_HW_IMPL_V_SUB_F32] = 4;
        m_s_hybridDevicePerfTable[VOP2_NO_HW_IMPL_V_SUBREV_F32] = 4;
        m_s_hybridDevicePerfTable[VOP2_NO_HW_IMPL_V_SUBREV_I32] = 4;
        m_s_hybridDevicePerfTable[VOP2_NO_HW_IMPL_V_MADMK_F32] = 4;
        m_s_hybridDevicePerfTable[VOP2_NO_HW_IMPL_V_MADAK_F32] = 4;
        m_s_hybridDevicePerfTable[VOP2_NO_HW_IMPL_V_SUBB_U32] = 4;
        m_s_hybridDevicePerfTable[VOP2_NO_HW_IMPL_V_LSHRREV_B32] = 4;
        m_s_hybridDevicePerfTable[VOP2_NO_HW_IMPL_V_ASHRREV_I32] = 4;
        m_s_hybridDevicePerfTable[VOP2_NO_HW_IMPL_V_LSHLREV_B32] = 4;
        m_s_hybridDevicePerfTable[VOP1_V_MOV_B32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_MOV_FED_B32] = 4;
        m_s_hybridDevicePerfTable[VOP1_V_NOT_B32] = 4;
        m_s_hybridDevicePerfTable[VOP1_V_CVT_F32_I32] = 4;
        m_s_hybridDevicePerfTable[VOP1_V_CVT_F32_U32] = 4;
        m_s_hybridDevicePerfTable[VOP1_V_CVT_U32_F32] = 4;
        m_s_hybridDevicePerfTable[VOP1_V_CVT_I32_F32] = 4;
        m_s_hybridDevicePerfTable[VOP1_V_CVT_RPI_I32_F32] = 4;
        m_s_hybridDevicePerfTable[VOP1_V_CVT_FLR_I32_F32] = 4;
        m_s_hybridDevicePerfTable[VOP1_V_CVT_OFF_F32_I4] = 4;
        m_s_hybridDevicePerfTable[VOP1_V_BFREV_B32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_MUL_I32_I24] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_MUL_HI_I32_I24] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_MUL_U32_U24] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_MUL_HI_U32_U24] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_MUL_F32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_MUL_LEGACY_F32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_ADD_F32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_ADD_I32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_ADDC_U32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_SUB_I32] = 4;
        m_s_hybridDevicePerfTable[VOP2_NO_DOC_V_ADD_U32] = 4;
        m_s_hybridDevicePerfTable[VOP2_NO_DOC_V_SUB_U32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_LSHL_B32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_XOR_B32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_OR_B32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_AND_B32] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_MAD_LEGACY_F32] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_FMA_F32] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_MULLIT_F32] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_ADD_F64] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_MAD_I32_I24] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_MAD_U32_U24] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_MAD_F32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_BFE_I32] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_ALIGNBIT_B32] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_ALIGNBYTE_B32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_BFM_B32] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_BFI_B32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_BCNT_U32_B32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_MBCNT_LO_U32_B32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_MBCNT_HI_U32_B32] = 4;
        m_s_hybridDevicePerfTable[VOP1_V_FFBH_U32] = 4;
        m_s_hybridDevicePerfTable[VOP1_V_FFBL_B32] = 4;
        m_s_hybridDevicePerfTable[VOP1_V_FFBH_I32] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_SAD_U8] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_MSAD_U8] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_SAD_HI_U8] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_SAD_U16] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_SAD_U32] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_LERP_U8] = 4;
        m_s_hybridDevicePerfTable[VOP1_V_CVT_F32_UBYTE0] = 4;
        m_s_hybridDevicePerfTable[VOP1_V_CVT_F32_UBYTE1] = 4;
        m_s_hybridDevicePerfTable[VOP1_V_CVT_F32_UBYTE2] = 4;
        m_s_hybridDevicePerfTable[VOP1_V_CVT_F32_UBYTE3] = 4;
        m_s_hybridDevicePerfTable[VOP1_V_CVT_OFF_F32_I4] = 4;
        m_s_hybridDevicePerfTable[VOP1_V_CVT_F16_F32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_CVT_PKRTZ_F16_F32] = 4;
        m_s_hybridDevicePerfTable[VOP1_V_CVT_F32_F16] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_CVT_PKNORM_I16_F32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_CVT_PKNORM_U16_F32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_CVT_PK_U16_U32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_CVT_PK_I16_I32] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_FMA_F32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_MUL_U32_U24] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_MUL_HI_U32_U24] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_MAD_U32_U24] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_MUL_I32_I24] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_MUL_HI_I32_I24] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_MAD_I32_I24] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_MULLIT_F32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_CNDMASK_B32] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_CUBEID_F32] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_CUBESC_F32] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_CUBETC_F32] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_CUBEMA_F32] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_MAX_F32] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_MAX_I32] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_MAX_U32] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_MIN3_F32] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_MIN3_I32] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_MIN3_U32] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_MED3_F32] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_MED3_I32] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_MED3_U32] = 4;
        m_s_hybridDevicePerfTable[VOP3_V_CVT_PK_U8_F32] = 4;
        m_s_hybridDevicePerfTable[VOP1_V_FREXP_MANT_F32] = 4;
        m_s_hybridDevicePerfTable[VOP1_V_FREXP_EXP_I32_F32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_LDEXP_F32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_EQ_F32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_LE_F32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_GT_F32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_LG_F32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_GE_F32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_O_F32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_U_F32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_NGE_F32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_NLG_F32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_NGT_F32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_NLE_F32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_NEQ_F32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_NLT_F32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_TRU_F32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_NGE_F32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_NLG_F32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_NGT_F32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_NLE_F32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_NEQ_F32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_NLT_F32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_TRU_F32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_F_I32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_LT_I32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_EQ_I32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_LE_I32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_GT_I32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_NE_I32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_GE_I32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_T_I32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_F_I32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_LT_I32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_EQ_I32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_LE_I32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_GT_I32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_NE_I32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_GE_I32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_T_I32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_F_U32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_LT_U32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_EQ_U32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_LE_U32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_GT_U32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_NE_U32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_GE_U32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_T_U32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_F_U32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_LT_U32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_EQ_U32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_LE_U32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_GT_U32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_NE_U32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_GE_U32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_T_U32] = 4;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_CLASS_F32] = 4;
        m_s_hybridDevicePerfTable[VI_V_ADD_F16] = 4;
        m_s_hybridDevicePerfTable[VI_V_MUL_F16] = 4;
        m_s_hybridDevicePerfTable[VI_V_MAD_F16] = 4;
        m_s_hybridDevicePerfTable[VI_V_FMA_F16] = 4;
        m_s_hybridDevicePerfTable[VI_V_DIV_SCALE_F16] = 4;
        m_s_hybridDevicePerfTable[VI_V_DIV_FMAS_F16] = 4;
        m_s_hybridDevicePerfTable[FRA] = 4;
        m_s_hybridDevicePerfTable[VI_V_FRACT_F16] = 4;
        m_s_hybridDevicePerfTable[VI_V_TRUNC_F16] = 4;
        m_s_hybridDevicePerfTable[VI_V_CEIL_F16] = 4;
        m_s_hybridDevicePerfTable[VI_V_RNDNE_F16] = 4;
        m_s_hybridDevicePerfTable[VI_V_FLOOR_F16] = 4;
        m_s_hybridDevicePerfTable[VI_V_FREXP_MANT_F16] = 4;
        m_s_hybridDevicePerfTable[VI_V_FREXP_EXP_I16_F16] = 4;
        m_s_hybridDevicePerfTable[VI_V_LDEXP_F16] = 4;
        m_s_hybridDevicePerfTable[VI_V_MAX_F16] = 4;
        m_s_hybridDevicePerfTable[VI_V_MIN_F16] = 4;
        m_s_hybridDevicePerfTable[VI_V_INTERP_P1LL_F16] = 4;
        m_s_hybridDevicePerfTable[VI_V_INTERP_P1LV_F16] = 4;
        m_s_hybridDevicePerfTable[VI_V_INTERP_P2_F16] = 4;
        m_s_hybridDevicePerfTable[VI_V_ADD_U16] = 4;
        m_s_hybridDevicePerfTable[VI_V_SUB_I16] = 4;
        m_s_hybridDevicePerfTable[VI_V_SUB_U16] = 4;
        m_s_hybridDevicePerfTable[VI_V_MUL_I16] = 4;
        m_s_hybridDevicePerfTable[VI_V_MUL_U16] = 4;
        m_s_hybridDevicePerfTable[VI_V_MAD_I16] = 4;
        m_s_hybridDevicePerfTable[VI_V_MAD_U16] = 4;
        m_s_hybridDevicePerfTable[VI_V_MAX_I16] = 4;
        m_s_hybridDevicePerfTable[VI_V_MIN_I16] = 4;
        m_s_hybridDevicePerfTable[VI_V_MAX_U16] = 4;
        m_s_hybridDevicePerfTable[VI_V_MIN_U16] = 4;
        m_s_hybridDevicePerfTable[VI_V_ASHR_I16] = 4;
        m_s_hybridDevicePerfTable[VI_V_LSHR_B16] = 4;
        m_s_hybridDevicePerfTable[VI_V_LSHL_B16] = 4;
        m_s_hybridDevicePerfTable[VI_V_CVT_I16_F16] = 4;
        m_s_hybridDevicePerfTable[VI_V_CVT_U16_F16] = 4;
        m_s_hybridDevicePerfTable[VI_V_CVT_F16_I16] = 4;
        m_s_hybridDevicePerfTable[VI_V_CVT_F16_U16] = 4;
        m_s_hybridDevicePerfTable[VI_V_CVT_PERM_B32] = 4;
        m_s_hybridDevicePerfTable[VOP1_V_FRACT_F32] = 4;
        m_s_hybridDevicePerfTable[VOP1_V_TRUNC_F32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_MAX_LEGACY_F32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_MIN_LEGACY_F32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_MIN_F32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_MAX_F32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_CEIL_F32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_RNDNE_F32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_FLOOR_F32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_MIN_I32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_MAX_U32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_MIN_U32] = 4;
        m_s_hybridDevicePerfTable[VOP2_V_ASHR_I32] = 4;

        // 1/4 full rate.
        m_s_hybridDevicePerfTable[VOP3_V_DIV_SCALE_F32] = 16;
        m_s_hybridDevicePerfTable[VOP3_V_DIV_FMAS_F32] = 16;
        m_s_hybridDevicePerfTable[VOP3_V_DIV_FIXUP_F32] = 16;

        // Half rate.
        m_s_hybridDevicePerfTable[VOP1_V_LOG_F32] = 8;
        m_s_hybridDevicePerfTable[VOP1_V_LOG_CLAMP_F32] = 8;

        // 1/4 half rate.
        m_s_hybridDevicePerfTable[VOP1_V_CVT_F32_F64] = 32;
        m_s_hybridDevicePerfTable[VOP1_V_CVT_I32_F64] = 32;
        m_s_hybridDevicePerfTable[VOP1_V_CVT_F64_I32] = 32;
        m_s_hybridDevicePerfTable[VOP1_V_CVT_U32_F64] = 32;
        m_s_hybridDevicePerfTable[VOP1_V_CVT_F64_U32] = 32;
        m_s_hybridDevicePerfTable[VOP3_V_MIN_F64] = 32;
        m_s_hybridDevicePerfTable[VOP3_V_MAX_F64] = 32;
        m_s_hybridDevicePerfTable[VOP3_V_LDEXP_F64] = 32;
        m_s_hybridDevicePerfTable[VOP3_V_FREXP_MANT_F64] = 32;
        m_s_hybridDevicePerfTable[VOP3_V_FREXP_EXP_I32_F64] = 32;
        m_s_hybridDevicePerfTable[VOP1_V_FRACT_F64] = 32;
        m_s_hybridDevicePerfTable[VOP1_V_TRUNC_F64] = 32;
        m_s_hybridDevicePerfTable[VOP1_V_CEIL_F64] = 32;
        m_s_hybridDevicePerfTable[VOP1_V_RNDNE_F64] = 32;
        m_s_hybridDevicePerfTable[VOP1_V_FLOOR_F64] = 32;
        m_s_hybridDevicePerfTable[VOP3_V_ASHR_I64] = 32;
        m_s_hybridDevicePerfTable[VOP3_V_DIV_SCALE_F64] = 32;
        m_s_hybridDevicePerfTable[VOP3_V_DIV_FIXUP_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_F_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_LT_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_EQ_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_LE_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_GT_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_LG_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_GE_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_O_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_U_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_NGE_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_NLG_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_NGT_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_NLE_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_NEQ_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_NLT_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_TRU_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_F_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_LT_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_EQ_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_LE_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_GT_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_LG_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_GE_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_O_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_U_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_NGE_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_NLG_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_NGT_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_NLE_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_NEQ_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_NLT_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_TRU_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_F_I64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_LT_I64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_EQ_I64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_LE_I64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_GT_I64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_LG_I64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_GE_I64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_T_I64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_F_I64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_LT_I64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_EQ_I64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_LE_I64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_GT_I64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_LG_I64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_GE_I64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_T_I64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_F_U64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_LT_U64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_EQ_U64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_LE_U64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_GT_U64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_LG_U64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_GE_U64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_T_U64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_F_U64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_LT_U64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_EQ_U64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_LE_U64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_GT_U64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_LG_U64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_GE_U64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_T_U64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMP_CLASS_F64] = 32;
        m_s_hybridDevicePerfTable[VOPC_V_CMPX_CLASS_F64] = 32;

        // Quarter rate.
        m_s_hybridDevicePerfTable[VOP1_V_SQRT_F32] = 16;
        m_s_hybridDevicePerfTable[VOP3_V_MUL_LO_I32] = 16;
        m_s_hybridDevicePerfTable[VOP3_V_MUL_HI_I32] = 16;
        m_s_hybridDevicePerfTable[VOP3_V_MUL_LO_U32] = 16;
        m_s_hybridDevicePerfTable[VOP3_V_MUL_HI_U32] = 16;
        m_s_hybridDevicePerfTable[VOP3_V_MAD_U64_U32] = 16;
        m_s_hybridDevicePerfTable[VOP3_V_MAD_I64_I32] = 16;
        m_s_hybridDevicePerfTable[VOP1_V_EXP_F32] = 16;
        m_s_hybridDevicePerfTable[VOP1_V_LOG_CLAMP_F32] = 16;
        m_s_hybridDevicePerfTable[VOP1_V_RCP_CLAMP_F32] = 16;
        m_s_hybridDevicePerfTable[VOP1_V_RCP_LEGACY_F32] = 16;
        m_s_hybridDevicePerfTable[VOP1_V_RCP_F32] = 16;
        m_s_hybridDevicePerfTable[VOP1_V_RCP_IFLAG_F32] = 16;
        m_s_hybridDevicePerfTable[VOP1_V_RSQ_CLAMP_F32] = 16;
        m_s_hybridDevicePerfTable[VOP1_V_RSQ_LEGACY_F32] = 16;
        m_s_hybridDevicePerfTable[VOP1_V_RSQ_F32] = 16;
        m_s_hybridDevicePerfTable[VOP1_V_SQRT_F32] = 16;
        m_s_hybridDevicePerfTable[VOP1_V_SIN_F32] = 16;
        m_s_hybridDevicePerfTable[VOP1_V_COS_F32] = 16;
        m_s_hybridDevicePerfTable[VOP1_V_RCP_F64] = 16;
        m_s_hybridDevicePerfTable[VOP1_V_RCP_CLAMP_F64] = 16;
        m_s_hybridDevicePerfTable[VOP1_V_RSQ_F64] = 16;
        m_s_hybridDevicePerfTable[VOP1_V_RSQ_CLAMP_F64] = 16;
        m_s_hybridDevicePerfTable[VOP3_V_TRIG_PREOP_F64] = 16;
        m_s_hybridDevicePerfTable[VOP3_V_MUL_F64] = 16;
        m_s_hybridDevicePerfTable[VOP3_V_FMA_F64] = 16;
        m_s_hybridDevicePerfTable[VOP3_V_DIV_FMAS_F64] = 16;
        m_s_hybridDevicePerfTable[VOP3_V_MQSAD_PK_U16_U8] = 16;
        m_s_hybridDevicePerfTable[VOP3_V_MQSAD_U32_U8] = 16;
        m_s_hybridDevicePerfTable[VI_V_RCP_F16] = 16;
        m_s_hybridDevicePerfTable[VI_V_SQRT_F16] = 16;
        m_s_hybridDevicePerfTable[VI_V_RSQ_F16] = 16;
        m_s_hybridDevicePerfTable[VI_V_EXP_F16] = 16;
        m_s_hybridDevicePerfTable[VI_V_LOG_F16] = 16;
        m_s_hybridDevicePerfTable[VI_V_SIN_F16] = 16;
        m_s_hybridDevicePerfTable[VI_V_COS_F16] = 16;
        m_s_hybridDevicePerfTable[VI_QSAD_PK_U16_U8] = 16;

        // 1/4 quarter rate.
        m_s_hybridDevicePerfTable[VOP3_V_MUL_F64] = 64;
        m_s_hybridDevicePerfTable[VOP3_V_FMA_F64] = 64;
        m_s_hybridDevicePerfTable[VOP1_V_SQRT_F64] = 64;
        m_s_hybridDevicePerfTable[VOP1_V_RSQ_CLAMP_F64] = 64;
        m_s_hybridDevicePerfTable[VOP1_V_RSQ_F64] = 64;
    }
}

void Instruction::SetUpHalfDevicesPerfTables()
{
    // Full-rate.
    m_s_halfDevicePerfTable[VOP2_NO_HW_IMPL_V_MAC_F32] = 4;
    m_s_halfDevicePerfTable[VOP2_NO_HW_IMPL_V_MAC_LEGACY_F32] = 4;
    m_s_halfDevicePerfTable[VOP2_NO_HW_IMPL_V_SUBBREV_U32] = 4;
    m_s_halfDevicePerfTable[VOP2_NO_HW_IMPL_V_SUB_F32] = 4;
    m_s_halfDevicePerfTable[VOP2_NO_HW_IMPL_V_SUBREV_F32] = 4;
    m_s_halfDevicePerfTable[VOP2_NO_HW_IMPL_V_SUBREV_I32] = 4;
    m_s_halfDevicePerfTable[VOP2_NO_HW_IMPL_V_MADMK_F32] = 4;
    m_s_halfDevicePerfTable[VOP2_NO_HW_IMPL_V_MADAK_F32] = 4;
    m_s_halfDevicePerfTable[VOP2_NO_HW_IMPL_V_SUBB_U32] = 4;
    m_s_halfDevicePerfTable[VOP2_NO_HW_IMPL_V_LSHRREV_B32] = 4;
    m_s_halfDevicePerfTable[VOP2_NO_HW_IMPL_V_ASHRREV_I32] = 4;
    m_s_halfDevicePerfTable[VOP2_NO_HW_IMPL_V_LSHLREV_B32] = 4;
    m_s_halfDevicePerfTable[VOP1_V_MOV_B32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_MOV_FED_B32] = 4;
    m_s_halfDevicePerfTable[VOP1_V_NOT_B32] = 4;
    m_s_halfDevicePerfTable[VOP1_V_CVT_F32_I32] = 4;
    m_s_halfDevicePerfTable[VOP1_V_CVT_F32_U32] = 4;
    m_s_halfDevicePerfTable[VOP1_V_CVT_U32_F32] = 4;
    m_s_halfDevicePerfTable[VOP1_V_CVT_I32_F32] = 4;
    m_s_halfDevicePerfTable[VOP1_V_CVT_RPI_I32_F32] = 4;
    m_s_halfDevicePerfTable[VOP1_V_CVT_FLR_I32_F32] = 4;
    m_s_halfDevicePerfTable[VOP1_V_CVT_OFF_F32_I4] = 4;
    m_s_halfDevicePerfTable[VOP1_V_BFREV_B32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_MUL_I32_I24] = 4;
    m_s_halfDevicePerfTable[VOP2_V_MUL_HI_I32_I24] = 4;
    m_s_halfDevicePerfTable[VOP2_V_MUL_U32_U24] = 4;
    m_s_halfDevicePerfTable[VOP2_V_MUL_HI_U32_U24] = 4;
    m_s_halfDevicePerfTable[VOP2_V_MUL_F32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_MUL_LEGACY_F32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_ADD_F32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_ADD_I32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_ADDC_U32] = 4;
    m_s_hybridDevicePerfTable[VOP2_V_SUB_I32] = 4;
    m_s_halfDevicePerfTable[VOP2_NO_DOC_V_ADD_U32] = 4;
    m_s_halfDevicePerfTable[VOP2_NO_DOC_V_SUB_U32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_LSHL_B32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_XOR_B32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_OR_B32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_AND_B32] = 4;
    m_s_halfDevicePerfTable[VOP3_V_MAD_LEGACY_F32] = 4;
    m_s_halfDevicePerfTable[VOP3_V_FMA_F32] = 4;
    m_s_halfDevicePerfTable[VOP3_V_MULLIT_F32] = 4;
    m_s_halfDevicePerfTable[VOP3_V_ADD_F64] = 4;
    m_s_halfDevicePerfTable[VOP3_V_MAD_I32_I24] = 4;
    m_s_halfDevicePerfTable[VOP3_V_MAD_U32_U24] = 4;
    m_s_halfDevicePerfTable[VOP3_V_MAD_F32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_BFE_I32] = 4;
    m_s_halfDevicePerfTable[VOP3_V_ALIGNBIT_B32] = 4;
    m_s_halfDevicePerfTable[VOP3_V_ALIGNBYTE_B32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_BFM_B32] = 4;
    m_s_halfDevicePerfTable[VOP3_V_BFI_B32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_BCNT_U32_B32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_MBCNT_LO_U32_B32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_MBCNT_HI_U32_B32] = 4;
    m_s_halfDevicePerfTable[VOP1_V_FFBH_U32] = 4;
    m_s_halfDevicePerfTable[VOP1_V_FFBL_B32] = 4;
    m_s_halfDevicePerfTable[VOP1_V_FFBH_I32] = 4;
    m_s_halfDevicePerfTable[VOP3_V_SAD_U8] = 4;
    m_s_halfDevicePerfTable[VOP3_V_MSAD_U8] = 4;
    m_s_halfDevicePerfTable[VOP3_V_SAD_HI_U8] = 4;
    m_s_halfDevicePerfTable[VOP3_V_SAD_U16] = 4;
    m_s_halfDevicePerfTable[VOP3_V_SAD_U32] = 4;
    m_s_halfDevicePerfTable[VOP3_V_LERP_U8] = 4;
    m_s_halfDevicePerfTable[VOP1_V_CVT_F32_UBYTE0] = 4;
    m_s_halfDevicePerfTable[VOP1_V_CVT_F32_UBYTE1] = 4;
    m_s_halfDevicePerfTable[VOP1_V_CVT_F32_UBYTE2] = 4;
    m_s_halfDevicePerfTable[VOP1_V_CVT_F32_UBYTE3] = 4;
    m_s_halfDevicePerfTable[VOP1_V_CVT_OFF_F32_I4] = 4;
    m_s_halfDevicePerfTable[VOP1_V_CVT_F16_F32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_CVT_PKRTZ_F16_F32] = 4;
    m_s_halfDevicePerfTable[VOP1_V_CVT_F32_F16] = 4;
    m_s_halfDevicePerfTable[VOP2_V_CVT_PKNORM_I16_F32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_CVT_PKNORM_U16_F32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_CVT_PK_U16_U32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_CVT_PK_I16_I32] = 4;
    m_s_halfDevicePerfTable[VOP3_V_FMA_F32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_MUL_U32_U24] = 4;
    m_s_halfDevicePerfTable[VOP2_V_MUL_HI_U32_U24] = 4;
    m_s_halfDevicePerfTable[VOP3_V_MAD_U32_U24] = 4;
    m_s_halfDevicePerfTable[VOP3_V_MUL_I32_I24] = 4;
    m_s_halfDevicePerfTable[VOP2_V_MUL_HI_I32_I24] = 4;
    m_s_halfDevicePerfTable[VOP3_V_MAD_I32_I24] = 4;
    m_s_halfDevicePerfTable[VOP3_V_MULLIT_F32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_CNDMASK_B32] = 4;
    m_s_halfDevicePerfTable[VOP3_V_CUBEID_F32] = 4;
    m_s_halfDevicePerfTable[VOP3_V_CUBESC_F32] = 4;
    m_s_halfDevicePerfTable[VOP3_V_CUBETC_F32] = 4;
    m_s_halfDevicePerfTable[VOP3_V_CUBEMA_F32] = 4;
    m_s_halfDevicePerfTable[VOP3_V_MAX_F32] = 4;
    m_s_halfDevicePerfTable[VOP3_V_MAX_I32] = 4;
    m_s_halfDevicePerfTable[VOP3_V_MAX_U32] = 4;
    m_s_halfDevicePerfTable[VOP3_V_MIN3_F32] = 4;
    m_s_halfDevicePerfTable[VOP3_V_MIN3_I32] = 4;
    m_s_halfDevicePerfTable[VOP3_V_MIN3_U32] = 4;
    m_s_halfDevicePerfTable[VOP3_V_MED3_F32] = 4;
    m_s_halfDevicePerfTable[VOP3_V_MED3_I32] = 4;
    m_s_halfDevicePerfTable[VOP3_V_MED3_U32] = 4;
    m_s_halfDevicePerfTable[VOP3_V_CVT_PK_U8_F32] = 4;
    m_s_halfDevicePerfTable[VOP1_V_FREXP_MANT_F32] = 4;
    m_s_halfDevicePerfTable[VOP1_V_FREXP_EXP_I32_F32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_LDEXP_F32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_EQ_F32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_LE_F32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_GT_F32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_LG_F32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_GE_F32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_O_F32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_U_F32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_NGE_F32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_NLG_F32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_NGT_F32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_NLE_F32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_NEQ_F32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_NLT_F32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_TRU_F32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_NGE_F32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_NLG_F32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_NGT_F32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_NLE_F32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_NEQ_F32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_NLT_F32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_TRU_F32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_F_I32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_LT_I32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_EQ_I32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_LE_I32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_GT_I32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_NE_I32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_GE_I32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_T_I32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_F_I32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_LT_I32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_EQ_I32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_LE_I32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_GT_I32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_NE_I32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_GE_I32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_T_I32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_F_U32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_LT_U32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_EQ_U32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_LE_U32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_GT_U32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_NE_U32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_GE_U32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_T_U32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_F_U32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_LT_U32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_EQ_U32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_LE_U32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_GT_U32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_NE_U32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_GE_U32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_T_U32] = 4;
    m_s_halfDevicePerfTable[VOPC_V_CMP_CLASS_F32] = 4;
    m_s_halfDevicePerfTable[VI_V_ADD_F16] = 4;
    m_s_halfDevicePerfTable[VI_V_MUL_F16] = 4;
    m_s_halfDevicePerfTable[VI_V_MAD_F16] = 4;
    m_s_halfDevicePerfTable[VI_V_FMA_F16] = 4;
    m_s_halfDevicePerfTable[VI_V_DIV_SCALE_F16] = 4;
    m_s_halfDevicePerfTable[VI_V_DIV_FMAS_F16] = 4;
    m_s_halfDevicePerfTable[FRA] = 4;
    m_s_halfDevicePerfTable[VI_V_FRACT_F16] = 4;
    m_s_halfDevicePerfTable[VI_V_TRUNC_F16] = 4;
    m_s_halfDevicePerfTable[VI_V_CEIL_F16] = 4;
    m_s_halfDevicePerfTable[VI_V_RNDNE_F16] = 4;
    m_s_halfDevicePerfTable[VI_V_FLOOR_F16] = 4;
    m_s_halfDevicePerfTable[VI_V_FREXP_MANT_F16] = 4;
    m_s_halfDevicePerfTable[VI_V_FREXP_EXP_I16_F16] = 4;
    m_s_halfDevicePerfTable[VI_V_LDEXP_F16] = 4;
    m_s_halfDevicePerfTable[VI_V_MAX_F16] = 4;
    m_s_halfDevicePerfTable[VI_V_MIN_F16] = 4;
    m_s_halfDevicePerfTable[VI_V_INTERP_P1LL_F16] = 4;
    m_s_halfDevicePerfTable[VI_V_INTERP_P1LV_F16] = 4;
    m_s_halfDevicePerfTable[VI_V_INTERP_P2_F16] = 4;
    m_s_halfDevicePerfTable[VI_V_ADD_U16] = 4;
    m_s_halfDevicePerfTable[VI_V_SUB_I16] = 4;
    m_s_halfDevicePerfTable[VI_V_SUB_U16] = 4;
    m_s_halfDevicePerfTable[VI_V_MUL_I16] = 4;
    m_s_halfDevicePerfTable[VI_V_MUL_U16] = 4;
    m_s_halfDevicePerfTable[VI_V_MAD_I16] = 4;
    m_s_halfDevicePerfTable[VI_V_MAD_U16] = 4;
    m_s_halfDevicePerfTable[VI_V_MAX_I16] = 4;
    m_s_halfDevicePerfTable[VI_V_MIN_I16] = 4;
    m_s_halfDevicePerfTable[VI_V_MAX_U16] = 4;
    m_s_halfDevicePerfTable[VI_V_MIN_U16] = 4;
    m_s_halfDevicePerfTable[VI_V_ASHR_I16] = 4;
    m_s_halfDevicePerfTable[VI_V_LSHR_B16] = 4;
    m_s_halfDevicePerfTable[VI_V_LSHL_B16] = 4;
    m_s_halfDevicePerfTable[VI_V_CVT_I16_F16] = 4;
    m_s_halfDevicePerfTable[VI_V_CVT_U16_F16] = 4;
    m_s_halfDevicePerfTable[VI_V_CVT_F16_I16] = 4;
    m_s_halfDevicePerfTable[VI_V_CVT_F16_U16] = 4;
    m_s_halfDevicePerfTable[VI_V_CVT_PERM_B32] = 4;
    m_s_halfDevicePerfTable[VOP3_V_DIV_SCALE_F32] = 4;
    m_s_halfDevicePerfTable[VOP3_V_DIV_FMAS_F32] = 4;
    m_s_halfDevicePerfTable[VOP3_V_DIV_FIXUP_F32] = 4;
    m_s_halfDevicePerfTable[VOP1_V_FRACT_F32] = 4;
    m_s_halfDevicePerfTable[VOP1_V_TRUNC_F32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_MAX_LEGACY_F32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_MIN_LEGACY_F32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_MIN_F32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_MAX_F32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_CEIL_F32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_RNDNE_F32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_FLOOR_F32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_MIN_I32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_MAX_U32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_MIN_U32] = 4;
    m_s_halfDevicePerfTable[VOP2_V_ASHR_I32] = 4;

    // Half rate.
    m_s_halfDevicePerfTable[VOP1_V_LOG_F32] = 8;
    m_s_halfDevicePerfTable[VOP1_V_LOG_CLAMP_F32] = 8;
    m_s_halfDevicePerfTable[VOP1_V_CVT_F32_F64] = 8;
    m_s_halfDevicePerfTable[VOP1_V_CVT_I32_F64] = 8;
    m_s_halfDevicePerfTable[VOP1_V_CVT_F64_I32] = 8;
    m_s_halfDevicePerfTable[VOP1_V_CVT_U32_F64] = 8;
    m_s_halfDevicePerfTable[VOP1_V_CVT_F64_U32] = 8;
    m_s_halfDevicePerfTable[VOP3_V_MIN_F64] = 8;
    m_s_halfDevicePerfTable[VOP3_V_MAX_F64] = 8;
    m_s_halfDevicePerfTable[VOP3_V_LDEXP_F64] = 8;
    m_s_halfDevicePerfTable[VOP3_V_FREXP_MANT_F64] = 8;
    m_s_halfDevicePerfTable[VOP3_V_FREXP_EXP_I32_F64] = 8;
    m_s_halfDevicePerfTable[VOP1_V_FRACT_F64] = 8;
    m_s_halfDevicePerfTable[VOP1_V_TRUNC_F64] = 8;
    m_s_halfDevicePerfTable[VOP1_V_CEIL_F64] = 8;
    m_s_halfDevicePerfTable[VOP1_V_RNDNE_F64] = 8;
    m_s_halfDevicePerfTable[VOP1_V_FLOOR_F64] = 8;
    m_s_halfDevicePerfTable[VOP3_V_ASHR_I64] = 8;
    m_s_halfDevicePerfTable[VOP3_V_DIV_SCALE_F64] = 8;
    m_s_halfDevicePerfTable[VOP3_V_DIV_FIXUP_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_F_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_LT_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_EQ_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_LE_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_GT_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_LG_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_GE_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_O_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_U_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_NGE_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_NLG_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_NGT_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_NLE_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_NEQ_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_NLT_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_TRU_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_F_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_LT_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_EQ_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_LE_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_GT_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_LG_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_GE_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_O_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_U_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_NGE_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_NLG_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_NGT_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_NLE_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_NEQ_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_NLT_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_TRU_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_F_I64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_LT_I64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_EQ_I64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_LE_I64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_GT_I64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_LG_I64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_GE_I64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_T_I64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_F_I64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_LT_I64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_EQ_I64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_LE_I64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_GT_I64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_LG_I64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_GE_I64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_T_I64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_F_U64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_LT_U64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_EQ_U64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_LE_U64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_GT_U64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_LG_U64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_GE_U64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_T_U64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_F_U64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_LT_U64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_EQ_U64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_LE_U64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_GT_U64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_LG_U64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_GE_U64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_T_U64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMP_CLASS_F64] = 8;
    m_s_halfDevicePerfTable[VOPC_V_CMPX_CLASS_F64] = 8;

    // 32-bit integer mul, muladd, and double precision
    // mul/fmas.
    m_s_halfDevicePerfTable[VOP3_V_MAD_U64_U32] = 8;
    m_s_halfDevicePerfTable[VOP3_V_MUL_LO_I32] = 8;
    m_s_halfDevicePerfTable[VOP3_V_MUL_HI_I32] = 8;
    m_s_halfDevicePerfTable[VOP3_V_MUL_LO_U32] = 8;
    m_s_halfDevicePerfTable[VOP3_V_MUL_HI_U32] = 8;
    m_s_halfDevicePerfTable[VOP3_V_MAD_I64_I32] = 8;
    m_s_halfDevicePerfTable[VOP3_V_MUL_F64] = 8;
    m_s_halfDevicePerfTable[VOP3_V_FMA_F64] = 8;
    m_s_halfDevicePerfTable[VOP3_V_DIV_FMAS_F64] = 8;
    m_s_halfDevicePerfTable[VOP3_V_MUL_F64] = 8;
    m_s_halfDevicePerfTable[VOP3_V_FMA_F64] = 8;

    // Quarter rate.
    m_s_halfDevicePerfTable[VOP1_V_SQRT_F32] = 16;
    m_s_halfDevicePerfTable[VOP1_V_EXP_F32] = 16;
    m_s_halfDevicePerfTable[VOP1_V_LOG_CLAMP_F32] = 16;
    m_s_halfDevicePerfTable[VOP1_V_RCP_CLAMP_F32] = 16;
    m_s_halfDevicePerfTable[VOP1_V_RCP_LEGACY_F32] = 16;
    m_s_halfDevicePerfTable[VOP1_V_RCP_F32] = 16;
    m_s_halfDevicePerfTable[VOP1_V_RCP_IFLAG_F32] = 16;
    m_s_halfDevicePerfTable[VOP1_V_RSQ_CLAMP_F32] = 16;
    m_s_halfDevicePerfTable[VOP1_V_RSQ_LEGACY_F32] = 16;
    m_s_halfDevicePerfTable[VOP1_V_RSQ_F32] = 16;
    m_s_halfDevicePerfTable[VOP1_V_SQRT_F32] = 16;
    m_s_halfDevicePerfTable[VOP1_V_SIN_F32] = 16;
    m_s_halfDevicePerfTable[VOP1_V_COS_F32] = 16;
    m_s_halfDevicePerfTable[VOP1_V_RCP_F64] = 16;
    m_s_halfDevicePerfTable[VOP1_V_RCP_CLAMP_F64] = 16;
    m_s_halfDevicePerfTable[VOP1_V_RSQ_F64] = 16;
    m_s_halfDevicePerfTable[VOP1_V_RSQ_CLAMP_F64] = 16;
    m_s_halfDevicePerfTable[VOP3_V_TRIG_PREOP_F64] = 16;
    m_s_halfDevicePerfTable[VOP3_V_MQSAD_PK_U16_U8] = 16;
    m_s_halfDevicePerfTable[VOP3_V_MQSAD_U32_U8] = 16;
    m_s_halfDevicePerfTable[VI_V_RCP_F16] = 16;
    m_s_halfDevicePerfTable[VI_V_SQRT_F16] = 16;
    m_s_halfDevicePerfTable[VI_V_RSQ_F16] = 16;
    m_s_halfDevicePerfTable[VI_V_EXP_F16] = 16;
    m_s_halfDevicePerfTable[VI_V_LOG_F16] = 16;
    m_s_halfDevicePerfTable[VI_V_SIN_F16] = 16;
    m_s_halfDevicePerfTable[VI_V_COS_F16] = 16;
    m_s_halfDevicePerfTable[VI_QSAD_PK_U16_U8] = 16;
    m_s_halfDevicePerfTable[VOP1_V_SQRT_F64] = 16;
    m_s_halfDevicePerfTable[VOP1_V_RSQ_CLAMP_F64] = 16;
    m_s_halfDevicePerfTable[VOP1_V_RSQ_F64] = 16;
}

void Instruction::SetUpScalarPerfTables()
{
    if (m_s_scalarPerfTable.empty())
    {
        m_s_scalarPerfTable[SOP2_S_ADD_U32] = 4;
        m_s_scalarPerfTable[SOP2_S_SUB_U32] = 4;
        m_s_scalarPerfTable[SOP2_S_ADD_I32] = 4;
        m_s_scalarPerfTable[SOP2_S_SUB_I32] = 4;
        m_s_scalarPerfTable[SOP2_S_ADDC_U32] = 4;
        m_s_scalarPerfTable[SOP2_S_SUBB_U32] = 4;
        m_s_scalarPerfTable[SOP2_S_LSHL_B32] = 4;
        m_s_scalarPerfTable[SOP2_S_LSHL_B64] = 4;
        m_s_scalarPerfTable[SOP2_S_LSHR_B32] = 4;
        m_s_scalarPerfTable[SOP2_S_LSHR_B64] = 4;
        m_s_scalarPerfTable[SOP2_S_MUL_I32] = 4;
        m_s_scalarPerfTable[SOP2_S_AND_B32] = 4;
        m_s_scalarPerfTable[SOP2_S_AND_B64] = 4;
        m_s_scalarPerfTable[SOP2_S_OR_B32] = 4;
        m_s_scalarPerfTable[SOP2_S_OR_B64] = 4;
        m_s_scalarPerfTable[SOP2_S_XOR_B32] = 4;
        m_s_scalarPerfTable[SOP2_S_XOR_B64] = 4;
        m_s_scalarPerfTable[SOP2_S_ANDN2_B32] = 4;
        m_s_scalarPerfTable[SOP2_S_ANDN2_B64] = 4;
        m_s_scalarPerfTable[SOP2_S_ORN2_B32] = 4;
        m_s_scalarPerfTable[SOP2_S_ORN2_B64] = 4;
        m_s_scalarPerfTable[SOP2_S_NAND_B32] = 4;
        m_s_scalarPerfTable[SOP2_S_NAND_B64] = 4;
        m_s_scalarPerfTable[SOP2_S_NOR_B32] = 4;
        m_s_scalarPerfTable[SOP2_S_NOR_B64] = 4;
        m_s_scalarPerfTable[SOP2_S_XNOR_B32] = 4;
        m_s_scalarPerfTable[SOP2_S_XNOR_B64] = 4;
        m_s_scalarPerfTable[SOP1_S_MOV_B32] = 4;
        m_s_scalarPerfTable[SOP1_S_MOV_B64] = 4;
        m_s_scalarPerfTable[SOP1_S_CMOV_B32] = 4;
        m_s_scalarPerfTable[SOP1_S_CMOV_B64] = 4;
        m_s_scalarPerfTable[SOP1_S_NOT_B32] = 4;
        m_s_scalarPerfTable[SOP1_S_NOT_B64] = 4;

        m_s_scalarPerfTable[SOPC_S_CMP_EQ_I32] = 4;
        m_s_scalarPerfTable[SOPC_S_CMP_NE_I32] = 4;
        m_s_scalarPerfTable[SOPC_S_CMP_GT_I32] = 4;
        m_s_scalarPerfTable[SOPC_S_CMP_GE_I32] = 4;
        m_s_scalarPerfTable[SOPC_S_CMP_LE_I32] = 4;
        m_s_scalarPerfTable[SOPC_S_CMP_LT_I32] = 4;
        m_s_scalarPerfTable[SOPC_S_CMP_EQ_U32] = 4;
        m_s_scalarPerfTable[SOPC_S_CMP_NE_U32] = 4;
        m_s_scalarPerfTable[SOPC_S_CMP_GT_U32] = 4;
        m_s_scalarPerfTable[SOPC_S_CMP_GE_U32] = 4;
        m_s_scalarPerfTable[SOPC_S_CMP_LE_U32] = 4;
        m_s_scalarPerfTable[SOPC_S_CMP_LT_U32] = 4;
        m_s_scalarPerfTable[SOPK_S_CMPK_EQ_I32] = 4;
        m_s_scalarPerfTable[SOPK_S_CMPK_NE_I32] = 4;
        m_s_scalarPerfTable[SOPK_S_CMPK_GT_I32] = 4;
        m_s_scalarPerfTable[SOPK_S_CMPK_GE_I32] = 4;
        m_s_scalarPerfTable[SOPK_S_CMPK_LE_I32] = 4;
        m_s_scalarPerfTable[SOPK_S_CMPK_LT_I32] = 4;
        m_s_scalarPerfTable[SOPK_S_CMPK_EQ_U32] = 4;
        m_s_scalarPerfTable[SOPK_S_CMPK_NE_U32] = 4;
        m_s_scalarPerfTable[SOPK_S_CMPK_GT_U32] = 4;
        m_s_scalarPerfTable[SOPK_S_CMPK_GE_U32] = 4;
        m_s_scalarPerfTable[SOPK_S_CMPK_LE_U32] = 4;
        m_s_scalarPerfTable[SOPK_S_CMPK_LT_U32] = 4;
        m_s_scalarPerfTable[SOPC_S_BITCMP0_B32] = 4;
        m_s_scalarPerfTable[SOPC_S_BITCMP0_B64] = 4;
        m_s_scalarPerfTable[SOPC_S_BITCMP1_B32] = 4;
        m_s_scalarPerfTable[SOPC_S_BITCMP1_B64] = 4;
        m_s_scalarPerfTable[SOP2_S_MOVK_I32] = 4;
        m_s_scalarPerfTable[SOP2_S_ASHR_I32] = 4;
        m_s_scalarPerfTable[SOP2_S_ASHR_I64] = 4;
        m_s_scalarPerfTable[SOP2_S_BFM_B32] = 4;
        m_s_scalarPerfTable[SOP2_S_BFM_B64] = 4;
        m_s_scalarPerfTable[SOP2_S_BFE_I32] = 4;
        m_s_scalarPerfTable[SOP2_S_BFE_I64] = 4;
        m_s_scalarPerfTable[SOP2_S_BFE_U32] = 4;
        m_s_scalarPerfTable[SOP2_S_BFE_U64] = 4;
        m_s_scalarPerfTable[SOP1_S_WQM_B32] = 4;
        m_s_scalarPerfTable[SOP1_S_WQM_B64] = 4;
        m_s_scalarPerfTable[SOP1_S_QUADMASK_B32] = 4;
        m_s_scalarPerfTable[SOP1_S_QUADMASK_B64] = 4;
        m_s_scalarPerfTable[SOP1_S_BREV_B32] = 4;
        m_s_scalarPerfTable[SOP1_S_BREV_B64] = 4;
        m_s_scalarPerfTable[SOP1_S_BCNT0_I32_B32] = 4;
        m_s_scalarPerfTable[SOP1_S_BCNT0_I32_B64] = 4;
        m_s_scalarPerfTable[SOP1_S_BCNT1_I32_B32] = 4;
        m_s_scalarPerfTable[SOP1_S_BCNT1_I32_B64] = 4;
        m_s_scalarPerfTable[SOP1_S_FF0_I32_B32] = 4;
        m_s_scalarPerfTable[SOP1_S_FF0_I32_B64] = 4;
        m_s_scalarPerfTable[SOP1_S_FF1_I32_B32] = 4;
        m_s_scalarPerfTable[SOP1_S_FF1_I32_B64] = 4;
        m_s_scalarPerfTable[SOP1_S_FLBIT_I32_B32] = 4;
        m_s_scalarPerfTable[SOP1_S_FLBIT_I32_B64] = 4;
        m_s_scalarPerfTable[SOP1_S_FLBIT_I32] = 4;
        m_s_scalarPerfTable[SOP1_S_FLBIT_I32_I64] = 4;
        m_s_scalarPerfTable[SOP1_S_BITSET0_B32] = 4;
        m_s_scalarPerfTable[SOP1_S_BITSET0_B64] = 4;
        m_s_scalarPerfTable[SOP1_S_BITSET1_B32] = 4;
        m_s_scalarPerfTable[SOP1_S_BITSET1_B64] = 4;
        m_s_scalarPerfTable[SOP1_S_AND_SAVEEXEC_B64] = 4;
        m_s_scalarPerfTable[SOP1_S_OR_SAVEEXEC_B64] = 4;
        m_s_scalarPerfTable[SOP1_S_XOR_SAVEEXEC_B64] = 4;
        m_s_scalarPerfTable[SOP1_S_ANDN2_SAVEEXEC_B64] = 4;
        m_s_scalarPerfTable[SOP1_S_ORN2_SAVEEXEC_B64] = 4;
        m_s_scalarPerfTable[SOP1_S_NAND_SAVEEXEC_B64] = 4;
        m_s_scalarPerfTable[SOP1_S_NOR_SAVEEXEC_B64] = 4;
        m_s_scalarPerfTable[SOP1_S_XNOR_SAVEEXEC_B64] = 4;
        m_s_scalarPerfTable[SOP1_S_MOVRELS_B32] = 4;
        m_s_scalarPerfTable[SOP1_S_MOVRELS_B64] = 4;
        m_s_scalarPerfTable[SOP1_S_MOVRELD_B32] = 4;
        m_s_scalarPerfTable[SOP1_S_MOVRELD_B64] = 4;
        m_s_scalarPerfTable[SOPK_S_GETREG_B32] = 4;
        m_s_scalarPerfTable[SOPK_S_SETREG_B32] = 4;
        m_s_scalarPerfTable[SOPK_S_SETREG_IMM32_B32] = 4;
        m_s_scalarPerfTable[SOP2_S_CSELECT_B32] = 4;
        m_s_scalarPerfTable[SOP2_S_CSELECT_B64] = 4;
        m_s_scalarPerfTable[SOPK_S_CMOVK_I32] = 4;
        m_s_scalarPerfTable[SOP1_S_CMOV_B32] = 4;
        m_s_scalarPerfTable[SOP1_S_CMOV_B64] = 4;
        m_s_scalarPerfTable[SOP2_S_ABSDIFF_I32] = 4;
        m_s_scalarPerfTable[SOP2_S_MIN_I32] = 4;
        m_s_scalarPerfTable[SOP2_S_MIN_U32] = 4;
        m_s_scalarPerfTable[SOP2_S_MAX_I32] = 4;
        m_s_scalarPerfTable[SOP2_S_MAX_U32] = 4;
        m_s_scalarPerfTable[SOP1_S_ABS_I32] = 4;
        m_s_scalarPerfTable[SOP1_S_SEXT_I32_I8] = 4;
        m_s_scalarPerfTable[SOP1_S_SEXT_I32_I16] = 4;

        m_s_scalarPerfTable[SOPP_S_NOP] = 1;
        m_s_scalarPerfTable[SOPP_S_ENDPGM] = 1;
        m_s_scalarPerfTable[SOPP_S_TRAP] = 1;
        m_s_scalarPerfTable[SOPP_S_RFE] = 1;
        m_s_scalarPerfTable[SOPP_S_SETPRIO] = 1;
        m_s_scalarPerfTable[SOPP_S_SLEEP] = 1;
        m_s_scalarPerfTable[SOPP_S_SENDMSG] = 1;
        m_s_scalarPerfTable[SOPP_S_BARRIER] = 1;
        m_s_scalarPerfTable[SOPP_S_SETHALT] = 1;
        m_s_scalarPerfTable[SOPP_S_SENDMSGHALT] = 1;
        m_s_scalarPerfTable[SOPP_S_ICACHE_INV] = 1;
        m_s_scalarPerfTable[SOPP_S_INCPERFLEVEL] = 1;
        m_s_scalarPerfTable[SOPP_S_DECPERFLEVEL] = 1;
        m_s_scalarPerfTable[SOPP_S_TTRACEDATA] = 1;
        m_s_scalarPerfTable[SOPP_S_SETPC] = 1;
        m_s_scalarPerfTable[SOPP_S_SWAPPC] = 1;
    }
}

void Instruction::SetUpQuarterDevicesPerfTables()
{
    // Full-rate.
    m_s_quarterDevicePerfTable[VOP2_NO_HW_IMPL_V_MAC_F32] = 4;
    m_s_quarterDevicePerfTable[VOP2_NO_HW_IMPL_V_MAC_LEGACY_F32] = 4;
    m_s_quarterDevicePerfTable[VOP2_NO_HW_IMPL_V_SUBBREV_U32] = 4;
    m_s_quarterDevicePerfTable[VOP2_NO_HW_IMPL_V_SUB_F32] = 4;
    m_s_quarterDevicePerfTable[VOP2_NO_HW_IMPL_V_SUBREV_F32] = 4;
    m_s_quarterDevicePerfTable[VOP2_NO_HW_IMPL_V_SUBREV_I32] = 4;
    m_s_quarterDevicePerfTable[VOP2_NO_HW_IMPL_V_MADMK_F32] = 4;
    m_s_quarterDevicePerfTable[VOP2_NO_HW_IMPL_V_MADAK_F32] = 4;
    m_s_quarterDevicePerfTable[VOP2_NO_HW_IMPL_V_SUBB_U32] = 4;
    m_s_quarterDevicePerfTable[VOP2_NO_HW_IMPL_V_LSHRREV_B32] = 4;
    m_s_quarterDevicePerfTable[VOP2_NO_HW_IMPL_V_ASHRREV_I32] = 4;
    m_s_quarterDevicePerfTable[VOP2_NO_HW_IMPL_V_LSHLREV_B32] = 4;
    m_s_quarterDevicePerfTable[VOP1_V_MOV_B32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_MOV_FED_B32] = 4;
    m_s_quarterDevicePerfTable[VOP1_V_NOT_B32] = 4;
    m_s_quarterDevicePerfTable[VOP1_V_CVT_F32_I32] = 4;
    m_s_quarterDevicePerfTable[VOP1_V_CVT_F32_U32] = 4;
    m_s_quarterDevicePerfTable[VOP1_V_CVT_U32_F32] = 4;
    m_s_quarterDevicePerfTable[VOP1_V_CVT_I32_F32] = 4;
    m_s_quarterDevicePerfTable[VOP1_V_CVT_RPI_I32_F32] = 4;
    m_s_quarterDevicePerfTable[VOP1_V_CVT_FLR_I32_F32] = 4;
    m_s_quarterDevicePerfTable[VOP1_V_CVT_OFF_F32_I4] = 4;
    m_s_quarterDevicePerfTable[VOP1_V_BFREV_B32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_MUL_I32_I24] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_MUL_HI_I32_I24] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_MUL_U32_U24] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_MUL_HI_U32_U24] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_MUL_F32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_MUL_LEGACY_F32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_ADD_F32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_ADD_I32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_ADDC_U32] = 4;
    m_s_hybridDevicePerfTable[VOP2_V_SUB_I32] = 4;
    m_s_quarterDevicePerfTable[VOP2_NO_DOC_V_ADD_U32] = 4;
    m_s_quarterDevicePerfTable[VOP2_NO_DOC_V_SUB_U32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_LSHL_B32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_XOR_B32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_OR_B32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_AND_B32] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_MAD_LEGACY_F32] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_FMA_F32] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_MULLIT_F32] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_ADD_F64] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_MAD_I32_I24] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_MAD_U32_U24] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_MAD_F32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_BFE_I32] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_ALIGNBIT_B32] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_ALIGNBYTE_B32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_BFM_B32] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_BFI_B32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_BCNT_U32_B32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_MBCNT_LO_U32_B32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_MBCNT_HI_U32_B32] = 4;
    m_s_quarterDevicePerfTable[VOP1_V_FFBH_U32] = 4;
    m_s_quarterDevicePerfTable[VOP1_V_FFBL_B32] = 4;
    m_s_quarterDevicePerfTable[VOP1_V_FFBH_I32] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_SAD_U8] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_MSAD_U8] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_SAD_HI_U8] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_SAD_U16] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_SAD_U32] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_LERP_U8] = 4;
    m_s_quarterDevicePerfTable[VOP1_V_CVT_F32_UBYTE0] = 4;
    m_s_quarterDevicePerfTable[VOP1_V_CVT_F32_UBYTE1] = 4;
    m_s_quarterDevicePerfTable[VOP1_V_CVT_F32_UBYTE2] = 4;
    m_s_quarterDevicePerfTable[VOP1_V_CVT_F32_UBYTE3] = 4;
    m_s_quarterDevicePerfTable[VOP1_V_CVT_OFF_F32_I4] = 4;
    m_s_quarterDevicePerfTable[VOP1_V_CVT_F16_F32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_CVT_PKRTZ_F16_F32] = 4;
    m_s_quarterDevicePerfTable[VOP1_V_CVT_F32_F16] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_CVT_PKNORM_I16_F32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_CVT_PKNORM_U16_F32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_CVT_PK_U16_U32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_CVT_PK_I16_I32] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_FMA_F32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_MUL_U32_U24] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_MUL_HI_U32_U24] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_MAD_U32_U24] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_MUL_I32_I24] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_MUL_HI_I32_I24] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_MAD_I32_I24] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_MULLIT_F32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_CNDMASK_B32] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_CUBEID_F32] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_CUBESC_F32] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_CUBETC_F32] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_CUBEMA_F32] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_MAX_F32] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_MAX_I32] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_MAX_U32] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_MIN3_F32] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_MIN3_I32] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_MIN3_U32] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_MED3_F32] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_MED3_I32] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_MED3_U32] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_CVT_PK_U8_F32] = 4;
    m_s_quarterDevicePerfTable[VOP1_V_FREXP_MANT_F32] = 4;
    m_s_quarterDevicePerfTable[VOP1_V_FREXP_EXP_I32_F32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_LDEXP_F32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_EQ_F32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_LE_F32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_GT_F32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_LG_F32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_GE_F32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_O_F32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_U_F32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_NGE_F32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_NLG_F32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_NGT_F32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_NLE_F32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_NEQ_F32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_NLT_F32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_TRU_F32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_NGE_F32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_NLG_F32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_NGT_F32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_NLE_F32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_NEQ_F32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_NLT_F32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_TRU_F32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_F_I32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_LT_I32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_EQ_I32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_LE_I32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_GT_I32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_NE_I32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_GE_I32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_T_I32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_F_I32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_LT_I32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_EQ_I32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_LE_I32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_GT_I32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_NE_I32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_GE_I32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_T_I32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_F_U32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_LT_U32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_EQ_U32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_LE_U32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_GT_U32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_NE_U32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_GE_U32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_T_U32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_F_U32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_LT_U32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_EQ_U32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_GT_U32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_NE_U32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_GE_U32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_T_U32] = 4;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_CLASS_F32] = 4;
    m_s_quarterDevicePerfTable[VI_V_ADD_F16] = 4;
    m_s_quarterDevicePerfTable[VI_V_MUL_F16] = 4;
    m_s_quarterDevicePerfTable[VI_V_MAD_F16] = 4;
    m_s_quarterDevicePerfTable[VI_V_FMA_F16] = 4;
    m_s_quarterDevicePerfTable[VI_V_DIV_SCALE_F16] = 4;
    m_s_quarterDevicePerfTable[VI_V_DIV_FMAS_F16] = 4;
    m_s_quarterDevicePerfTable[FRA] = 4;
    m_s_quarterDevicePerfTable[VI_V_FRACT_F16] = 4;
    m_s_quarterDevicePerfTable[VI_V_TRUNC_F16] = 4;
    m_s_quarterDevicePerfTable[VI_V_CEIL_F16] = 4;
    m_s_quarterDevicePerfTable[VI_V_RNDNE_F16] = 4;
    m_s_quarterDevicePerfTable[VI_V_FLOOR_F16] = 4;
    m_s_quarterDevicePerfTable[VI_V_FREXP_MANT_F16] = 4;
    m_s_quarterDevicePerfTable[VI_V_FREXP_EXP_I16_F16] = 4;
    m_s_quarterDevicePerfTable[VI_V_LDEXP_F16] = 4;
    m_s_quarterDevicePerfTable[VI_V_MAX_F16] = 4;
    m_s_quarterDevicePerfTable[VI_V_MIN_F16] = 4;
    m_s_quarterDevicePerfTable[VI_V_INTERP_P1LL_F16] = 4;
    m_s_quarterDevicePerfTable[VI_V_INTERP_P1LV_F16] = 4;
    m_s_quarterDevicePerfTable[VI_V_INTERP_P2_F16] = 4;
    m_s_quarterDevicePerfTable[VI_V_ADD_U16] = 4;
    m_s_quarterDevicePerfTable[VI_V_SUB_I16] = 4;
    m_s_quarterDevicePerfTable[VI_V_SUB_U16] = 4;
    m_s_quarterDevicePerfTable[VI_V_MUL_I16] = 4;
    m_s_quarterDevicePerfTable[VI_V_MUL_U16] = 4;
    m_s_quarterDevicePerfTable[VI_V_MAD_I16] = 4;
    m_s_quarterDevicePerfTable[VI_V_MAD_U16] = 4;
    m_s_quarterDevicePerfTable[VI_V_MAX_I16] = 4;
    m_s_quarterDevicePerfTable[VI_V_MIN_I16] = 4;
    m_s_quarterDevicePerfTable[VI_V_MAX_U16] = 4;
    m_s_quarterDevicePerfTable[VI_V_MIN_U16] = 4;
    m_s_quarterDevicePerfTable[VI_V_ASHR_I16] = 4;
    m_s_quarterDevicePerfTable[VI_V_LSHR_B16] = 4;
    m_s_quarterDevicePerfTable[VI_V_LSHL_B16] = 4;
    m_s_quarterDevicePerfTable[VI_V_CVT_I16_F16] = 4;
    m_s_quarterDevicePerfTable[VI_V_CVT_U16_F16] = 4;
    m_s_quarterDevicePerfTable[VI_V_CVT_F16_I16] = 4;
    m_s_quarterDevicePerfTable[VI_V_CVT_F16_U16] = 4;
    m_s_quarterDevicePerfTable[VI_V_CVT_PERM_B32] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_DIV_SCALE_F32] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_DIV_FMAS_F32] = 4;
    m_s_quarterDevicePerfTable[VOP3_V_DIV_FIXUP_F32] = 4;
    m_s_quarterDevicePerfTable[VOP1_V_FRACT_F32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_MAX_LEGACY_F32] = 4;
    m_s_quarterDevicePerfTable[VOP1_V_TRUNC_F32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_MIN_LEGACY_F32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_MIN_F32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_MAX_F32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_CEIL_F32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_RNDNE_F32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_FLOOR_F32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_MIN_I32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_MAX_U32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_MIN_U32] = 4;
    m_s_quarterDevicePerfTable[VOP2_V_ASHR_I32] = 4;

    // Half rate.
    m_s_quarterDevicePerfTable[VOP1_V_LOG_F32] = 8;
    m_s_quarterDevicePerfTable[VOP1_V_LOG_CLAMP_F32] = 8;
    m_s_quarterDevicePerfTable[VOP1_V_CVT_F32_F64] = 8;
    m_s_quarterDevicePerfTable[VOP1_V_CVT_I32_F64] = 8;
    m_s_quarterDevicePerfTable[VOP1_V_CVT_F64_I32] = 8;
    m_s_quarterDevicePerfTable[VOP1_V_CVT_U32_F64] = 8;
    m_s_quarterDevicePerfTable[VOP1_V_CVT_F64_U32] = 8;
    m_s_quarterDevicePerfTable[VOP3_V_MIN_F64] = 8;
    m_s_quarterDevicePerfTable[VOP3_V_MAX_F64] = 8;
    m_s_quarterDevicePerfTable[VOP3_V_LDEXP_F64] = 8;
    m_s_quarterDevicePerfTable[VOP3_V_FREXP_MANT_F64] = 8;
    m_s_quarterDevicePerfTable[VOP3_V_FREXP_EXP_I32_F64] = 8;
    m_s_quarterDevicePerfTable[VOP1_V_FRACT_F64] = 8;
    m_s_quarterDevicePerfTable[VOP1_V_TRUNC_F64] = 8;
    m_s_quarterDevicePerfTable[VOP1_V_CEIL_F64] = 8;
    m_s_quarterDevicePerfTable[VOP1_V_RNDNE_F64] = 8;
    m_s_quarterDevicePerfTable[VOP1_V_FLOOR_F64] = 8;
    m_s_quarterDevicePerfTable[VOP3_V_ASHR_I64] = 8;
    m_s_quarterDevicePerfTable[VOP3_V_DIV_SCALE_F64] = 8;
    m_s_quarterDevicePerfTable[VOP3_V_DIV_FIXUP_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_F_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_LT_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_EQ_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_LE_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_GT_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_LG_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_GE_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_O_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_U_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_NGE_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_NLG_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_NGT_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_NLE_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_NEQ_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_NLT_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_TRU_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_F_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_LT_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_EQ_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_LE_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_GT_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_LG_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_GE_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_O_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_U_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_NGE_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_NLG_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_NGT_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_NLE_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_NEQ_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_NLT_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_TRU_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_F_I64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_LT_I64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_EQ_I64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_LE_I64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_GT_I64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_LG_I64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_GE_I64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_T_I64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_F_I64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_LT_I64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_EQ_I64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_LE_I64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_GT_I64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_LG_I64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_GE_I64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_T_I64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_F_U64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_LT_U64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_EQ_U64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_LE_U64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_GT_U64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_LG_U64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_GE_U64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_T_U64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_F_U64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_LT_U64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_EQ_U64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_LE_U64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_GT_U64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_LG_U64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_GE_U64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_T_U64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMP_CLASS_F64] = 8;
    m_s_quarterDevicePerfTable[VOPC_V_CMPX_CLASS_F64] = 8;

    // Quarter rate.
    m_s_quarterDevicePerfTable[VOP1_V_SQRT_F32] = 16;
    m_s_quarterDevicePerfTable[VOP3_V_MUL_LO_I32] = 16;
    m_s_quarterDevicePerfTable[VOP3_V_MUL_HI_I32] = 16;
    m_s_quarterDevicePerfTable[VOP3_V_MUL_LO_U32] = 16;
    m_s_quarterDevicePerfTable[VOP3_V_MUL_HI_U32] = 16;
    m_s_quarterDevicePerfTable[VOP3_V_MAD_U64_U32] = 16;
    m_s_quarterDevicePerfTable[VOP3_V_MAD_I64_I32] = 16;
    m_s_quarterDevicePerfTable[VOP1_V_EXP_F32] = 16;
    m_s_quarterDevicePerfTable[VOP1_V_LOG_CLAMP_F32] = 16;
    m_s_quarterDevicePerfTable[VOP1_V_RCP_CLAMP_F32] = 16;
    m_s_quarterDevicePerfTable[VOP1_V_RCP_LEGACY_F32] = 16;
    m_s_quarterDevicePerfTable[VOP1_V_RCP_F32] = 16;
    m_s_quarterDevicePerfTable[VOP1_V_RCP_IFLAG_F32] = 16;
    m_s_quarterDevicePerfTable[VOP1_V_RSQ_CLAMP_F32] = 16;
    m_s_quarterDevicePerfTable[VOP1_V_RSQ_LEGACY_F32] = 16;
    m_s_quarterDevicePerfTable[VOP1_V_RSQ_F32] = 16;
    m_s_quarterDevicePerfTable[VOP1_V_SQRT_F32] = 16;
    m_s_quarterDevicePerfTable[VOP1_V_SIN_F32] = 16;
    m_s_quarterDevicePerfTable[VOP1_V_COS_F32] = 16;
    m_s_quarterDevicePerfTable[VOP1_V_RCP_F64] = 16;
    m_s_quarterDevicePerfTable[VOP1_V_RCP_CLAMP_F64] = 16;
    m_s_quarterDevicePerfTable[VOP1_V_RSQ_F64] = 16;
    m_s_quarterDevicePerfTable[VOP1_V_RSQ_CLAMP_F64] = 16;
    m_s_quarterDevicePerfTable[VOP3_V_TRIG_PREOP_F64] = 16;
    m_s_quarterDevicePerfTable[VOP3_V_MUL_F64] = 16;
    m_s_quarterDevicePerfTable[VOP3_V_FMA_F64] = 16;
    m_s_quarterDevicePerfTable[VOP3_V_DIV_FMAS_F64] = 16;
    m_s_quarterDevicePerfTable[VOP3_V_MQSAD_PK_U16_U8] = 16;
    m_s_quarterDevicePerfTable[VOP3_V_MQSAD_U32_U8] = 16;
    m_s_quarterDevicePerfTable[VI_V_RCP_F16] = 16;
    m_s_quarterDevicePerfTable[VI_V_SQRT_F16] = 16;
    m_s_quarterDevicePerfTable[VI_V_RSQ_F16] = 16;
    m_s_quarterDevicePerfTable[VI_V_EXP_F16] = 16;
    m_s_quarterDevicePerfTable[VI_V_LOG_F16] = 16;
    m_s_quarterDevicePerfTable[VI_V_SIN_F16] = 16;
    m_s_quarterDevicePerfTable[VI_V_COS_F16] = 16;
    m_s_quarterDevicePerfTable[VI_QSAD_PK_U16_U8] = 16;
    m_s_quarterDevicePerfTable[VOP3_V_MUL_F64] = 16;
    m_s_quarterDevicePerfTable[VOP3_V_FMA_F64] = 16;
    m_s_quarterDevicePerfTable[VOP1_V_SQRT_F64] = 16;
    m_s_quarterDevicePerfTable[VOP1_V_RSQ_CLAMP_F64] = 16;
    m_s_quarterDevicePerfTable[VOP1_V_RSQ_F64] = 16;
}

std::string Instruction::GetFunctionalUnitAsString(InstructionCategory category)
{
    std::string ret;

    switch (category)
    {
        case ScalarALU:
            ret = FUNC_UNIT_SALU;
            break;

        case ScalarMemoryRead:
        case ScalarMemoryWrite:
            ret = FUNC_UNIT_SMEM;
            break;

        case VectorMemoryRead:
        case VectorMemoryWrite:
            ret = FUNC_UNIT_VMEM;
            break;

        case VectorALU:
            ret = FUNC_UNIT_VALU;
            break;

        case LDS:
            ret = FUNC_UNIT_LDS;
            break;

        case GDS:
        case Export:
            ret = FUNC_UNIT_GDS_EXPORT;
            break;

        case Atomics:
            ret = FUNC_UNIT_ATOMICS;
            break;

        case Internal:
            // We will still name it "Flow Control"
            // so that it will be meaningful for the end user.
            ret = FUNC_UNIT_INTERNAL_FLOW;
            break;

        case Branch:
            ret = FUNC_UNIT_BRANCH;
            break;

        default:
            ret = FUNC_UNIT_UNKNOWN;

            // We shouldn't get here.
            break;
    }

    return ret;
}

Instruction::Instruction(const std::string& labelString)
{
    // Setup the performance tables.
    SetUpPerfTables();

    m_pointingLabelString = labelString;

    size_t strLen = m_pointingLabelString.size();

    // Remove terminating carriage return character from the label if it exists
    if (!m_pointingLabelString.empty() && m_pointingLabelString[strLen - 1] == '\r')
    {
        m_pointingLabelString.erase(strLen - 1);
    }
}


#endif  // INSTRUCTION_STRING_REPRESENTATION

Instruction::Instruction(unsigned int instructionWidth, InstructionCategory instructionFormatKind, InstructionSet instructionFormat) :
    m_instructionWidth(instructionWidth), m_instructionCategory(instructionFormatKind), m_instructionFormat(instructionFormat), m_HwGen(GDT_HW_GENERATION_SOUTHERNISLAND)
{

#ifdef INSTRUCTION_STRING_REPRESENTATION
    // Setup the performance tables.
    SetUpPerfTables();
#endif  // INSTRUCTION_STRING_REPRESENTATION

}

