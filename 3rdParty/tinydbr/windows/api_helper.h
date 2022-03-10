#pragma once

#include "common.h"
#include <unordered_set>
#include <list>

class Executor;

class ApiHelper
{
public:
	ApiHelper(Executor& executor);

	virtual ~ApiHelper();

	virtual void ExtractAndProtectCodeRanges(
		void*                    module_base,
		size_t                   min_address,
		size_t                   max_address,
		std::list<AddressRange>* executable_ranges,
		size_t*                  code_size);

	virtual void ProtectCodeRanges(
		std::list<AddressRange>* executable_ranges);

	virtual uint32_t GetImageSize(void* base_address) = 0;

	virtual uint32_t GetProcOffset(void* module, const char* name) = 0;

	virtual void GetExceptionHandlers(
		size_t module_header, std::unordered_set<size_t>& handlers) = 0;

protected:
	Executor& m_executor;
};

///////////////////////////////////////////////////////////////////

class ModuleHelper : public ApiHelper
{
public:
	ModuleHelper(Executor& executor);
	virtual ~ModuleHelper();

	virtual uint32_t GetImageSize(void* base_address) override;

	virtual uint32_t GetProcOffset(void* module, const char* name) override;

	virtual void GetExceptionHandlers(
		size_t module_header, std::unordered_set<size_t>& handlers) override;

private:
};

///////////////////////////////////////////////////////////////////

class ShellcodeHelper : public ApiHelper
{
public:
	ShellcodeHelper(Executor& executor);
	virtual ~ShellcodeHelper();

	virtual uint32_t GetImageSize(void* base_address) override;

	virtual uint32_t GetProcOffset(void* module, const char* name) override;

	virtual void GetExceptionHandlers(
		size_t module_header, std::unordered_set<size_t>& handlers) override;

private:
	const TargetModule* GetModule(void* base_address);
};

