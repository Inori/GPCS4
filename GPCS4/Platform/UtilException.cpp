#include "UtilException.h"

#include "UtilSync.h"

#include <vector>
#include <mutex>
#include <algorithm>

namespace UtilException
{;

sync::Spinlock                 g_handlerMutex;
std::vector<EXCEPTION_HANDLER> g_handlerArray;

#ifdef GPCS4_WINDOWS

#include <Windows.h>

static void* g_vehHandler = nullptr;


void ConvertExceptionPtoI(EXCEPTION_INFORMATION& Out, PEXCEPTION_POINTERS In)
{
	Out.Code = static_cast<EXCEPTION_CODE>(In->ExceptionRecord->ExceptionCode);
	
	Out.Context.Rax = In->ContextRecord->Rax;
	Out.Context.Rcx = In->ContextRecord->Rcx;
	Out.Context.Rdx = In->ContextRecord->Rdx;
	Out.Context.Rbx = In->ContextRecord->Rbx;
	Out.Context.Rsp = In->ContextRecord->Rsp;
	Out.Context.Rbp = In->ContextRecord->Rbp;
	Out.Context.Rsi = In->ContextRecord->Rsi;
	Out.Context.Rdi = In->ContextRecord->Rdi;
	Out.Context.R8  = In->ContextRecord->R8;
	Out.Context.R9  = In->ContextRecord->R9;
	Out.Context.R10 = In->ContextRecord->R10;
	Out.Context.R11 = In->ContextRecord->R11;
	Out.Context.R12 = In->ContextRecord->R12;
	Out.Context.R13 = In->ContextRecord->R13;
	Out.Context.R14 = In->ContextRecord->R14;
	Out.Context.R15 = In->ContextRecord->R15;

	Out.Context.Rip = In->ContextRecord->Rip;

	if (In->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION ||
		In->ExceptionRecord->ExceptionCode == EXCEPTION_IN_PAGE_ERROR)
	{
		Out.Meta.Access         = static_cast<EXCEPTION_ACCESS>(In->ExceptionRecord->ExceptionInformation[0]);
		Out.Meta.VirtualAddress = In->ExceptionRecord->ExceptionInformation[1];
	}
}

void ConvertExceptionItoP(PEXCEPTION_POINTERS Out, EXCEPTION_INFORMATION& In)
{
	Out->ContextRecord->Rax = In.Context.Rax;
	Out->ContextRecord->Rcx = In.Context.Rcx;
	Out->ContextRecord->Rdx = In.Context.Rdx;
	Out->ContextRecord->Rbx = In.Context.Rbx;
	Out->ContextRecord->Rsp = In.Context.Rsp;
	Out->ContextRecord->Rbp = In.Context.Rbp;
	Out->ContextRecord->Rsi = In.Context.Rsi;
	Out->ContextRecord->Rdi = In.Context.Rdi;
	Out->ContextRecord->R8  = In.Context.R8;
	Out->ContextRecord->R9  = In.Context.R9;
	Out->ContextRecord->R10 = In.Context.R10;
	Out->ContextRecord->R11 = In.Context.R11;
	Out->ContextRecord->R12 = In.Context.R12;
	Out->ContextRecord->R13 = In.Context.R13;
	Out->ContextRecord->R14 = In.Context.R14;
	Out->ContextRecord->R15 = In.Context.R15;

	Out->ContextRecord->Rip = In.Context.Rip;
}

long __stdcall VEHExceptionHandler(void* exceptionArg)
{
	std::lock_guard lock(g_handlerMutex);

	PEXCEPTION_POINTERS ExceptionPointers = (PEXCEPTION_POINTERS)exceptionArg;
	EXCEPTION_ACTION    action;

	do 
	{
		EXCEPTION_INFORMATION ExceptionInfo = {};
		ConvertExceptionPtoI(ExceptionInfo, ExceptionPointers);

		for (auto& handler : g_handlerArray)
		{
			action = handler.Callback(&ExceptionInfo, handler.Context);
			if (action != EXCEPTION_ACTION::CONTINUE_SEARCH)
			{
				break;
			}
		}

		ConvertExceptionItoP(ExceptionPointers, ExceptionInfo);
		
	} while (false);

	return static_cast<long>(action);
}

bool InstallVEH()
{
	bool ret = false;
	do
	{
		if (g_vehHandler)
		{
			ret = true;
			break;
		}

		g_vehHandler = AddVectoredExceptionHandler(TRUE,
												   (PVECTORED_EXCEPTION_HANDLER)VEHExceptionHandler);
		if (!g_vehHandler)
		{
			break;
		}

		ret = true;
	} while (false);
	return ret;
}

void UninstallVEH()
{
	if (g_vehHandler)
	{
		RemoveVectoredExceptionHandler(g_vehHandler);
		g_vehHandler = nullptr;
	}
}

bool AddExceptionHandler(const EXCEPTION_HANDLER& Handler)
{
	std::lock_guard lock(g_handlerMutex);
	bool            ret = false;
	do 
	{
		if (!Handler.Callback)
		{
			break;
		}

		if (g_handlerArray.empty())
		{
			if (!InstallVEH())
			{
				break;
			}
		}

		g_handlerArray.emplace_back(Handler);

		ret = true;
	} while (false);
	return ret;
}

bool RemoveExceptionHandler(const EXCEPTION_HANDLER& Handler)
{
	std::lock_guard lock(g_handlerMutex);
	bool            ret = false;
	do
	{
		if (!Handler.Callback)
		{
			break;
		}

		auto equalHandler = [&Handler](const EXCEPTION_HANDLER& item) 
		{
			return Handler.Callback == item.Callback &&
				   Handler.Context == item.Context;
		};

		auto iter = std::find_if(g_handlerArray.begin(), g_handlerArray.end(), equalHandler);
		if (iter == g_handlerArray.end())
		{
			break;
		}

		g_handlerArray.erase(iter);

		if (g_handlerArray.empty())
		{
			UninstallVEH();
		}

		ret = true;
	} while (false);
	return ret;
}

#else

#include <signal.h>

void AddExceptionHandler(const EXCEPTION_HANDLER* Handler)
{
}

void RemoveExceptionHandler(const EXCEPTION_HANDLER* Handler)
{
}

#endif



}  // namespace UtilException