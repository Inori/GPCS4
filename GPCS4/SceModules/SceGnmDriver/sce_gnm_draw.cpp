#include "sce_gnmdriver.h"
#include "Graphic/Gnm/GnmOpCode.h"
#include <cassert>

// well, it seems that libSceGnmDriverForNeoMode.sprx does nothing but simply fills structs
// in command buffer during these draw calls, actual work is done in submit calls.
// we do the same.

uint32_t PS4API sceGnmDrawInitDefaultHardwareState350(uint32_t* cmdBuffer, uint64_t numDwords)
{
	LOG_SCE_GRAPHIC("cmdbuff %p numDwords %d", cmdBuffer, numDwords);
	const uint initCmdSize = sizeof(GnmDrawInitDefaultHardwareState) / sizeof(uint32_t);
	assert(numDwords >= initCmdSize);
	GnmDrawInitDefaultHardwareState* initParam = (GnmDrawInitDefaultHardwareState*)cmdBuffer;
	initParam->opcode = OPCODE_BUILD(initCmdSize, OP_TYPE_PRIV_DRAW, OP_INFO_DRAW_INITIALIZE_DEFAULT_HARDWARE_STATE);
	memset(initParam->reserved, 0, sizeof(initParam->reserved) * sizeof(uint32_t));
	return initCmdSize;
}


// called in waitUntilSafeForRendering
int PS4API sceGnmInsertWaitFlipDone(uint32_t* cmdBuffer, uint32_t numDwords, int videoOutHandle, uint32_t displayBufferIndex)
{
	LOG_SCE_GRAPHIC("cmdbuff %p numdws %d handle %d dpindex %d", cmdBuffer, numDwords, videoOutHandle, displayBufferIndex);
	const uint cmdSize = sizeof(GnmWaitFlipDone) / sizeof(uint32_t);
	assert(cmdSize == numDwords);
	GnmWaitFlipDone* param = (GnmWaitFlipDone*)cmdBuffer;
	param->opcode = OPCODE_BUILD(cmdSize, OP_TYPE_PRIV_DRAW, OP_INFO_WAIT_UNTIL_SAFE_FOR_RENDERING);
	param->videoOutHandle = videoOutHandle;
	param->displayBufferIndex = displayBufferIndex;
	memset(param->reserved, 0, sizeof(param->reserved) * sizeof(uint32_t));

	// this is a hack of Nier:Automata, just let the game run without trap into infinity loop
	//static uint nCount = 0;
	//static uint nMod = 0;
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
	uint32_t indexCount, const void *indexAddr, uint32_t pred, uint32_t inlineMode)
{
	LOG_SCE_GRAPHIC("cmd %p numdw %d idxcount %d", cmdBuffer, numDwords, indexCount);
	const uint32_t paramSize = sizeof(GnmDrawIndex) / sizeof(uint32_t);
	assert(paramSize == numDwords);
	GnmDrawIndex* param = (GnmDrawIndex*)cmdBuffer;
	param->opcode = OPCODE_BUILD(paramSize, OP_TYPE_PRIV_DRAW, OP_INFO_DRAW_INDEX);
	param->indexCount = indexCount;
	param->indexAddr = (ulong_ptr)indexAddr;
	param->pred = pred;
	param->inlineMode = inlineMode;
	memset(param->reserved, 0, sizeof(param->reserved) * sizeof(uint32_t));
	return SCE_OK;
}


int PS4API sceGnmDrawIndexAuto(uint32_t* cmdBuffer, uint32_t numDwords,
	uint32_t indexCount, uint32_t pred)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDrawIndexIndirect(uint32_t* cmdBuffer, uint32_t numDwords,
	uint32_t dataOffsetInBytes,
	ShaderStage stage,
	uint8_t vertexOffsetUserSgpr,
	uint8_t instanceOffsetUserSgpr,
	uint32_t pred)
{
	LOG_SCE_GRAPHIC("Not implemented");
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


int PS4API sceGnmSetEmbeddedVsShader(uint32_t* cmdBuffer, uint32_t numDwords)
{
	LOG_SCE_GRAPHIC("Not implemented");
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


int PS4API sceGnmSetVsShader(uint32_t* cmdBuffer, uint32_t numDwords,
	const VsStageRegisters *vsRegs, uint32_t shaderModifier)
{
	LOG_SCE_GRAPHIC("cmd %p numdw %d vs %p mod %d", cmdBuffer, numDwords, vsRegs, shaderModifier);
	const uint32_t paramSize = sizeof(GnmVSShader) / sizeof(uint32_t);
	assert(paramSize == numDwords);
	GnmVSShader* param = (GnmVSShader*)cmdBuffer;
	param->opcode = OPCODE_BUILD(paramSize, OP_TYPE_PRIV_DRAW, OP_INFO_SET_VS_SHADER);
	param->modifier = shaderModifier;
	memcpy(&param->vsRegs, vsRegs, sizeof(VsStageRegisters));
	memset(param->reserved, 0, sizeof(param->reserved) * sizeof(uint32_t));
	return SCE_OK;
}


int PS4API sceGnmSetPsShader350(uint32_t* cmdBuffer, uint32_t numDwords, 
	const PsStageRegisters *psRegs)
{
	LOG_SCE_GRAPHIC("cmd %p numdw %d ps %p", cmdBuffer, numDwords, psRegs);
	const uint32_t paramSize = sizeof(GnmPSShader) / sizeof(uint32_t);
	assert(paramSize == numDwords);
	GnmPSShader* param = (GnmPSShader*)cmdBuffer;
	param->opcode = OPCODE_BUILD(paramSize, OP_TYPE_PRIV_DRAW, OP_INFO_SET_PS_SHADER);
	memcpy(&param->psRegs, psRegs, sizeof(PsStageRegisters));
	memset(param->reserved, 0, sizeof(param->reserved) * sizeof(uint32_t));
	return SCE_OK;
}



int PS4API sceGnmSetVgtControl(uint32_t* cmdBuffer, uint32_t numDwords,
	uint8_t primGroupSizeMinusOne, 
	VgtPartialVsWaveMode partialVsWaveMode, 
	WdSwitchOnlyOnEopMode wdSwitchOnlyOnEopMode)
{
	LOG_SCE_GRAPHIC("cmd %p numdw %d", cmdBuffer, numDwords);
	const uint32_t paramSize = sizeof(GnmVgtControl) / sizeof(uint32_t);
	assert(paramSize == numDwords);
	GnmVgtControl* param = (GnmVgtControl*)cmdBuffer;
	param->opcode = OPCODE_BUILD(paramSize, OP_TYPE_PRIV_DRAW, OP_INFO_SET_VGT_CONTROL);
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


int PS4API sceGnmUpdatePsShader350(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmUpdateVsShader(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


