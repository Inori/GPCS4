#pragma once

#include "arch/x86/reg.h"
#include "common.h"
#include "api_helper.h"

#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <mutex>
#include <memory>

#include <Windows.h>


struct SavedRegisters
{
	CONTEXT saved_context;
};

typedef union _LDR_DLL_NOTIFICATION_DATA LDR_DLL_NOTIFICATION_DATA;


class Executor
{
	friend class UnwindGenerator;
	friend class ApiHelper;
	friend class ModuleHelper;
	friend class ShellcodeHelper;
#if defined(_WIN64)
	friend class WinUnwindGenerator;
#elif __APPLE__
	friend class UnwindGeneratorMacOS;
#endif

public:
	Executor();
	virtual ~Executor();

	virtual void Init(const std::vector<TargetModule>& target_modules,
					  const Options&                   options);

	virtual void Unit();

	long OnVEHException(EXCEPTION_POINTERS* ExceptionInfo);

protected:
	enum ExceptionType
	{
		BREAKPOINT,
		ACCESS_VIOLATION,
		ILLEGAL_INSTRUCTION,
		STACK_OVERFLOW,
		OTHER
	};

	struct Exception
	{
		ExceptionType type;
		void*         ip;
		bool          maybe_write_violation;
		bool          maybe_execute_violation;
		void*         access_address;
	};

	using Context = ::CONTEXT;

	virtual void OnEntrypoint();
	virtual void OnProcessCreated();
	virtual void OnProcessExit();
	virtual void OnModuleLoaded(void* module, char* module_name);
	virtual void OnModuleUnloaded(void* module);
	virtual bool OnException(Exception* exception_record, Context* context_record);
	virtual void OnCrashed(Exception* exception_record);

	virtual size_t GetTranslatedAddress(size_t address);

protected:
	enum MemoryProtection
	{
		READONLY,
		READWRITE,
		READEXECUTE,
		READWRITEEXECUTE
	};

protected:
	void ExtractAndProtectCodeRanges(void*                    module_base,
									 size_t                   min_address,
									 size_t                   max_address,
									 std::list<AddressRange>* executable_ranges,
									 size_t*                  code_size);

	void ProtectCodeRanges(
		std::list<AddressRange>* executable_ranges);

	uint32_t GetImageSize(void* base_address);
	void     GetImageSize(
			void*   base_address,
			size_t* min_address,
			size_t* max_address);

	void GetExceptionHandlers(size_t module_haeder, std::unordered_set<size_t>& handlers);

	void PatchPointersRemote(size_t min_address, size_t max_address, std::unordered_map<size_t, size_t>& search_replace);
	template <typename T>
	void PatchPointersRemoteT(size_t min_address, size_t max_address, std::unordered_map<size_t, size_t>& search_replace);

	void* RemoteAllocateNear(uint64_t         region_min,
							 uint64_t         region_max,
							 size_t           size,
							 MemoryProtection protection,
							 bool             use_shared_memory = false);
	void  RemoteFree(void* address, size_t size);
	void  RemoteWrite(void* address, void* buffer, size_t size);
	void  RemoteRead(void* address, void* buffer, size_t size);
	void  RemoteProtect(void* address, size_t size, MemoryProtection protect);

	size_t GetRegister(Context* context, Register r);
	void   SetRegister(Context* context, Register r, size_t value);

	void SaveRegisters(Context* context, SavedRegisters* registers);
	void RestoreRegisters(Context* context, SavedRegisters* registers);

	uint32_t GetProcOffset(void* module, const char* name);

protected:
	int32_t child_ptr_size           = sizeof(void*);
	bool    child_entrypoint_reached = false;
	bool    shellcode_mode           = false;

	static Executor* instance;

private:
	void* InstallVEHHandler();
	void UninstallVEHHandler(void* handle);

	static long __stdcall VectoredExceptionHandler(EXCEPTION_POINTERS* ExceptionInfo);
	
	static void __stdcall DllNotificationHandler(
		unsigned long                    notification_reason,
		const LDR_DLL_NOTIFICATION_DATA* notification_data,
		void*                            context);

	void* RemoteAllocateBefore(uint64_t         min_address,
							   uint64_t         max_address,
							   size_t           size,
							   MemoryProtection protection);

	void* RemoteAllocateAfter(uint64_t         min_address,
							  uint64_t         max_address,
							  size_t           size,
							  MemoryProtection protection);

	uint32_t WindowsProtectionFlags(MemoryProtection protection);

	uint32_t GetLoadedModules(void** modules);

	void ConvertException(EXCEPTION_RECORD* win_exception_record,
						  Exception*        exception);

private:
	bool trace_debug_events = false;

	void*  veh_handle             = nullptr;
	void*  dll_notify_cookie      = nullptr;
	size_t allocation_granularity = 0;
	void*  self_handle            = nullptr;
	bool   have_thread_context    = false;

	std::vector<TargetModule>  instrument_modules;
	std::mutex                 exception_mutex;
	std::unique_ptr<ApiHelper> api_helper;
};
