#include "sce_libc.h"
#include "Platform/PlatformUtils.h"

LOG_CHANNEL(SceModules.SceLibc.file);

FILE* PS4API scec_fopen(const char *pathname, const char *mode)
{
	auto pcPath = UtilPath::PS4PathToPCPath(pathname);
	FILE* fp = fopen(pcPath.c_str(), mode);
	LOG_SCE_TRACE("(fname '%s' mode '%s') = %p", pathname, mode, fp);
	return fp;
}

int PS4API scec_fseek(FILE *stream, long offset, int whence)
{
	LOG_SCE_TRACE("fp %p off %d whence %d", stream, offset, whence);
	return fseek(stream, offset, whence);
}


size_t PS4API scec_fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	LOG_SCE_TRACE("fp %p size %zu", stream, size * nmemb);
	return fread(ptr, size, nmemb, stream);
}

int PS4API scec_fgetpos( FILE* stream, fpos_t* pos )
{
	LOG_SCE_TRACE("fp %p pos %p", stream, pos);
	return fgetpos(stream, pos);
}


size_t PS4API scec_fwrite(const void *ptr, size_t size, size_t nmemb, FILE* stream)
{
	LOG_SCE_DUMMY_IMPL();
	return nmemb;
}


long PS4API scec_ftell(FILE *stream)
{
	sce_off_t off = ftell(stream);
	LOG_SCE_TRACE("fp (%p) = %zu", stream, off);
	return off;
}


int PS4API scec_fclose(FILE *stream)
{
	LOG_SCE_TRACE("fp %p", stream);
	int ret = 0;
	do
	{
		if (stream == nullptr)
		{
			break;
		}

		ret = fclose(stream);
	} while (false);
	return ret;
}


int PS4API scec_feof(FILE * stream)
{
	LOG_SCE_TRACE("fp %p", stream);
	return feof(stream);
}


int PS4API scec_fgetc(FILE * stream)
{
	LOG_SCE_TRACE("fp %p", stream);
	return fgetc(stream);
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


int PS4API scec_setvbuf(FILE *stream, char *buf, int mode, size_t size)
{
	LOG_SCE_TRACE("file %p buff %p mode %x size %x", stream, buf, mode, size);
	return setvbuf(stream, buf, mode, size);
}


int PS4API scec_ferror(FILE* stream)
{
	LOG_SCE_TRACE("fp %p", stream);
	return ferror(stream);
}


int PS4API scec_fflush(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


char* PS4API scec_fgets(char * str, int num, FILE * stream)
{
	return fgets(str, num, stream);
}


FILE* PS4API scec_freopen(const char * filename, const char * mode, FILE * stream)
{
	LOG_SCE_TRACE("filename %s, mode %s, fp %p", filename, mode, stream );
	return freopen(filename, mode, stream);
}
