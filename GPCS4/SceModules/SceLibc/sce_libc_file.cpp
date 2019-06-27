#include "sce_libc.h"


int PS4API scec_fseek(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_ftell(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


size_t PS4API scec_fwrite(const void *ptr, size_t size, size_t nmemb, FILE* stream)
{
	LOG_SCE_DUMMY_IMPL();
	return nmemb;
}


int PS4API scec_fclose(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_feof(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_fgetc(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_fopen(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_fprintf(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_fputc(int c, FILE *stream)
{
	//LOG_SCE_TRACE("c %x stream %p", c, stream);
	//return fputc(c, stream);
	LOG_SCE_DUMMY_IMPL();
	return (unsigned char)c;
}


int PS4API scec_fputs(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_fread(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_setvbuf(FILE *stream, char *buf, int mode, size_t size)
{
	//LOG_SCE_TRACE("file %p buff %p mode %x size %x", stream, buf, mode, size);
	//return setvbuf(stream, buf, mode, size);
	LOG_SCE_DUMMY_IMPL();
	return 0;
}


int PS4API scec_ferror(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_fflush(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_fgets(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_freopen(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}
