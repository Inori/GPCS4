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


int PS4API scec_memcpy(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_memcpy_s(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_memmove(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
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


int PS4API scec_sprintf(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_sprintf_s(char *buffer, size_t sizeOfBuffer, const char *format, ...)
{
	LOG_SCE_TRACE("buffer %p %x %p", buffer, sizeOfBuffer, format);

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


int PS4API scec_strcat_s(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
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


int PS4API scec_strcpy_s(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_strlen(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
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


int PS4API scec_strncpy(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
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


int PS4API scec_vsnprintf(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
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


