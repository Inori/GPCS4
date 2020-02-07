#include "sce_gnmdriver.h"
#include "Graphic/Gnm/GnmOpCode.h"
#include <cassert>

LOG_CHANNEL(SceModules.SceGnmDriver.GnmDraw);

// well, it seems that libSceGnmDriverForNeoMode.sprx does nothing but simply fills structs
// in command buffer during these draw calls, actual work is done in submit calls.
// we do the same.

uint32_t PS4API sceGnmDrawInitDefaultHardwareState350(uint32_t* cmdBuffer, uint64_t numDwords)
{
	LOG_SCE_GRAPHIC("cmdbuff %p numDwords %d", cmdBuffer, numDwords);
	const uint32_t initCmdSize = sizeof(GnmCmdDrawInitDefaultHardwareState) / sizeof(uint32_t);
	assert(numDwords >= initCmdSize);
	GnmCmdDrawInitDefaultHardwareState* initParam = (GnmCmdDrawInitDefaultHardwareState*)cmdBuffer;
	initParam->opcode = PM4_HEADER_BUILD(initCmdSize, IT_GNM_PRIVATE, OP_PRIV_INITIALIZE_DEFAULT_HARDWARE_STATE);
	memset(initParam->reserved, 0, sizeof(initParam->reserved) * sizeof(uint32_t));
	return initCmdSize;
}


uint32_t PS4API sceGnmDrawInitDefaultHardwareState200(uint32_t* cmdBuffer, uint64_t numDwords)
{
	LOG_SCE_GRAPHIC("cmdbuff %p numDwords %d", cmdBuffer, numDwords);
	const uint32_t initCmdSize = sizeof(GnmCmdDrawInitDefaultHardwareState) / sizeof(uint32_t);
	assert(numDwords >= initCmdSize);
	GnmCmdDrawInitDefaultHardwareState* initParam = (GnmCmdDrawInitDefaultHardwareState*)cmdBuffer;
	initParam->opcode                             = PM4_HEADER_BUILD(initCmdSize, IT_GNM_PRIVATE, OP_PRIV_INITIALIZE_DEFAULT_HARDWARE_STATE);
	memset(initParam->reserved, 0, sizeof(initParam->reserved) * sizeof(uint32_t));
	return initCmdSize;
}


uint32_t PS4API sceGnmDrawInitDefaultHardwareState175(uint32_t* cmdBuffer, uint64_t numDwords)
{
	LOG_SCE_GRAPHIC("cmdbuff %p numDwords %d", cmdBuffer, numDwords);
	const uint32_t initCmdSize = sizeof(GnmCmdDrawInitDefaultHardwareState) / sizeof(uint32_t);
	assert(numDwords >= initCmdSize);
	GnmCmdDrawInitDefaultHardwareState* initParam = (GnmCmdDrawInitDefaultHardwareState*)cmdBuffer;
	initParam->opcode = PM4_HEADER_BUILD(initCmdSize, IT_GNM_PRIVATE, OP_PRIV_INITIALIZE_DEFAULT_HARDWARE_STATE);
	memset(initParam->reserved, 0, sizeof(initParam->reserved) * sizeof(uint32_t));
	return initCmdSize;
}


// called in waitUntilSafeForRendering
int PS4API sceGnmInsertWaitFlipDone(uint32_t* cmdBuffer, uint32_t numDwords, int videoOutHandle, uint32_t displayBufferIndex)
{
	LOG_SCE_GRAPHIC("cmdbuff %p numdws %d handle %d dpindex %d", cmdBuffer, numDwords, videoOutHandle, displayBufferIndex);
	const uint32_t cmdSize = sizeof(GnmCmdWaitFlipDone) / sizeof(uint32_t);
	assert(cmdSize == numDwords);
	GnmCmdWaitFlipDone* param = (GnmCmdWaitFlipDone*)cmdBuffer;
	param->opcode = PM4_HEADER_BUILD(cmdSize, IT_GNM_PRIVATE, OP_PRIV_WAIT_UNTIL_SAFE_FOR_RENDERING);
	param->videoOutHandle = videoOutHandle;
	param->displayBufferIndex = displayBufferIndex;
	memset(param->reserved, 0, sizeof(param->reserved) * sizeof(uint32_t));

	// this is a hack of Nier:Automata, just let the game run without trap into infinity loop
	//static uint32_t nCount = 0;
	//static uint32_t nMod = 0;
	//*(uint32_t*)((uint8_t*)gpuAddress + 0x2000034) = value + nCount;
	//*(uint32_t*)((uint8_t*)gpuAddress + 0x200008C) = value + nCount;
	//++nMod;
	//if (nMod % 3 == 0)
	//{
	//	nCount += 3;
	//}
	return SCE_OK;
}



int PS4API sceGnmDrawIndex(uint32_t* cmdBuffer, uint32_t numDwords,
	uint32_t indexCount, const void *indexAddr, uint32_t predAndMod, uint32_t inlineMode)
{
	LOG_SCE_GRAPHIC("cmd %p numdw %d idxcount %d", cmdBuffer, numDwords, indexCount);
	const uint32_t paramSize = sizeof(GnmCmdDrawIndex) / sizeof(uint32_t);
	assert(paramSize == numDwords);
	GnmCmdDrawIndex* param = (GnmCmdDrawIndex*)cmdBuffer;
	param->opcode = PM4_HEADER_BUILD(paramSize, IT_GNM_PRIVATE, OP_PRIV_DRAW_INDEX);
	param->indexCount = indexCount;
	param->indexAddr = (uintptr_t)indexAddr;
	param->predAndMod = predAndMod;
	param->inlineMode = (GnmEnumDrawIndexInlineMode)inlineMode;
	memset(param->reserved, 0, sizeof(param->reserved) * sizeof(uint32_t));
	return SCE_OK;
}


int PS4API sceGnmDrawIndexAuto(uint32_t* cmdBuffer, uint32_t numDwords,
	uint32_t indexCount, uint32_t pred)
{
	LOG_SCE_GRAPHIC("cmd %p numdw %d idxcount %d", cmdBuffer, numDwords, indexCount);
	const uint32_t paramSize = sizeof(GnmCmdDrawIndexAuto) / sizeof(uint32_t);
	assert(paramSize == numDwords);
	GnmCmdDrawIndexAuto* param = (GnmCmdDrawIndexAuto*)cmdBuffer;
	param->opcode = PM4_HEADER_BUILD(paramSize, IT_GNM_PRIVATE, OP_PRIV_DRAW_INDEX_AUTO);
	param->indexCount = indexCount;
	param->predAndMod = pred;
	memset(param->reserved, 0, sizeof(param->reserved) * sizeof(uint32_t));
	return SCE_OK;
}


int PS4API sceGnmDrawIndexIndirect(uint32_t* cmdBuffer, uint32_t numDwords,
	uint32_t dataOffsetInBytes,
	ShaderStage stage,
	uint8_t vertexOffsetUserSgpr,
	uint8_t instanceOffsetUserSgpr,
	uint32_t pred)
{
	LOG_SCE_GRAPHIC("cmd %p numdw %d", cmdBuffer, numDwords);
	//LOG_SCE_GRAPHIC("Not implemented");
	GnmCmdDrawIndexIndirect *param = (GnmCmdDrawIndexIndirect *)cmdBuffer;
	const uint32_t paramSize = sizeof(GnmCmdDrawIndexIndirect) / sizeof(uint32_t);
	assert(paramSize == numDwords);
	
	param->opcode =
		PM4_HEADER_BUILD(paramSize, IT_GNM_PRIVATE, OP_PRIV_DRAW_INDEX_INDIRECT);

	param->dataOffsetInBytes      = dataOffsetInBytes;
	param->stage                  = stage;
	param->vertexOffsetUserSgpr   = vertexOffsetUserSgpr;
	param->instanceOffsetUserSgpr = instanceOffsetUserSgpr;
	param->pred                   = pred;
	memset(param->reserved, 0, sizeof(param->reserved) * sizeof(uint32_t));
	
	return SCE_OK;
}


int PS4API sceGnmDrawIndexIndirectMulti(uint32_t* cmdBuffer, uint32_t numDwords)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDrawIndexMultiInstanced(uint32_t* cmdBuffer, uint32_t numDwords)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDrawIndexOffset(uint32_t* cmdBuffer, uint32_t numDwords)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDrawIndirect(uint32_t* cmdBuffer, uint32_t numDwords)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDrawIndirectCountMulti(uint32_t* cmdBuffer, uint32_t numDwords)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDrawIndirectMulti(uint32_t* cmdBuffer, uint32_t numDwords)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDrawOpaqueAuto(uint32_t* cmdBuffer, uint32_t numDwords)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSetEmbeddedPsShader(uint32_t* cmdBuffer, uint32_t numDwords)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSetEmbeddedVsShader(uint32_t* cmdBuffer, uint32_t numDwords, 
	EmbeddedVsShader shaderId, uint32_t shaderModifier)
{
	LOG_SCE_GRAPHIC("cmd %p numdw %d id %d mod %d", cmdBuffer, numDwords, shaderId, shaderModifier);

	const uint32_t paramSize = sizeof(GnmCmdVSShader) / sizeof(uint32_t);
	assert(paramSize == numDwords);
	GnmCmdVSShader* param = (GnmCmdVSShader*)cmdBuffer;
	param->opcode = PM4_HEADER_BUILD(paramSize, IT_GNM_PRIVATE, OP_PRIV_SET_EMBEDDED_VS_SHADER);
	param->shaderId = shaderId;
	param->modifier = shaderModifier;
	memset(param->reserved, 0, sizeof(param->reserved) * sizeof(uint32_t));
	return SCE_OK;
}


int PS4API sceGnmSetEsShader(uint32_t* cmdBuffer, uint32_t numDwords)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}



int PS4API sceGnmSetGsShader(uint32_t* cmdBuffer, uint32_t numDwords)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSetHsShader(uint32_t* cmdBuffer, uint32_t numDwords)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSetLsShader(uint32_t* cmdBuffer, uint32_t numDwords)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSetCsShader(uint32_t* cmdBuffer, uint32_t numDwords, 
	const pssl::CsStageRegisters *csRegs, uint32_t shaderModifier)
{
	LOG_SCE_GRAPHIC("cmd %p numdw %d vs %p mod %d", cmdBuffer, numDwords, csRegs, shaderModifier);

	const uint32_t paramSize = sizeof(GnmCmdCSShader) / sizeof(uint32_t);
	assert(paramSize == numDwords);
	GnmCmdCSShader* param = (GnmCmdCSShader*)cmdBuffer;
	param->opcode         = PM4_HEADER_BUILD(paramSize, IT_GNM_PRIVATE, OP_PRIV_SET_CS_SHADER);
	param->modifier       = shaderModifier;
	if (csRegs != NULL)
	{
		memcpy(&param->csRegs, csRegs, sizeof(pssl::CsStageRegisters));
	}
	else
	{
		memset(&param->csRegs, 0, sizeof(pssl::CsStageRegisters));
	}
	memset(param->reserved, 0, sizeof(param->reserved) * sizeof(uint32_t));
	return SCE_OK;
}


int PS4API sceGnmSetVsShader(uint32_t* cmdBuffer, uint32_t numDwords,
	const pssl::VsStageRegisters *vsRegs, uint32_t shaderModifier)
{
	LOG_SCE_GRAPHIC("cmd %p numdw %d vs %p mod %d", cmdBuffer, numDwords, vsRegs, shaderModifier);

	const uint32_t paramSize = sizeof(GnmCmdVSShader) / sizeof(uint32_t);
	assert(paramSize == numDwords);
	GnmCmdVSShader* param = (GnmCmdVSShader*)cmdBuffer;
	param->opcode = PM4_HEADER_BUILD(paramSize, IT_GNM_PRIVATE, OP_PRIV_SET_VS_SHADER);
	param->modifier = shaderModifier;
	if (vsRegs != NULL)
	{
		memcpy(&param->vsRegs, vsRegs, sizeof(pssl::VsStageRegisters));
	}
	else
	{
		memset(&param->vsRegs, 0, sizeof(pssl::VsStageRegisters));
	}
	memset(param->reserved, 0, sizeof(param->reserved) * sizeof(uint32_t));
	return SCE_OK;
}


int PS4API sceGnmSetPsShader350(uint32_t* cmdBuffer, uint32_t numDwords, 
	const pssl::PsStageRegisters *psRegs)
{
	LOG_SCE_GRAPHIC("cmd %p numdw %d ps %p", cmdBuffer, numDwords, psRegs);

	const uint32_t paramSize = sizeof(GnmCmdPSShader) / sizeof(uint32_t);
	assert(paramSize == numDwords);
	GnmCmdPSShader* param = (GnmCmdPSShader*)cmdBuffer;
	param->opcode = PM4_HEADER_BUILD(paramSize, IT_GNM_PRIVATE, OP_PRIV_SET_PS_SHADER);
	if (psRegs != NULL)
	{
		memcpy(&param->psRegs, psRegs, sizeof(pssl::PsStageRegisters));
	}
	else
	{
		memset(&param->psRegs, 0, sizeof(pssl::PsStageRegisters));
	}
	memset(param->reserved, 0, sizeof(param->reserved) * sizeof(uint32_t));
	return SCE_OK;
}


int PS4API sceGnmSetPsShader(uint32_t* cmdBuffer, uint32_t numDwords, 
	const pssl::PsStageRegisters *psRegs)
{
	LOG_SCE_GRAPHIC("cmd %p numdw %d ps %p", cmdBuffer, numDwords, psRegs);

	const uint32_t paramSize = sizeof(GnmCmdPSShader) / sizeof(uint32_t);
	assert(paramSize == numDwords);
	GnmCmdPSShader* param = (GnmCmdPSShader*)cmdBuffer;
	param->opcode         = PM4_HEADER_BUILD(paramSize, IT_GNM_PRIVATE, OP_PRIV_SET_PS_SHADER);
	if (psRegs != NULL)
	{
		memcpy(&param->psRegs, psRegs, sizeof(pssl::PsStageRegisters));
	}
	else
	{
		memset(&param->psRegs, 0, sizeof(pssl::PsStageRegisters));
	}
	memset(param->reserved, 0, sizeof(param->reserved) * sizeof(uint32_t));
	return SCE_OK;
}


int PS4API sceGnmSetVgtControl(uint32_t* cmdBuffer, uint32_t numDwords,
	uint8_t primGroupSizeMinusOne, 
	VgtPartialVsWaveMode partialVsWaveMode, 
	WdSwitchOnlyOnEopMode wdSwitchOnlyOnEopMode)
{
	LOG_SCE_GRAPHIC("cmd %p numdw %d", cmdBuffer, numDwords);
	const uint32_t paramSize = sizeof(GnmCmdVgtControl) / sizeof(uint32_t);
	assert(paramSize == numDwords);
	GnmCmdVgtControl* param = (GnmCmdVgtControl*)cmdBuffer;
	param->opcode = PM4_HEADER_BUILD(paramSize, IT_GNM_PRIVATE, OP_PRIV_SET_VGT_CONTROL);
	param->primGroupSizeMinusOne = primGroupSizeMinusOne;
	param->partialVsWaveMode = partialVsWaveMode;
	param->wdSwitchOnlyOnEopMode = wdSwitchOnlyOnEopMode;
	return SCE_OK;
}



int PS4API sceGnmResetVgtControl(uint32_t* cmdBuffer, uint32_t numDwords)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmLogicalTcaUnitToPhysical(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmUpdateVsShader(uint32_t* cmdBuffer, uint32_t numDwords, 
	const pssl::VsStageRegisters *vsRegs, uint32_t shaderModifier)
{
	LOG_SCE_GRAPHIC("cmd %p numdw %d vs %p mod %d", cmdBuffer, numDwords, vsRegs, shaderModifier);

	const uint32_t paramSize = sizeof(GnmCmdVSShader) / sizeof(uint32_t);
	assert(paramSize == numDwords);
	GnmCmdVSShader* param = (GnmCmdVSShader*)cmdBuffer;
	param->opcode = PM4_HEADER_BUILD(paramSize, IT_GNM_PRIVATE, OP_PRIV_UPDATE_VS_SHADER);
	param->modifier = shaderModifier;
	if (vsRegs != NULL)
	{
		memcpy(&param->vsRegs, vsRegs, sizeof(pssl::VsStageRegisters));
	}
	else
	{
		memset(&param->vsRegs, 0, sizeof(pssl::VsStageRegisters));
	}
	memset(param->reserved, 0, sizeof(param->reserved) * sizeof(uint32_t));
	return SCE_OK;
}


int PS4API sceGnmUpdatePsShader350(uint32_t* cmdBuffer, uint32_t numDwords, 
	const pssl::PsStageRegisters *psRegs)
{
	LOG_SCE_GRAPHIC("cmd %p numdw %d ps %p", cmdBuffer, numDwords, psRegs);

	const uint32_t paramSize = sizeof(GnmCmdPSShader) / sizeof(uint32_t);
	assert(paramSize == numDwords);
	GnmCmdPSShader* param = (GnmCmdPSShader*)cmdBuffer;
	param->opcode = PM4_HEADER_BUILD(paramSize, IT_GNM_PRIVATE, OP_PRIV_SET_PS_SHADER);
	if (psRegs != NULL)
	{
		memcpy(&param->psRegs, psRegs, sizeof(pssl::PsStageRegisters));
	}
	else
	{
		memset(&param->psRegs, 0, sizeof(pssl::PsStageRegisters));
	}
	memset(param->reserved, 0, sizeof(param->reserved) * sizeof(uint32_t));
	return SCE_OK;
}


int PS4API sceGnmUpdatePsShader(uint32_t* cmdBuffer, uint32_t numDwords, 
	const pssl::PsStageRegisters *psRegs)
{
	LOG_SCE_GRAPHIC("cmd %p numdw %d ps %p", cmdBuffer, numDwords, psRegs);

	const uint32_t paramSize = sizeof(GnmCmdPSShader) / sizeof(uint32_t);
	assert(paramSize == numDwords);
	GnmCmdPSShader* param = (GnmCmdPSShader*)cmdBuffer;
	param->opcode         = PM4_HEADER_BUILD(paramSize, IT_GNM_PRIVATE, OP_PRIV_SET_PS_SHADER);
	if (psRegs != NULL)
	{
		memcpy(&param->psRegs, psRegs, sizeof(pssl::PsStageRegisters));
	}
	else
	{
		memset(&param->psRegs, 0, sizeof(pssl::PsStageRegisters));
	}
	memset(param->reserved, 0, sizeof(param->reserved) * sizeof(uint32_t));
	return SCE_OK;
}


int PS4API sceGnmUpdateGsShader(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmUpdateHsShader(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


