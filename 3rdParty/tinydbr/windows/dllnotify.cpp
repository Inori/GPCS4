#include "dllnotify.h"

#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS 0
#endif

typedef NTSTATUS (NTAPI *PfuncLdrRegisterDllNotification)(
	ULONG                          Flags,
	PLDR_DLL_NOTIFICATION_FUNCTION NotificationFunction,
	PVOID                          Context,
	PVOID*                         Cookie);

typedef NTSTATUS (NTAPI *PfuncLdrUnregisterDllNotification)(
	PVOID Cookie);

PfuncLdrRegisterDllNotification LdrRegisterDllNotification = nullptr;
PfuncLdrUnregisterDllNotification LdrUnregisterDllNotification = nullptr;

void* InstallDllNotification(PLDR_DLL_NOTIFICATION_FUNCTION NotificationFunction, PVOID Context)
{
	void* pCookie = nullptr;
	do 
	{
		if (!LdrRegisterDllNotification)
		{
			HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
			if (!hNtdll)
			{
				break;
			}

			LdrRegisterDllNotification = (PfuncLdrRegisterDllNotification)GetProcAddress(hNtdll, "LdrRegisterDllNotification");
			if (!LdrRegisterDllNotification)
			{
				break;
			}
		}

		NTSTATUS status = LdrRegisterDllNotification(0, NotificationFunction, Context, &pCookie);
		if (status != STATUS_SUCCESS)
		{
			break;
		}

	} while (false);
	return pCookie;
}

void UninstallDllNotification(void* Cookie)
{
	do 
	{
		if (!LdrUnregisterDllNotification)
		{
			HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
			if (!hNtdll)
			{
				break;
			}

			LdrUnregisterDllNotification = (PfuncLdrUnregisterDllNotification)GetProcAddress(hNtdll, "LdrUnregisterDllNotification");
			if (!LdrUnregisterDllNotification)
			{
				break;
			}
		}

		LdrUnregisterDllNotification(Cookie);
	} while (false);
}
