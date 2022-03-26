#include "PlatException.h"
#include "UtilSync.h"

#include <vector>
#include <mutex>
#include <algorithm>

namespace plat
{

util::sync::Spinlock          g_handlerMutex;
std::vector<ExceptionHandler> g_handlerArray;

#ifdef GPCS4_WINDOWS

#include <Windows.h>

static void* g_vehHandler = nullptr;


void convertExceptionPtoR(ExceptionRecord& out, PEXCEPTION_POINTERS in)
{
	out.code = static_cast<ExceptionCode>(in->ExceptionRecord->ExceptionCode);
	
	out.context.Rax = in->ContextRecord->Rax;
	out.context.Rcx = in->ContextRecord->Rcx;
	out.context.Rdx = in->ContextRecord->Rdx;
	out.context.Rbx = in->ContextRecord->Rbx;
	out.context.Rsp = in->ContextRecord->Rsp;
	out.context.Rbp = in->ContextRecord->Rbp;
	out.context.Rsi = in->ContextRecord->Rsi;
	out.context.Rdi = in->ContextRecord->Rdi;
	out.context.R8  = in->ContextRecord->R8;
	out.context.R9  = in->ContextRecord->R9;
	out.context.R10 = in->ContextRecord->R10;
	out.context.R11 = in->ContextRecord->R11;
	out.context.R12 = in->ContextRecord->R12;
	out.context.R13 = in->ContextRecord->R13;
	out.context.R14 = in->ContextRecord->R14;
	out.context.R15 = in->ContextRecord->R15;
	
	out.context.Rip = in->ContextRecord->Rip;

	out.context.EFlags = in->ContextRecord->EFlags;

	if (in->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION ||
		in->ExceptionRecord->ExceptionCode == EXCEPTION_IN_PAGE_ERROR)
	{
		out.info.access         = static_cast<ExceptionAccess>(in->ExceptionRecord->ExceptionInformation[0]);
		out.info.virtualAddress = in->ExceptionRecord->ExceptionInformation[1];
	}
}

void convertExceptionRtoP(PEXCEPTION_POINTERS out, ExceptionRecord& in)
{
	out->ContextRecord->Rax = in.context.Rax;
	out->ContextRecord->Rcx = in.context.Rcx;
	out->ContextRecord->Rdx = in.context.Rdx;
	out->ContextRecord->Rbx = in.context.Rbx;
	out->ContextRecord->Rsp = in.context.Rsp;
	out->ContextRecord->Rbp = in.context.Rbp;
	out->ContextRecord->Rsi = in.context.Rsi;
	out->ContextRecord->Rdi = in.context.Rdi;
	out->ContextRecord->R8  = in.context.R8;
	out->ContextRecord->R9  = in.context.R9;
	out->ContextRecord->R10 = in.context.R10;
	out->ContextRecord->R11 = in.context.R11;
	out->ContextRecord->R12 = in.context.R12;
	out->ContextRecord->R13 = in.context.R13;
	out->ContextRecord->R14 = in.context.R14;
	out->ContextRecord->R15 = in.context.R15;

	out->ContextRecord->Rip = in.context.Rip;

	out->ContextRecord->EFlags = in.context.EFlags;
}

long __stdcall VEHExceptionHandler(void* exceptionArg)
{
	std::lock_guard lock(g_handlerMutex);

	PEXCEPTION_POINTERS exceptionPointers = (PEXCEPTION_POINTERS)exceptionArg;
	ExceptionAction     action            = ExceptionAction::CONTINUE_SEARCH;

	do 
	{
		ExceptionRecord exceptionRecord = {};
		convertExceptionPtoR(exceptionRecord, exceptionPointers);

		for (auto& handler : g_handlerArray)
		{
			action = handler.callback(&exceptionRecord, handler.param);
			if (action != ExceptionAction::CONTINUE_SEARCH)
			{
				break;
			}
		}

		convertExceptionRtoP(exceptionPointers, exceptionRecord);
		
	} while (false);

	return static_cast<long>(action);
}

bool installVEH()
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

void uninstallVEH()
{
	if (g_vehHandler)
	{
		RemoveVectoredExceptionHandler(g_vehHandler);
		g_vehHandler = nullptr;
	}
}

bool addExceptionHandler(const ExceptionHandler& handler)
{
	std::lock_guard lock(g_handlerMutex);
	bool            ret = false;
	do 
	{
		if (!handler.callback)
		{
			break;
		}

		if (g_handlerArray.empty())
		{
			if (!installVEH())
			{
				break;
			}
		}

		g_handlerArray.emplace_back(handler);

		ret = true;
	} while (false);
	return ret;
}

bool removeExceptionHandler(const ExceptionHandler& handler)
{
	std::lock_guard lock(g_handlerMutex);
	bool            ret = false;
	do
	{
		if (!handler.callback)
		{
			break;
		}

		auto equalHandler = [&handler](const ExceptionHandler& item) 
		{
			return handler.callback == item.callback &&
				   handler.param == item.param;
		};

		auto iter = std::find_if(g_handlerArray.begin(), g_handlerArray.end(), equalHandler);
		if (iter == g_handlerArray.end())
		{
			break;
		}

		g_handlerArray.erase(iter);

		if (g_handlerArray.empty())
		{
			uninstallVEH();
		}

		ret = true;
	} while (false);
	return ret;
}

#else

#include <signal.h>

void addExceptionHandler(const ExceptionHandler* Handler)
{
}

void removeExceptionHandler(const ExceptionHandler* Handler)
{
}

#endif



}  // namespace UtilException