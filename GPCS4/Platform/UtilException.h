#pragma once

#include "GPCS4Common.h"


namespace UtilException
{;


enum class EXCEPTION_ACTION
{
	CONTINUE_SEARCH    = 0,
	CONTINUE_EXECUTION = -1
};


enum EXCEPTION_CODE
{
	EXCEPTION_ACCESS_VIOLATION         = 0xC0000005L,
	EXCEPTION_DATATYPE_MISALIGNMENT    = 0x80000002L,
	EXCEPTION_BREAKPOINT               = 0x80000003L,
	EXCEPTION_SINGLE_STEP              = 0x80000004L,
	EXCEPTION_ARRAY_BOUNDS_EXCEEDED    = 0xC000008CL,
	EXCEPTION_FLT_DENORMAL_OPERAND     = 0xC000008DL,
	EXCEPTION_FLT_DIVIDE_BY_ZERO       = 0xC000008EL,
	EXCEPTION_FLT_INEXACT_RESULT       = 0xC000008FL,
	EXCEPTION_FLT_INVALID_OPERATION    = 0xC0000090L,
	EXCEPTION_FLT_OVERFLOW             = 0xC0000091L,
	EXCEPTION_FLT_STACK_CHECK          = 0xC0000092L,
	EXCEPTION_FLT_UNDERFLOW            = 0xC0000093L,
	EXCEPTION_INT_DIVIDE_BY_ZERO       = 0xC0000094L,
	EXCEPTION_INT_OVERFLOW             = 0xC0000095L,
	EXCEPTION_PRIV_INSTRUCTION         = 0xC0000096L,
	EXCEPTION_IN_PAGE_ERROR            = 0xC0000006L,
	EXCEPTION_ILLEGAL_INSTRUCTION      = 0xC000001DL,
	EXCEPTION_NONCONTINUABLE_EXCEPTION = 0xC0000025L,
	EXCEPTION_STACK_OVERFLOW           = 0xC00000FDL,
	EXCEPTION_INVALID_DISPOSITION      = 0xC0000026L,
	EXCEPTION_GUARD_PAGE               = 0x80000001L,
	EXCEPTION_INVALID_HANDLE           = 0xC0000008L,
};


/**
 * \brief Exception register context
 *
 * Extend this struct as development needs.
 */

struct EXCEPTION_CONTEXT
{
	//
	// Integer registers.
	//

	uint64_t Rax;
	uint64_t Rcx;
	uint64_t Rdx;
	uint64_t Rbx;
	uint64_t Rsp;
	uint64_t Rbp;
	uint64_t Rsi;
	uint64_t Rdi;
	uint64_t R8;
	uint64_t R9;
	uint64_t R10;
	uint64_t R11;
	uint64_t R12;
	uint64_t R13;
	uint64_t R14;
	uint64_t R15;

	//
	// Program counter.
	//

	uint64_t Rip;
};


enum EXCEPTION_ACCESS
{
	EXCEPTION_READ = 0,
	EXCEPTION_WRITE = 1,
};


/**
 * \brief Extra exception information
 *
 * For details, see EXCEPTION_RECORD from MSDN
 */
struct EXCEPTION_META
{
	EXCEPTION_ACCESS Access;
	uint64_t         VirtualAddress;
};


struct EXCEPTION_INFORMATION
{
	EXCEPTION_CODE        Code;
	EXCEPTION_CONTEXT     Context;
	EXCEPTION_META        Meta;
};


/**
 * \brief Exception callback declaration
 *
 * return value is 
 */
typedef EXCEPTION_ACTION (*EXCEPTION_CALLBACK)(EXCEPTION_INFORMATION* Info, void* Context);

struct EXCEPTION_HANDLER
{
	EXCEPTION_CALLBACK Callback;
	void*              Context;
};


bool AddExceptionHandler(const EXCEPTION_HANDLER& Handler);

bool RemoveExceptionHandler(const EXCEPTION_HANDLER& Handler);


	
}  // namespace UtilException