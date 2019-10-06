#pragma once

#include "GPCS4Common.h"
#include "Singleton.h"
#include "zydis/Zydis.h"
#include <vector>


class CTLSHandler
{
public:
	CTLSHandler();
	virtual ~CTLSHandler();

	virtual bool initialize(void *pTls, uint initSize, uint totalSize) = 0;

	static void NotifyThreadCreate(uint nTid);

	static void NotifyThreadExit(uint nTid);

protected:
	struct TCB
	{
		void* pSegBase;
		ulong_ptr* pDTV;
	};

protected:
	static void InitZydis();
	static void PrintInst(ZydisDecodedInstruction& inst);
	static bool IsTlsAccess(void* pCode);
	static uint GetPatchLen(byte* pCode, uint nOldLen);
	static uint GetMovFsLen(void* pCode);
	static bool PatchTLSInstruction(void* pCode);
	//
	static bool BuildTLSBackup(void* pTls, uint nInitSize, uint nTotalSize);
	static void* AllocateTLS();
	static void FreeTLS(TCB* pTcb);
protected:
	static ZydisDecoder s_oDecoder;
	static ZydisFormatter s_oFormatter;
	// build tls backup on install,
	// whenever a new thread created,
	// we just copy this backup
	static std::vector<byte> s_vtTlsImageBackup;
	static thread_local TCB* t_pTcbRecord;
private:

};


//////////////////////////////////////////////////////////////////////////
// Windows tls implementation


#ifdef GPCS4_WINDOWS

class CTLSHandlerWin : public CTLSHandler
{
public:
	CTLSHandlerWin();
	virtual ~CTLSHandlerWin();

	static bool Install(void* pTls, uint nInitSize, uint nTotalSize);
	virtual bool initialize(void *pTls, uint initSize, uint totalSize);

	static void Uninstall();


private:
	static long __stdcall VEHExceptionHandler(void* pExceptionArg);

private:

	static void* s_pVEHHandle;
	
};


#else

// on linux, the system provide us a syscall "sys_arch"
// to change the fs register.
// maybe there's a more native way to handle TLS,
// but I suggest using the same way as windows,
// that is make fs segment access raising an exception,
// then catch the exception and patch the code
// for two reason:
// 1. the emulator itself use C++11's "thread_local" feature,
// which make use of fs register
// 2. keep the code consistency, we don't need to create a new framework

class CTLSHandlerLinux : public CTLSHandler
{
public:
	CTLSHandlerLinux();
	virtual ~CTLSHandlerLinux();

private:

};

#endif  //GPCS4_WINDOWS
