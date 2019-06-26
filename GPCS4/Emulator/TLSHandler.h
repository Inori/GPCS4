#pragma once

#include "GPCS4Common.h"
#include "Singleton.h"
#include "zydis/Zydis.h"


class CTLSHandler
{
public:
	CTLSHandler();
	virtual ~CTLSHandler();

	virtual bool Install() = 0;

	virtual void Uninstall() = 0;

protected:
	void InitZydis();
	bool IsTlsAccess(void* pCode);

protected:
	ZydisDecoder m_oDecoder;
	ZydisFormatter m_oFormatter;

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

	virtual bool Install() override;


	virtual void Uninstall() override;

private:
	static long __stdcall StaticExceptionHandler(void* pExceptionArg);

private:
	void* m_pVEHHandle;
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
