#include "UtilPath.h"
#include "UtilString.h"
#include <algorithm>


namespace UtilPath
{;


#ifdef GPCS4_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN


std::string GetExePath()
{
	std::string strExePath;
	do 
	{
		HMODULE hModule = GetModuleHandleW(NULL);
		if (!hModule)
		{
			break;
		}
		CHAR szPath[MAX_PATH];
		if (!GetModuleFileNameA(hModule, szPath, MAX_PATH))
		{
			break;
		}
		strExePath.assign(szPath);
		strExePath = strExePath.substr(0, strExePath.find_last_of('\\') + 1);

	} while (false);
	return strExePath;
}

std::string PS4PathToPCPath(const std::string& strPs4Path)
{
	std::string strPcPath = strPs4Path;
	std::replace(strPcPath.begin(), strPcPath.end(), '/', '\\');
	std::string strExePath = GetExePath();
	strPcPath = UtilString::ReplaceAll(strPcPath, "\\app0\\", strExePath);
	return strPcPath;
}

#else


#endif  //GPCS4_WINDOWS

}