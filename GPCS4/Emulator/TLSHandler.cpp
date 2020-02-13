#include "TLSHandler.h"
#include "Platform/UtilMemory.h"
#include "Util/UtilMath.h"
// x86_64 max instruction length
#define X64_INSTRUCTION_LEN_MAX 15

LOG_CHANNEL(Emulator.TLSHandler);

thread_local void* TLSManager::t_fsbase = nullptr;

TLSManager::TLSManager()
{
}

TLSManager::~TLSManager()
{
}

void TLSManager::backupTLSImage(std::vector<uint8_t>& image, const TLSBlock& block)
{
	size_t algnedSize = util::alignRound(block.totalSize, block.align);
	image.resize(algnedSize);
	memcpy(image.data(), block.address, block.initSize);

	uint32_t uninitSize = algnedSize - block.initSize;
	memset(image.data() + block.initSize, 0, uninitSize);
}

size_t TLSManager::calculateStaticTLSSize()
{
	size_t size = 0;
	for (const auto& imgPair : m_TLSImages)
	{
		if (imgPair.first.isDynamic)
		{
			continue;
		}
		size += util::alignRound(imgPair.first.totalSize, imgPair.first.align);
	}
	return size;
}

void TLSManager::allocateTLSOffset(TLSBlock& block)
{
	LOG_ASSERT(block.index != 0, "tls index is 0.");

	uint32_t offset = 0;
	if (block.index == TLS_MODULE_ID_MAIN)
	{
		offset = util::alignRound(block.totalSize, block.align);
	}
	else
	{
		uint32_t lastTLSOffset = calculateStaticTLSSize();
		offset                 = lastTLSOffset + util::alignRound(block.totalSize, block.align);
	}

	block.offset = offset;
}

void TLSManager::registerTLSBlock(const TLSBlock& block)
{
	do 
	{
		if (!block.address || !block.totalSize)
		{
			// No TLS in this module
			break;
		}

		std::vector<uint8_t> tlsImage;

		backupTLSImage(tlsImage, block);

		TLSBlock newBlock = block;
		allocateTLSOffset(newBlock);

		m_TLSImages.emplace_back(newBlock, std::move(tlsImage));

	} while (false);
}

void TLSManager::unregisterTLSBlock(const TLSBlock& block)
{
	do 
	{
		auto iter = std::find_if(m_TLSImages.begin(), m_TLSImages.end(),
		[&](const auto& imgPair) 
		{
			return block.index == imgPair.first.index;
		});

		if (iter == m_TLSImages.end())
		{
			break;
		}

		m_TLSImages.erase(iter);

	} while (false);
}


void* TLSManager::tlsGetAddr(uint32_t moduleId, uint32_t offset)
{
	// TODO:
	return calloc(1, 0x1000);
}

void TLSManager::notifyThreadExit()
{
	freeTLS(t_fsbase);
}

void* TLSManager::allocateTLS()
{
	std::lock_guard<std::mutex> lock(m_mutex);

	TCB* tcbSegbase = nullptr;
	do 
	{
		size_t imageSize = calculateStaticTLSSize();
		if (!imageSize)
		{
			break;
		}

		uint32_t moduleCount = m_TLSImages.size();
		uint8_t* tlsAndTCB   = reinterpret_cast<uint8_t*>(calloc(1, imageSize + sizeof(TCB)));
		DTV* dtv             = reinterpret_cast<DTV*>(calloc(1, (moduleCount + 2) * sizeof(DTV)));

		dtv[0].counter = moduleCount;
		dtv[1].counter = moduleCount;

		tcbSegbase          = reinterpret_cast<TCB*>(tlsAndTCB + imageSize);
		tcbSegbase->segbase = tcbSegbase;
		tcbSegbase->dtv     = dtv;

		for (const auto& imgPair : m_TLSImages)
		{
			void* dst = reinterpret_cast<uint8_t*>(tcbSegbase) - imgPair.first.offset;
			// copy tls image backup to new allocated memory bound to current thread.
			std::memcpy(dst, imgPair.second.data(), imgPair.second.size());
			// update dtv array
			dtv[imgPair.first.index + 1].pointer = dst;
		}

	} while (false);
	return reinterpret_cast<void*>(tcbSegbase);
}

void TLSManager::freeTLS(void* tls)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	do
	{
		if (!tls)
		{
			break;
		}

		size_t imageSize = calculateStaticTLSSize();
		void* tlsStart   = reinterpret_cast<uint8_t*>(tls) - imageSize;
		void* dtv        = reinterpret_cast<TCB*>(tls)->dtv;

		free(tlsStart);
		free(dtv);

	} while (false);
}

void* TLSManager::readFSRegister(int32_t offset)
{
	if (!t_fsbase)
	{
		t_fsbase = allocateTLS();
	}
	return reinterpret_cast<uint8_t*>(t_fsbase) + offset;
}


AssembleHelper::AssembleHelper()
{
	initZydis();
}

AssembleHelper::~AssembleHelper()
{
}


bool AssembleHelper::isTlsAccess(void* code)
{
	bool ret = false;
	do
	{
		if (!code)
		{
			break;
		}

		ZydisDecodedInstruction instruction;
		ZydisStatus status = ZydisDecoderDecodeBuffer(&m_decoder, code,
													  X64_INSTRUCTION_LEN_MAX, (ZydisU64)code, &instruction);
		if (!ZYDIS_SUCCESS(status))
		{
			LOG_ERR("decode instruction failed at %p", code);
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
		// so currently we only support
		// mov rax, fs:[0x0000000000000000]
		// this should be easy to extend to other cases
		if (instruction.operands[0].reg.value != ZYDIS_REGISTER_RAX ||
			instruction.operands[1].mem.segment != ZYDIS_REGISTER_FS)
		{
			break;
		}

		LOG_ASSERT(instruction.raw.disp.value == 0, "only support mov rax, fs:[0x0000000000000000] currently.");

		ret = true;
	} while (false);
	return ret;
}

void AssembleHelper::getMovFsInfo(void* code, uint32_t& instLen, int64_t& fsOffset)
{
	ZydisDecodedInstruction instruction;
	ZydisDecoderDecodeBuffer(&m_decoder, code,
							 X64_INSTRUCTION_LEN_MAX, (ZydisU64)code, &instruction);
	instLen  = instruction.length;
	fsOffset = instruction.raw.disp.value;
}

bool AssembleHelper::patchTLSInstruction(void* code)
{
	bool ret = false;

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
		if (!code)
		{
			break;
		}

		uint32_t nPatchedLen = getPatchLen((uint8_t*)code, sizeof(JMP_CODE_HEAD));
		if (!nPatchedLen)
		{
			break;
		}

		// first, build foot jmp code
		JMP_CODE_FOOT jmpFoot;
		jmpFoot.nJmpVal = (uint64_t)((uint8_t*)code + nPatchedLen);
		uint32_t nMovFsLen  = 0;
		int64_t fsOffset  = 0;
		getMovFsInfo(code, nMovFsLen, fsOffset);

		// delete mov rax, fs:[0]
		// go straight to next instruction
		uint8_t* pMovNext    = (uint8_t*)code + nMovFsLen;
		uint32_t nMovLeftLen  = nPatchedLen - nMovFsLen;
		uint32_t nFootCodeLen = nMovLeftLen + sizeof(jmpFoot);
		uint8_t* pFootCode   = (uint8_t*)UtilMemory::VMMapFlexible(nullptr, nFootCodeLen, UtilMemory::VMPF_READ_WRITE_EXEC);
		if (!pFootCode)
		{
			break;
		}

		memcpy(pFootCode, pMovNext, nMovLeftLen);
		memcpy(pFootCode + nMovLeftLen, &jmpFoot, sizeof(jmpFoot));

		// second, build head jmp code
		JMP_CODE_HEAD jmpHead;
		jmpHead.nPushVal = (uint32_t)((uint64_t)pFootCode & 0xFFFFFFFF);
		jmpHead.nMovVal  = (uint32_t)((uint64_t)pFootCode >> 32);

		// In asm file.
		// jmpHead.nJmpVal  = (uint64_t)GetTlsDataStub;

		// make game code writable
		if (!UtilMemory::VMProtect(code, sizeof(jmpHead), UtilMemory::VMPF_READ_WRITE_EXEC))
		{
			break;
		}

		memcpy(code, &jmpHead, sizeof(jmpHead));

		ret = true;
	} while (false);
	return ret;
}

void AssembleHelper::printInstruction(void* code)
{
	ZydisDecodedInstruction instruction;
	ZydisDecoderDecodeBuffer(&m_decoder, code,
							 X64_INSTRUCTION_LEN_MAX, (ZydisU64)code, &instruction);
	printInst(instruction);
}

void AssembleHelper::initZydis()
{
	ZydisDecoderInit(&m_decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_ADDRESS_WIDTH_64);
	ZydisFormatterInit(&m_formatter, ZYDIS_FORMATTER_STYLE_INTEL);
}

void AssembleHelper::printInst(ZydisDecodedInstruction& inst)
{
	const int bufLen = 256;
	char szBuffer[bufLen];
	ZydisFormatterFormatInstruction(&m_formatter, &inst, szBuffer, sizeof(szBuffer));
	LOG_DEBUG("instruction: %s", szBuffer);
}

uint32_t AssembleHelper::getPatchLen(uint8_t* code, uint32_t oldLen)
{
	uint32_t sumLen = 0;
	ZydisDecodedInstruction instruction;
	while (sumLen < oldLen)
	{
		ZydisStatus status = ZydisDecoderDecodeBuffer(&m_decoder, code,
													  X64_INSTRUCTION_LEN_MAX, (ZydisU64)code, &instruction);
		if (!ZYDIS_SUCCESS(status))
		{
			LOG_ERR("decode instruction failed at %p", code);
			break;
		}

#ifdef GPCS4_DEBUG
		printInst(instruction);
#endif
		// TODO:
		// if patched instruction contains position relative ones
		// we currently not support
		if (instruction.attributes & ZYDIS_ATTRIB_IS_RELATIVE)
		{
			LOG_ERR("patched instruction contains relative inst.");
			sumLen = 0;
			break;
		}

		code += instruction.length;
		sumLen += instruction.length;
	}
	return sumLen;
}

#ifdef GPCS4_WINDOWS

#include <Windows.h>

void* TLSManagerWin::s_vehHandler = nullptr;

AssembleHelper TLSManagerWin::s_asmHelper;

TLSManagerWin::TLSManagerWin()
{
}

TLSManagerWin::~TLSManagerWin()
{
}


bool TLSManagerWin::install()
{
	bool ret = false;
	do
	{
		if (s_vehHandler)
		{
			ret = true;
			break;
		}

		s_vehHandler = AddVectoredExceptionHandler(TRUE,
												   (PVECTORED_EXCEPTION_HANDLER)TLSManagerWin::VEHExceptionHandler);
		if (!s_vehHandler)
		{
			break;
		}

		ret = true;
	} while (false);
	return ret;
}

void TLSManagerWin::uninstall()
{
	if (s_vehHandler)
	{
		RemoveVectoredExceptionHandler(s_vehHandler);
		s_vehHandler = nullptr;
	}
}

long __stdcall TLSManagerWin::VEHExceptionHandler(void* exceptionArg)
{
	PEXCEPTION_POINTERS pExceptionInfo = (PEXCEPTION_POINTERS)exceptionArg;
	long nRet                          = EXCEPTION_CONTINUE_SEARCH;
	do
	{
		void* pExcptAddr = pExceptionInfo->ExceptionRecord->ExceptionAddress;

#ifdef GPCS4_DEBUG
		// Useful for viewing random crash instructions.
		s_asmHelper.printInstruction(pExcptAddr);
#endif // GPCS4_DEBUG

		if (pExceptionInfo->ExceptionRecord->ExceptionCode != EXCEPTION_ACCESS_VIOLATION)
		{
			break;
		}

		LOG_DEBUG("exception code %x addr %p",
				  pExceptionInfo->ExceptionRecord->ExceptionCode, pExceptionInfo->ExceptionRecord->ExceptionAddress);

		if (!s_asmHelper.isTlsAccess(pExcptAddr))
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

		TLSManagerWin* tlsMgrWin = TLSManagerWin::GetInstance();

		uint32_t instLen = 0;
		int64_t fsOffset = 0;
		s_asmHelper.getMovFsInfo(pExcptAddr, instLen, fsOffset);
		
		pExceptionInfo->ContextRecord->Rip += instLen;
		pExceptionInfo->ContextRecord->Rax = reinterpret_cast<uintptr_t>(tlsMgrWin->readFSRegister(fsOffset));

		nRet = EXCEPTION_CONTINUE_EXECUTION;
	} while (false);
	return nRet;
}

#else  // GPCS4_WINDOWS

TLSManagerLinux::TLSManagerLinux()
{
}

TLSManagerLinux::~TLSManagerLinux()
{
}

#endif  // GPCS4_WINDOWS


bool installTLSManager()
{
	bool ret = false;
	do
	{
		TLSManager* tlsMgr = TLSManager::GetInstance();
		if (!tlsMgr)
		{
			break;
		}
		if (!tlsMgr->install())
		{
			break;
		}
		ret = true;
	} while (false);
	return ret;
}

void uninstallTLSManager()
{
	TLSManager* tlsMgr = TLSManager::GetInstance();
	tlsMgr->uninstall();
}