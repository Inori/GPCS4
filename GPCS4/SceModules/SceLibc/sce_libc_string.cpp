#include "sce_libc.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cstdarg>


int PS4API scec_mblen(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_mbsrtowcs(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_mbstowcs_s(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_memchr(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_memcmp(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


void* PS4API scec_memcpy(void *dest, const void *src, size_t n)
{
	LOG_SCE_TRACE("dst %p src %p sizt %x", dest, src, n);
	return memcpy(dest, src, n);
}


int PS4API scec_memcpy_s(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


void* PS4API scec_memmove(void * destination, const void * source, size_t num)
{
	LOG_SCE_TRACE("dst %p src %p num %x", destination, source, num);
	return memmove(destination, source, num);
}


void* PS4API scec_memset(void * ptr, int value, size_t num)
{
	LOG_SCE_TRACE("ptr %p val %x num %x", ptr, value, num);
	return memset(ptr, value, num);
}


int PS4API scec_snprintf(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


#ifdef GPCS4_WINDOWS
extern "C" int scec_sprintf_asm();
#endif // GPCS4_WINDOWS

int
PS4API
#ifdef GPCS4_WINDOWS
PS4NAKED
#endif // GPCS4_WINDOWS
PS4API scec_sprintf(char *str, const char *format, ...)
{
	//LOG_SCE_TRACE("buffer %p %x %p", buffer, sizeOfBuffer, format);

#ifdef GPCS4_WINDOWS

	asm volatile("jmp scec_sprintf_asm");

#elif defined(GPCS4_LINUX)
	// on linux, this can be implemented more friendly.
	va_list arg_list;

	va_start(arg_list, format);
	int ret = vsprintf(buffer, format, arg_list);
	va_end(arg_list);
	return ret;
#endif
}


#ifdef GPCS4_WINDOWS
extern "C" int scec_sprintf_s_asm();
#endif // GPCS4_WINDOWS

int 
PS4API
#ifdef GPCS4_WINDOWS
PS4NAKED
#endif // GPCS4_WINDOWS
scec_sprintf_s(char *buffer, size_t sizeOfBuffer, const char *format, ...)
{
	//LOG_SCE_TRACE("buffer %p %x %p", buffer, sizeOfBuffer, format);

#ifdef GPCS4_WINDOWS
	// why not just write inline assembly directly here?
	// because I really hate the stupid AT&T syntax
	// after 3 hours' learn, I still get compiling errors and errors and erros..
	// so I just give up.
	// fuck that.
	asm volatile("jmp scec_sprintf_s_asm");

#elif defined(GPCS4_LINUX)
	// on linux, this can be implemented more friendly.
	va_list arg_list;

	va_start(arg_list, format);
	int ret = vsprintf_s(buffer, sizeOfBuffer, format, arg_list);
	va_end(arg_list);
	return ret;
#endif

}


int PS4API scec_snprintf_s(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_sscanf(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_sscanf_s(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_strcasecmp(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_strcat(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


errno_t PS4API scec_strcat_s(char *dest, rsize_t destsz, const char *src)
{
	LOG_SCE_TRACE("dst %p dstsize %x src %p", dest, destsz, src);
	return strcat_s(dest, destsz, src);
}


int PS4API scec_strchr(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_strcmp(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_strcpy(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


errno_t PS4API scec_strcpy_s(char *dest, rsize_t dest_size, const char *src)
{
	LOG_SCE_TRACE("dst %p dst_sz %x src %p", dest, dest_size, src);
	return strcpy_s(dest, dest_size, src);
}


size_t PS4API scec_strlen(const char* str)
{
	LOG_SCE_TRACE("str %p", str);
	return strlen(str);
}


int PS4API scec_strncasecmp(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_strncat(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_strncmp(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


void* PS4API scec_strncpy(char *dest, const char *src, size_t n)
{
	LOG_SCE_TRACE("dst %p src %p size %x", dest, src, n);
	return strncpy(dest, src, n);
}


int PS4API scec_strncpy_s(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_strnlen_s(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_strrchr(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_strstr(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_strtod(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_strtok(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_strtol(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_swprintf_s(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


#ifdef GPCS4_WINDOWS
extern "C" int scec_vsnprintf_asm();
#endif  //GPCS4_WINDOWS

int PS4API 
#ifdef GPCS4_WINDOWS
PS4NAKED
#endif  //GPCS4_WINDOWS
scec_vsnprintf(char * s, size_t n, const char * format, void* arg)
{
	//LOG_SCE_TRACE("s %p n %x format %p", s, n, format);
#ifdef GPCS4_WINDOWS
	asm volatile ("jmp  scec_vsnprintf_asm");
#else
	return SCE_OK;
#endif  //GPCS4_WINDOWS
}


int PS4API scec_vsprintf(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_vsprintf_s(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_wcscat_s(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_wcschr(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_wcscmp(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_wcscpy_s(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_wcslen(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_wcsncat_s(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_wcsncmp(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_wcsncpy_s(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_wcsnlen_s(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_wcsrtombs(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_wcsstr(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_wcstombs_s(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_printf(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_putc(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_putchar(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_puts(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


