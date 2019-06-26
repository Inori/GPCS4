#include "TLSHandler.h"

CTLSHandler::CTLSHandler()
{
}

CTLSHandler::~CTLSHandler()
{
}



void CTLSHandler::InitZydis()
{
	ZydisDecoderInit(&m_oDecoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_ADDRESS_WIDTH_64);
	ZydisFormatterInit(&m_oFormatter, ZYDIS_FORMATTER_STYLE_INTEL);
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
	do 
	{
		LOG_DEBUG("exception code %x addr %p",
			pExceptionInfo->ExceptionRecord->ExceptionCode, pExceptionInfo->ExceptionRecord->ExceptionAddress);



	} while (false);
	return EXCEPTION_CONTINUE_EXECUTION;
}

#else


CTLSHandlerLinux::CTLSHandlerLinux()
{
}

CTLSHandlerLinux::~CTLSHandlerLinux()
{
}


#endif  //GPCS4_WINDOWS

