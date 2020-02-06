#include "sce_fios2.h"

#include "Platform/UtilPath.h"

#include <filesystem>

// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs

LOG_CHANNEL(SceModules.SceFios2);

//////////////////////////////////////////////////////////////////////////
// library: libSceFios2
//////////////////////////////////////////////////////////////////////////

int PS4API sceFiosDateToComponents(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceFiosDeleteSync(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceFiosDirectoryCreateSync(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


bool PS4API sceFiosFileExistsSync(const SceFiosOpAttr* pAttr, const char* pPath)
{
	LOG_SCE_TRACE("path %s", pPath);

	LOG_ASSERT(pAttr == nullptr, "only support null attr.");
	auto path = UtilPath::PS4PathToPCPath(pPath);
	bool isDir = std::filesystem::is_directory(path);
	bool exists = std::filesystem::exists(path);
	return !isDir && exists;
}


bool PS4API sceFiosDirectoryExistsSync(const SceFiosOpAttr *pAttr, const char *pPath)
{
	LOG_SCE_TRACE("path %s", pPath);

	LOG_ASSERT(pAttr == nullptr, "only support null attr.");
	auto path   = UtilPath::PS4PathToPCPath(pPath);
	bool isDir  = std::filesystem::is_directory(path);
	bool exists = std::filesystem::exists(path);
	return isDir && exists;
}


int PS4API sceFiosFHCloseSync(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceFiosFHOpenSync(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceFiosFHReadSync(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceFiosFHSeek(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceFiosFHStatSync(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceFiosFHWriteSync(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceFiosFileGetSizeSync(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceFiosFileReadSync(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceFiosFileWriteSync(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceFiosStatSync(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceFiosDeallocatePassthruFH(void) 
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceFiosFilenoToFH(void) 
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceFiosFHToFileno(void) 
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceFiosIsValidHandle(void) 
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceFiosFHOpenWithModeSync(void) 
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceFiosRenameSync(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}
