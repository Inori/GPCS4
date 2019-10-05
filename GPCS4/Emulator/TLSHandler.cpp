#include "TLSHandler.h"
#include "Platform/UtilMemory.h"

// x86_64 max instruction length
#define X64_INSTRUCTION_LEN_MAX 15


ZydisDecoder CTLSHandler::s_oDecoder;
ZydisFormatter CTLSHandler::s_oFormatter;
std::vector<byte> CTLSHandler::s_vtTlsImageBackup;
thread_local CTLSHandler::TCB* CTLSHandler::t_pTcbRecord = NULL;


CTLSHandler::CTLSHandler()
{
}

CTLSHandler::~CTLSHandler()
{
}

void CTLSHandler::NotifyThreadCreate(uint nTid)
{

}

void CTLSHandler::NotifyThreadExit(uint nTid)
{
	if (t_pTcbRecord)
	{
		FreeTLS(t_pTcbRecord);
		t_pTcbRecord = NULL;
	}
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
	LOG_DEBUG("instruction: %s", szBuffer);
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
	uint nSumLen = 0;
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

uint CTLSHandler::GetMovFsLen(void* pCode)
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
		uint nMovFsLen = GetMovFsLen(pCode);
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

bool CTLSHandler::BuildTLSBackup(void* pTls, uint nInitSize, uint nTotalSize)
{
	bool bRet = false;
	do
	{
		if (!pTls || !nInitSize || !nTotalSize)
		{
			bRet = true;  // No TLS, mainly some sdk demos.
			break;
		}

		s_vtTlsImageBackup.resize(nTotalSize);
		memcpy(s_vtTlsImageBackup.data(), pTls, nInitSize);
		memset(s_vtTlsImageBackup.data() + nInitSize, 0, nTotalSize - nInitSize);

		bRet = true;
	} while (false);
	return bRet;
}

// based on freebsd 9 libc implementation
// https://github.com/freebsd/freebsd/blob/stable/9/lib/libc/gen/tls.c#L201
void* CTLSHandler::AllocateTLS()
{
	TCB* pTcb = NULL;
	do
	{
		if (t_pTcbRecord)
		{
			pTcb = t_pTcbRecord;
			break;
		}

		byte* pTls = new byte[s_vtTlsImageBackup.size() + sizeof(TCB)];
		memcpy(pTls, s_vtTlsImageBackup.data(), s_vtTlsImageBackup.size());

		// TODO:
		// currently, it seems ps4 game only use these DTVs
		// but who knows..
		// will see if there's crash...
		ulong_ptr* pDtv = new ulong_ptr[3];
		pDtv[0] = 1;
		pDtv[1] = 1;
		pDtv[2] = (ulong_ptr)pTls;

		pTcb = (TCB*)(pTls + s_vtTlsImageBackup.size());
		pTcb->pSegBase = pTcb;
		pTcb->pDTV = pDtv;

		// record this allocation, will free on thread exit
		t_pTcbRecord = pTcb;
	} while (false);
	return pTcb;
}

void CTLSHandler::FreeTLS(TCB* pTcb)
{
	do
	{
		if (!pTcb)
		{
			break;
		}

		ulong_ptr* pDtv = pTcb->pDTV;
		if (pDtv)
		{
			delete pDtv;
		}

		byte* pTls = (byte*)pTcb->pSegBase - s_vtTlsImageBackup.size();
		if (pTls)
		{
			delete pTls;
		}

	} while (false);
}

//////////////////////////////////////////////////////////////////////////
#ifdef GPCS4_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN

void* CTLSHandlerWin::s_pVEHHandle = NULL;



CTLSHandlerWin::CTLSHandlerWin()
{
}

CTLSHandlerWin::~CTLSHandlerWin()
{
}

bool CTLSHandlerWin::Install(void* pTls, uint nInitSize, uint nTotalSize)
{
	bool bRet = false;
	do
	{
		InitZydis();

		s_pVEHHandle = AddVectoredExceptionHandler(TRUE,
			(PVECTORED_EXCEPTION_HANDLER)CTLSHandlerWin::VEHExceptionHandler);
		if (!s_pVEHHandle)
		{
			break;
		}

		if (!BuildTLSBackup(pTls, nInitSize, nTotalSize))
		{
			break;
		}
		bRet = true;
	} while (false);
	return bRet;
}

void CTLSHandlerWin::Uninstall()
{
	if (s_pVEHHandle)
	{
		RemoveVectoredExceptionHandler(s_pVEHHandle);
		s_pVEHHandle = NULL;
	}
}


// exception handler runs in the same thread which cause this exception
long CTLSHandlerWin::VEHExceptionHandler(void* pExceptionArg)
{
	PEXCEPTION_POINTERS pExceptionInfo = (PEXCEPTION_POINTERS)pExceptionArg;
	long nRet = EXCEPTION_CONTINUE_SEARCH;
	do 
	{
		if (pExceptionInfo->ExceptionRecord->ExceptionCode != EXCEPTION_ACCESS_VIOLATION)
		{
			break;
		}

		LOG_DEBUG("exception code %x addr %p",
			pExceptionInfo->ExceptionRecord->ExceptionCode, pExceptionInfo->ExceptionRecord->ExceptionAddress);

		void* pExcptAddr = pExceptionInfo->ExceptionRecord->ExceptionAddress;
		if (!IsTlsAccess(pExcptAddr))
		{
			LOG_ERR("unknown exception raised at %p", pExcptAddr);
			break;
		}	

		// I firstly tried to make a simple hook to the tls access instruction,
		// hoping there's no rip-relative instruction around,
		// but soon I found I was wrong, rip-relative instructions are everywhere
		// thus I need to do relocation...
		// fuck that.
		// let's get rid of that shit and focus on more important things
		// current implementation will raise an exception on every tls access
		// which is not efficient
		// I'll fix this soon or later
		//if (!PatchTLSInstruction(pExcptAddr))
		//{
		//	LOG_ERR("patch tls instruction failed.");
		//	break;
		//}

		uint nMovFsLen = GetMovFsLen(pExcptAddr);
		pExceptionInfo->ContextRecord->Rip += nMovFsLen;
		pExceptionInfo->ContextRecord->Rax = (DWORD64)AllocateTLS();

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

