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
#include <cstdlib>
#include <cstdio>


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

float PS4API scec__FLog(float arg, int n);

float PS4API scec__FSin(float arg, unsigned int m, int n);

int PS4API scec__Getmbcurmax(void);

int PS4API scec__Getpctype(void);

int PS4API scec__Getptolower(void);

int PS4API scec__Getptoupper(void);

int PS4API scec__Log(void);

double PS4API scec__Sin(double arg, unsigned int m, int n);

int PS4API scec__Sinh(void);

int PS4API scec__Stderr(void);

int PS4API scec__Stdout(void);

unsigned long PS4API scec__Stoul(const char* str, char ** endptr, int base);

int PS4API scec__Stoull(void);

int PS4API scec__ZSt14_Xlength_errorPKc(void);

int PS4API scec__ZSt14_Xout_of_rangePKc(void);

int PS4API scec___cxa_atexit(pfunc_cxa_exit_handler func, void * arg, void * dso_handle);

int PS4API scec___cxa_guard_acquire(uint64_t* guard_object);

void PS4API scec___cxa_guard_release(uint64_t* guard_object);

int PS4API scec___cxa_pure_virtual(void);

// TODO: I don't know wether this function belongs to this catalog. 
int PS4API scec___catchReturnFromMain(void);

int PS4API scec___isfinite(void);

int PS4API scec___isfinitef(void);

int PS4API scec___isfinitel(void);

int PS4API scec___isinf(void);

int PS4API scec___isnan(void);

int PS4API scec___signbitl(void);

int PS4API scec_init_env(void* env);

int PS4API scec_abort(void);

int PS4API scec_acos(void);

float PS4API scec_acosf(float x);

int PS4API scec_acosh(void);

int PS4API scec_asin(void);

float PS4API scec_asinf(float x);

int PS4API scec_asinh(void);

int PS4API scec_atan(void);

int PS4API scec_atan2(void);

float PS4API scec_atan2f(float y, float x);

int PS4API scec_atanh(void);

int PS4API scec_atexit(pfunc_exit_handler handler);

double PS4API scec_atof(const char* nptr);

int PS4API scec_bsearch(void);

int PS4API scec_catchReturnFromMain(void);

int PS4API scec_cbrt(void);

sce_div_t PS4API scec_div(int numerator, int denominator);

int PS4API scec_erf(void);

int PS4API scec_erfc(void);

void PS4API scec_exit(int status);

int PS4API scec_exp(void);

double PS4API scec_exp2(double arg);

float PS4API scec_exp2f(float arg);

float PS4API scec_expf(float x);

int PS4API scec_fclose(FILE *stream);

int PS4API scec_feof(void);

int PS4API scec_fgetc(void);

int PS4API scec_fmod(void);

int PS4API scec_fmodf(void);

FILE* PS4API scec_fopen(const char *pathname, const char *mode);

int PS4API scec_fprintf(void);

int PS4API scec_fputc(int c, FILE *stream);

int PS4API scec_fputs(void);

size_t PS4API scec_fread(void *ptr, size_t size, size_t nmemb, FILE *stream);

void PS4API scec_free(void *ptr);

int PS4API scec_frexp(void);

int PS4API scec_fseek(FILE *stream, long offset, int whence);

long PS4API scec_ftell(FILE *stream);

size_t PS4API scec_fwrite(const void *ptr, size_t size, size_t nmemb, FILE* stream);

int PS4API scec_hypot(void);

int PS4API scec_ldexp(void);

int PS4API scec_longjmp(void);

void* PS4API scec_malloc(size_t size);

void* PS4API scec_calloc(size_t nmemb, size_t size);

int PS4API scec_mblen(void);

int PS4API scec_mbsrtowcs(void);

errno_t PS4API scec_mbstowcs_s(size_t * retval, wchar_t * dst, rsize_t dstsz, const char * src, rsize_t len);

int PS4API scec_memchr(void);

int PS4API scec_memcmp(const void *s1, const void *s2, size_t n);

void* PS4API scec_memcpy(void *dest, const void *src, size_t n);

int PS4API scec_memcpy_s(void);

void* PS4API scec_memmove(void * destination, const void * source, size_t num);

void* PS4API scec_memset(void * ptr, int value, size_t num);

int PS4API scec_modf(void);

int PS4API scec_modff(void);

int PS4API scec_pow(void);

float PS4API scec_powf(float x, float y);

int PS4API scec_printf(void);

int PS4API scec_putc(void);

int PS4API scec_putchar(void);

int PS4API scec_puts(void);

void PS4API scec_qsort(void *base, size_t nmemb, size_t size, int(PS4API *compar)(const void *, const void *));

int PS4API scec_rand(void);

void* PS4API scec_realloc(void *ptr, size_t size);

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

int PS4API scec_sprintf(char *str, const char *format, ...);

int PS4API PS4NAKED scec_sprintf_s(char *buffer, size_t sizeOfBuffer, const char *format, ...);

int PS4API scec_sscanf(void);

int PS4API scec_sscanf_s(void);

int PS4API scec_strcasecmp(const char *s1, const char *s2);

char* PS4API scec_strcat(char *dest, const char *src);

errno_t PS4API scec_strcat_s(char *dest, rsize_t destsz, const char *src);

char* PS4API scec_strchr(const char *s, int c);

int PS4API scec_strcmp(const char *s1, const char *s2);

char* PS4API scec_strcpy(char * dst, const char * src);

errno_t PS4API scec_strcpy_s(char *dest, rsize_t dest_size, const char *src);

size_t PS4API scec_strlen(const char* str);

int PS4API scec_strncasecmp(const char *s1, const char *s2, size_t n);

int PS4API scec_strncat(void);

int PS4API scec_strncmp(const char *s1, const char *s2, size_t n);

void* PS4API scec_strncpy(char *dest, const char *src, size_t n);

errno_t PS4API scec_strncpy_s(char* dest, rsize_t destsz, const char* src, rsize_t count);

size_t PS4API scec_strnlen_s(const char *str, size_t strsz);

char* PS4API scec_strrchr(const char *s, int c);

char* PS4API scec_strstr(const char *haystack, const char *needle);

int PS4API scec_strtod(void);

int PS4API scec_strtok(void);

long int PS4API scec_strtol(const char *nptr, char **endptr, int base);

int PS4API scec_swprintf_s(void);

double PS4API scec_tan(double arg);

int PS4API scec_tanf(void);

int PS4API scec_tanh(void);

sce_time_t PS4API scec_time(sce_time_t* pt);

int PS4API scec_vsnprintf(char * s, size_t n, const char * format, void* arg);

int PS4API scec_vsprintf(void);

int PS4API scec_vsprintf_s(char *buffer, size_t numberOfElements, const char *format, void* argptr);

int PS4API scec_wcscat_s(void);

int PS4API scec_wcschr(void);

int PS4API scec_wcscmp(void);

errno_t PS4API scec_wcscpy_s(wchar_t * dest, rsize_t destsz, const wchar_t * src);

size_t PS4API scec_wcslen(const wchar_t *str);

int PS4API scec_wcsncat_s(void);

int PS4API scec_wcsncmp(void);

int PS4API scec_wcsncpy_s(void);

int PS4API scec_wcsnlen_s(void);

int PS4API scec_wcsrtombs(void);

int PS4API scec_wcsstr(void);

int PS4API scec_wcstombs_s(void);

int PS4API scec__Stdin(void);

int PS4API scec__ZSt11_Xbad_allocv(void);

int PS4API scec__ZSt7nothrow(void);

int PS4API scec_ferror(void);

int PS4API scec_fflush(void);

int PS4API scec_fgets(void);

int PS4API scec_freopen(void);

int PS4API scec_getc(void);

int PS4API scec_localeconv(void);

int PS4API scec_localtime(void);

int PS4API scec_mbstowcs(void);

int PS4API scec_memalign(void);

int PS4API scec_reallocalign(void);

int PS4API scec_scalbn(void);

int PS4API scec_scalbnf(void);

int PS4API scec_set_constraint_handler_s(void);

int PS4API scec_setvbuf(FILE *stream, char *buf, int mode, size_t size);

int PS4API scec_snprintf_s(void);

int PS4API scec_srand(void);

int PS4API scec_strcoll(void);

int PS4API scec_strdup(void);

int PS4API scec_strerror(void);

int PS4API scec_strpbrk(void);

int PS4API scec_strspn(void);

int PS4API scec_wcsncpy(void);

int PS4API scec_wcstombs(void);

int PS4API scec_roundf(void);

//////////////////////////////////////////////////////////////////////////
// library: libc_setjmp
//////////////////////////////////////////////////////////////////////////

int PS4API _longjmp(void);


int PS4API _setjmp(void);

