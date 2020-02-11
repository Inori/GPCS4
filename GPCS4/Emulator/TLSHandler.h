#pragma once

#include "GPCS4Common.h"
#include "UtilSingleton.h"
#include "zydis/Zydis.h"

#include <array>
#include <mutex>
#include <vector>

// TLS index and module id for the main executable.
constexpr uint32_t TLS_MODULE_ID_MAIN = 1;

// TLS types:
// 1.TLS variable locally defined and used within an executable
// 2.TLS variable externally defined in a shared object but used in a executable
// 3.TLS variable locally defined in a shared object and used in the same shared object
// 4.TLS variable externally defined in a shared object and used in an arbitrary shared object
//
// Relocation types:
// R_X86_64_TPOFF64
// R_X86_64_DTPOFF64
// R_X86_64_DTPMOD64

// TODO:
// To fully support all TLS types, the above TLS types and relocation types
// should be all implemented.
// Currently, only type 1 TLS is supported.

struct TLSBlock
{
	// TLS segment address in loaded module.
	void* address = nullptr;
	// initialized TLS data size
	uint32_t initSize = 0;
	// total TLS data size (initialized + uninitialized)
	uint32_t totalSize = 0;
	// alignment
	uint32_t align = 0;
	// tls index == module id
	uint32_t index = 0;
	// Dynamically-loaded modules here don¡¯t mean any dynamic shared objects,
	// they only refer to the shared objects that are loaded by explicitly calling dlopen
	bool isDynamic = false;
	// TLS image offset at TCB block
	uint32_t offset;
};

// The TLSManager implementation is based on FreeBSD 9.0 stable.
// https://github.com/freebsd/freebsd/blob/stable/9/libexec/rtld-elf/rtld_tls.h
// https://github.com/freebsd/freebsd/blob/stable/9/libexec/rtld-elf/rtld.c#L4305
// And with the help of this post.
// https://chao-tic.github.io/blog/2018/12/25/tls

class TLSManagerWin;
class TLSManagerLinux;

#ifdef GPCS4_WINDOWS
using TLSSingletonBase = Singleton<TLSManagerWin>;
#else   // GPCS4_WINDOWS
using TLSSingletonBase = Singleton<TLSManagerLinux>;
#endif  // GPCS4_WINDOWS

class TLSManager : public TLSSingletonBase
{
private:
	union DTV
	{
		void* pointer;
		uintptr_t counter;
	};

	struct TCB
	{
		void* segbase;
		void* dtv;
	};

	using TLSImage = std::vector<uint8_t>;

public:
	TLSManager();
	virtual ~TLSManager();

	virtual bool install() = 0;

	virtual void uninstall() = 0;

	void registerTLSBlock(const TLSBlock& block);

	void unregisterTLSBlock(const TLSBlock& block);

	void* tlsGetAddr(uint32_t moduleId, uint32_t offset);

	void notifyThreadExit();

protected:
	void* allocateTLS();
	void freeTLS(void* tls);

	void* readFSRegister(int32_t offset);

private:
	void backupTLSImage(std::vector<uint8_t>& image, const TLSBlock& block);

	size_t calculateStaticTLSSize();
	void allocateTLSOffset(TLSBlock& block);

protected:
	// emulated fs register
	static thread_local void* t_fsbase;

private:
	std::vector<std::pair<TLSBlock, TLSImage>> m_TLSImages;
	std::mutex m_mutex;
};

class AssembleHelper
{
public:
	AssembleHelper();
	~AssembleHelper();

	bool isTlsAccess(void* code);

	void getMovFsInfo(void* code, uint32_t& instLen, int64_t& fsOffset);

	bool patchTLSInstruction(void* code);

	void printInstruction(void* code);

private:
	void initZydis();

	void printInst(ZydisDecodedInstruction& inst);

	uint32_t getPatchLen(uint8_t* code, uint32_t oldLen);

private:
	ZydisDecoder m_decoder;
	ZydisFormatter m_formatter;
};

#ifdef GPCS4_WINDOWS

class TLSManagerWin : public TLSManager
{
	friend class Singleton<TLSManagerWin>;

public:
	virtual bool install();

	virtual void uninstall();

private:
	static long __stdcall VEHExceptionHandler(void* exceptionArg);

private:
	static void* s_vehHandler;
	static AssembleHelper s_asmHelper;

private:
	TLSManagerWin();
	virtual ~TLSManagerWin();
	TLSManagerWin(const TLSManagerWin&) = delete;
	TLSManagerWin& operator=(const TLSManagerWin&) = delete;
};

#else  // GPCS4_WINDOWS

class TLSManagerLinux : public TLSManager
{
	friend class Singleton<TLSManagerLinux>;

public:
private:
	TLSManagerLinux();
	virtual ~TLSManagerLinux();
	TLSManagerLinux(const TLSManagerLinux&) = delete;
	TLSManagerLinux& operator=(const TLSManagerLinux&) = delete;
};

#endif  // GPCS4_WINDOWS

bool installTLSManager();
void uninstallTLSManager();