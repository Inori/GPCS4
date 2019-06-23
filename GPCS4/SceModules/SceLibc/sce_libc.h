/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libc
 *        library: libc
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceLibc;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libc
//////////////////////////////////////////////////////////////////////////

typedef void(*pfunc_cxa_exit_handler)(void*);
typedef void(*pfunc_exit_handler)(void);


//////////////////////////////////////////////////////////////////////////

int PS4API scec_Need_sceLibc(void);

int PS4API scec__Cosh(void);

int PS4API scec__FDtest(void);

int PS4API scec__FLog(void);

int PS4API scec__FSin(void);

int PS4API scec__Getmbcurmax(void);

int PS4API scec__Getpctype(void);

int PS4API scec__Getptolower(void);

int PS4API scec__Getptoupper(void);

int PS4API scec__Log(void);

int PS4API scec__Sin(void);

int PS4API scec__Sinh(void);

int PS4API scec__Stderr(void);

int PS4API scec__Stdout(void);

int PS4API scec__Stoul(void);

int PS4API scec__Stoull(void);

int PS4API scec__ZSt14_Xlength_errorPKc(void);

int PS4API scec__ZSt14_Xout_of_rangePKc(void);

int PS4API scec___cxa_atexit(pfunc_cxa_exit_handler func, void * arg, void * dso_handle);

int PS4API scec___cxa_guard_acquire(uint64_t* guard_object);

void PS4API scec___cxa_guard_release(uint64_t* guard_object);

int PS4API scec___cxa_pure_virtual(void);

int PS4API scec___isfinite(void);

int PS4API scec___isfinitef(void);

int PS4API scec___isfinitel(void);

int PS4API scec___isinf(void);

int PS4API scec___isnan(void);

int PS4API scec___signbitl(void);

int PS4API scec_init_env(void* env);

int PS4API scec_abort(void);

int PS4API scec_acos(void);

int PS4API scec_acosf(void);

int PS4API scec_acosh(void);

int PS4API scec_asin(void);

int PS4API scec_asinf(void);

int PS4API scec_asinh(void);

int PS4API scec_atan(void);

int PS4API scec_atan2(void);

int PS4API scec_atan2f(void);

int PS4API scec_atanh(void);

int PS4API scec_atexit(pfunc_exit_handler handler);

int PS4API scec_atof(void);

int PS4API scec_bsearch(void);

int PS4API scec_catchReturnFromMain(void);

int PS4API scec_cbrt(void);

int PS4API scec_div(void);

int PS4API scec_erf(void);

int PS4API scec_erfc(void);

int PS4API scec_exit(void);

int PS4API scec_exp(void);

int PS4API scec_exp2(void);

int PS4API scec_exp2f(void);

int PS4API scec_expf(void);

int PS4API scec_fclose(void);

int PS4API scec_feof(void);

int PS4API scec_fgetc(void);

int PS4API scec_fmod(void);

int PS4API scec_fmodf(void);

int PS4API scec_fopen(void);

int PS4API scec_fprintf(void);

int PS4API scec_fputc(void);

int PS4API scec_fputs(void);

int PS4API scec_fread(void);

int PS4API scec_free(void);

int PS4API scec_frexp(void);

int PS4API scec_fseek(void);

int PS4API scec_ftell(void);

int PS4API scec_fwrite(void);

int PS4API scec_hypot(void);

int PS4API scec_ldexp(void);

int PS4API scec_longjmp(void);

int PS4API scec_malloc(void);

int PS4API scec_mblen(void);

int PS4API scec_mbsrtowcs(void);

int PS4API scec_mbstowcs_s(void);

int PS4API scec_memchr(void);

int PS4API scec_memcmp(void);

int PS4API scec_memcpy(void);

int PS4API scec_memcpy_s(void);

int PS4API scec_memmove(void);

void* PS4API scec_memset(void * ptr, int value, size_t num);

int PS4API scec_modf(void);

int PS4API scec_modff(void);

int PS4API scec_pow(void);

int PS4API scec_powf(void);

int PS4API scec_printf(void);

int PS4API scec_putc(void);

int PS4API scec_putchar(void);

int PS4API scec_puts(void);

int PS4API scec_qsort(void);

int PS4API scec_rand(void);

int PS4API scec_realloc(void);

int PS4API scec_sceLibcMspaceCalloc(void);

int PS4API scec_sceLibcMspaceCreate(void);

int PS4API scec_sceLibcMspaceDestroy(void);

int PS4API scec_sceLibcMspaceFree(void);

int PS4API scec_sceLibcMspaceMalloc(void);

int PS4API scec_sceLibcMspaceMallocStats(void);

int PS4API scec_sceLibcMspaceMallocStatsFast(void);

int PS4API scec_sceLibcMspaceMallocUsableSize(void);

int PS4API scec_sceLibcMspaceMemalign(void);

int PS4API scec_sceLibcMspacePosixMemalign(void);

int PS4API scec_sceLibcMspaceRealloc(void);

int PS4API scec_sceLibcMspaceReallocalign(void);

int PS4API scec_setjmp(void);

int PS4API scec_snprintf(void);

int PS4API scec_sprintf(void);

int PS4API PS4NAKED scec_sprintf_s(char *buffer, size_t sizeOfBuffer, const char *format, ...);

int PS4API scec_sscanf(void);

int PS4API scec_sscanf_s(void);

int PS4API scec_strcasecmp(void);

int PS4API scec_strcat(void);

int PS4API scec_strcat_s(void);

int PS4API scec_strchr(void);

int PS4API scec_strcmp(void);

int PS4API scec_strcpy(void);

errno_t PS4API scec_strcpy_s(char *dest, rsize_t dest_size, const char *src);

size_t PS4API scec_strlen(const char* str);

int PS4API scec_strncasecmp(void);

int PS4API scec_strncat(void);

int PS4API scec_strncmp(void);

int PS4API scec_strncpy(void);

int PS4API scec_strncpy_s(void);

int PS4API scec_strnlen_s(void);

int PS4API scec_strrchr(void);

int PS4API scec_strstr(void);

int PS4API scec_strtod(void);

int PS4API scec_strtok(void);

int PS4API scec_strtol(void);

int PS4API scec_swprintf_s(void);

int PS4API scec_tan(void);

int PS4API scec_tanf(void);

int PS4API scec_tanh(void);

int PS4API scec_time(void);

int PS4API scec_vsnprintf(char * s, size_t n, const char * format, va_list arg);

int PS4API scec_vsprintf(void);

int PS4API scec_vsprintf_s(void);

int PS4API scec_wcscat_s(void);

int PS4API scec_wcschr(void);

int PS4API scec_wcscmp(void);

int PS4API scec_wcscpy_s(void);

int PS4API scec_wcslen(void);

int PS4API scec_wcsncat_s(void);

int PS4API scec_wcsncmp(void);

int PS4API scec_wcsncpy_s(void);

int PS4API scec_wcsnlen_s(void);

int PS4API scec_wcsrtombs(void);

int PS4API scec_wcsstr(void);

int PS4API scec_wcstombs_s(void);





