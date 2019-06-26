#include "TLSHandler.h"
#include "Platform/UtilMemory.h"
// x86_64 max instruction length
#define X64_INSTRUCTION_LEN_MAX 15



ZydisDecoder CTLSHandler::s_oDecoder;

ZydisFormatter CTLSHandler::s_oFormatter;


CTLSHandler::CTLSHandler()
{
}

CTLSHandler::~CTLSHandler()
{
}



void CTLSHandler::InitZydis()
{
	ZydisDecoderInit(&CTLSHandler::s_oDecoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_ADDRESS_WIDTH_64);
	ZydisFormatterInit(&CTLSHandler::s_oFormatter, ZYDIS_FORMATTER_STYLE_INTEL);
}

void CTLSHandler::PrintInst(ZydisDecodedInstruction& inst)
{
	const int nBufLen = 256;
	char szBuffer[nBufLen];
	ZydisFormatterFormatInstruction(&s_oFormatter, &inst, szBuffer, sizeof(szBuffer));
	LOG_DEBUG("exception inst: %s", szBuffer);
}

bool CTLSHandler::IsTlsAccess(void* pCode)
{
	bool bRet = false;
	do 
	{
		if (!pCode)
		{
			break;
		}

		ZydisDecodedInstruction instruction;
		ZydisStatus status = ZydisDecoderDecodeBuffer(&s_oDecoder, pCode, 
			X64_INSTRUCTION_LEN_MAX, (ZydisU64)pCode, &instruction);
		if (!ZYDIS_SUCCESS(status))
		{
			LOG_ERR("decode instruction failed at %p", pCode);
			break;
		}

		// mov
		if (instruction.mnemonic != ZYDIS_MNEMONIC_MOV)
		{
			break;
		}

		if (instruction.operandCount != 2)
		{
			break;
		}

		// TODO:
		// I found that all TLS access instructions is the same:
		// mov rax, fs:[0]
		// so currently we only support rax as destination,
		// this should be easy to extend to other registers
		if (instruction.operands[0].reg.value != ZYDIS_REGISTER_RAX || 
			instruction.operands[1].mem.segment != ZYDIS_REGISTER_FS)
		{
			break;
		}

#ifdef GPCS4_DEBUG
		PrintInst(instruction);
#endif

		bRet = true;
	} while (false);
	return bRet;
}


uint CTLSHandler::GetPatchLen(byte* pCode, uint nOldLen)
{
	uint nSumLen = 0, nInstLen = 0;
	ZydisDecodedInstruction instruction;
	while (nSumLen < nOldLen)
	{
		ZydisStatus status = ZydisDecoderDecodeBuffer(&s_oDecoder, pCode,
			X64_INSTRUCTION_LEN_MAX, (ZydisU64)pCode, &instruction);
		if (!ZYDIS_SUCCESS(status))
		{
			LOG_ERR("decode instruction failed at %p", pCode);
			break;
		}

#ifdef GPCS4_DEBUG
		PrintInst(instruction);
#endif
		// TODO:
		// if patched instruction contains position relative ones
		// we currently not support
		if (instruction.attributes & ZYDIS_ATTRIB_IS_RELATIVE)
		{
			LOG_ERR("patched instruction contains relative inst.");
			nSumLen = 0;
			break;
		}

		pCode += instruction.length;
		nSumLen += instruction.length;
	}
	return nSumLen;
}

uint CTLSHandler::GetMovFsLenLen(void* pCode)
{
	ZydisDecodedInstruction instruction;
	ZydisDecoderDecodeBuffer(&s_oDecoder, pCode,
			X64_INSTRUCTION_LEN_MAX, (ZydisU64)pCode, &instruction);
	return instruction.length;
}

extern "C" void GetTlsDataStub(void);

bool CTLSHandler::PatchTLSInstruction(void* pCode)
{
	bool bRet = false;

#pragma pack(1)
	struct JMP_CODE_HEAD
	{
		// push xxxx
		unsigned char byPush = 0x68;
		unsigned int nPushVal;
		// mov dword ptr [rsp + 4], xxxx
		unsigned char byMovEsp[4] = { 0xC7, 0x44, 0x24, 0x04 };
		unsigned int nMovVal;
		// jmp [nJmpVal]
		unsigned char byJmp[6] = { 0xFF, 0x25, 0x00, 0x00, 0x00, 0x00 };
		unsigned long long nJmpVal;
	};

	struct JMP_CODE_FOOT
	{
		// jmp [nJmpVal]
		unsigned char byJmp[6] = { 0xFF, 0x25, 0x00, 0x00, 0x00, 0x00 };
		unsigned long long nJmpVal;
	};
#pragma pack()

	do 
	{
		if (!pCode)
		{
			break;
		}

		uint nPatchedLen = GetPatchLen((byte*)pCode, sizeof(JMP_CODE_HEAD));
		if (!nPatchedLen)
		{
			break;
		}

		// first, build foot jmp code
		JMP_CODE_FOOT jmpFoot;
		jmpFoot.nJmpVal = (uint64)((byte*)pCode + nPatchedLen);
		uint nMovFsLen = GetMovFsLenLen(pCode);
		// delete mov rax, fs:[0]
		// go straight to next instruction
		byte* pMovNext = (byte*)pCode + nMovFsLen;
		uint nMovLeftLen = nPatchedLen - nMovFsLen;
		uint nFootCodeLen = nMovLeftLen + sizeof(jmpFoot);
		byte* pFootCode = (byte*)UtilMemory::VMMap(nFootCodeLen, UtilMemory::VMPF_READ_WRITE_EXECUTE);
		if (!pFootCode)
		{
			break;
		}

		memcpy(pFootCode, pMovNext, nMovLeftLen);
		memcpy(pFootCode + nMovLeftLen, &jmpFoot, sizeof(jmpFoot));

		// second, build head jmp code
		JMP_CODE_HEAD jmpHead;
		jmpHead.nPushVal = (uint)((uint64)pFootCode & 0xFFFFFFFF);
		jmpHead.nMovVal = (uint)((uint64)pFootCode >> 32);
		jmpHead.nJmpVal = (uint64)GetTlsDataStub;

		// make game code writable
		if (!UtilMemory::VMProtect(pCode, sizeof(jmpHead), UtilMemory::VMPF_READ_WRITE_EXECUTE))
		{
			break;
		}

		memcpy(pCode, &jmpHead, sizeof(jmpHead));

		bRet = true;
	} while (false);
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
#ifdef GPCS4_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN

CTLSHandlerWin::CTLSHandlerWin():
	m_pVEHHandle(NULL)
{
}

CTLSHandlerWin::~CTLSHandlerWin()
{
}


bool CTLSHandlerWin::Install()
{
	bool bRet = false;
	do 
	{
		InitZydis();

		m_pVEHHandle = AddVectoredExceptionHandler(TRUE, 
			(PVECTORED_EXCEPTION_HANDLER)CTLSHandlerWin::StaticExceptionHandler);
		if (!m_pVEHHandle)
		{
			break;
		}

		bRet = true;
	} while (false);
	return bRet;
}

void CTLSHandlerWin::Uninstall()
{
	if (m_pVEHHandle)
	{
		RemoveVectoredExceptionHandler(m_pVEHHandle);
		m_pVEHHandle = NULL;
	}
}

long CTLSHandlerWin::StaticExceptionHandler(void* pExceptionArg)
{
	PEXCEPTION_POINTERS pExceptionInfo = (PEXCEPTION_POINTERS)pExceptionArg;
	long nRet = EXCEPTION_CONTINUE_SEARCH;
	do 
	{
		LOG_DEBUG("exception code %x addr %p",
			pExceptionInfo->ExceptionRecord->ExceptionCode, pExceptionInfo->ExceptionRecord->ExceptionAddress);

		void* pExcptAddr = pExceptionInfo->ExceptionRecord->ExceptionAddress;
		if (!IsTlsAccess(pExcptAddr))
		{
			LOG_ERR("unknown exception raised at %p", pExcptAddr);
			break;
		}	

		if (!PatchTLSInstruction(pExcptAddr))
		{
			LOG_ERR("patch tls instruction failed.");
			break;
		}

		nRet = EXCEPTION_CONTINUE_EXECUTION;
	} while (false);
	return nRet;
}

#else


CTLSHandlerLinux::CTLSHandlerLinux()
{
}

CTLSHandlerLinux::~CTLSHandlerLinux()
{
}


#endif  //GPCS4_WINDOWS

