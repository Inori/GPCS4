#include "sce_gnmdriver.h"


// TODO:
// from reverse engining of libSceGnmDriverForNeoMode.sprx,
// it seems that the 'pStruct' points to a complex structure.
// and 'size' is some what a size or offset.
// and this function returns a size value, depend on an ioctrl syscall
// the returned value will affect the address passed to sceGnmInsertWaitFlipDone
//
/*
LOAD:0000000000004700                 cmp     byte ptr [rax], 0  ; [rax] depend on a ioctrl
LOAD:0000000000004703                 mov     eax, 0C0017900h
LOAD:0000000000004708                 cmovz   edx, eax
LOAD:000000000000470B                 mov     eax, 40000258h
LOAD:0000000000004710                 cmovz   esi, eax
LOAD:0000000000004713                 mov     eax, 0D00FFh
LOAD:0000000000004718                 mov     [rcx+20Ch], edx
LOAD:000000000000471E                 mov     edx, 100h
LOAD:0000000000004723                 cmovz   eax, r8d
LOAD:0000000000004727                 mov     [rcx+210h], esi
LOAD:000000000000472D                 mov     [rcx+214h], eax
LOAD:0000000000004733                 lea     rax, [rcx+218h]
LOAD:000000000000473A                 sub     rax, rdi
LOAD:000000000000473D                 shr     rax, 2
LOAD:0000000000004741                 sub     rdx, rax
LOAD:0000000000004744                 lea     rax, [rcx+rdx*4+218h]
LOAD:000000000000474C                 shl     edx, 10h
LOAD:000000000000474F                 add     edx, 3FFE0000h
LOAD:0000000000004755                 sub     rax, rdi
LOAD:0000000000004758                 or      edx, 0C0001000h
LOAD:000000000000475E                 shr     rax, 2
*/
uint64_t PS4API sceGnmDrawInitDefaultHardwareState350(void* pStruct, uint64_t size)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return 0;
}


// TODO:
// this is just a hack on Nier:Automata to let the game goes on without trapping
// into the infinity loop checking a label.
// and I can focus on more important things
//
// from reverse engining of libSceGnmDriverForNeoMode.sprx,
// this function will fill a struct pointed to by  'gpuAddress'
// the value of 'gpuAddress' is affected by the return value of sceGnmDrawInitDefaultHardwareState350
//
// the purpose of this function is to insert a wait token in the command buffer,
// and when the flip is done on gpu/kernel,
// the gpu/kernel will set the value specified by 'value' param on gpuAddress.
// I just set this value immediately.

/*
LOAD:000000000000493D      mov     dword ptr [rbx], 0C0053C00h ; rbx = gpuAddress
LOAD:0000000000004943      mov     dword ptr [rbx+4], 13h
LOAD:000000000000494A      lea     rax, [rax+rcx*8]
LOAD:000000000000494E      mov     ecx, eax
LOAD:0000000000004950      shr     rax, 20h
LOAD:0000000000004954      and     ecx, 0FFFFFFFCh
LOAD:0000000000004957      movzx   eax, ax
LOAD:000000000000495A      mov     [rbx+8], ecx
LOAD:000000000000495D      mov     [rbx+0Ch], eax
LOAD:0000000000004960      xor     eax, eax
LOAD:0000000000004962      mov     dword ptr [rbx+10h], 0
LOAD:0000000000004969      mov     dword ptr [rbx+14h], 0FFFFFFFFh
LOAD:0000000000004970      mov     dword ptr [rbx+18h], 0Ah
*/
int PS4API sceGnmInsertWaitFlipDone(void* gpuAddress, int type_or_mask, int uk, int value)
{
	LOG_SCE_GRAPHIC("gpuaddr %p type %d uk %d val %d", gpuAddress, type_or_mask, uk, value);


	static uint nCount = 0;
	static uint nMod = 0;
	*(uint32_t*)((uint8_t*)gpuAddress + 0x2000034) = value + nCount;
	*(uint32_t*)((uint8_t*)gpuAddress + 0x200008C) = value + nCount;
	++nMod;
	if (nMod % 3 == 0)
	{
		nCount += 3;
	}

	return SCE_OK;
}


int PS4API sceGnmDrawOpaqueAuto(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDrawIndex(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDrawIndexAuto(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDrawIndexIndirect(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDrawIndexIndirectMulti(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDrawIndexMultiInstanced(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDrawIndexOffset(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDrawIndirect(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDrawIndirectCountMulti(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDrawIndirectMulti(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}



int PS4API sceGnmSetEmbeddedPsShader(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSetEmbeddedVsShader(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSetEsShader(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}



int PS4API sceGnmSetGsShader(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSetHsShader(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSetLsShader(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSetPsShader350(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}



int PS4API sceGnmSetVsShader(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSetVgtControl(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}



int PS4API sceGnmResetVgtControl(void)
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


