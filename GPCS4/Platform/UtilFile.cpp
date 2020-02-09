#include "UtilFile.h"
#include <fstream>

namespace UtilFile
{;

bool LoadFile(const std::string& strFilename, std::vector<uint8_t>& vtData)
{
	bool bRet = false;
	do
	{
		if (strFilename.empty())
		{
			break;
		}

		std::ifstream fin(strFilename, std::ofstream::in | std::ofstream::ate | std::ofstream::binary);
		if (!fin.is_open())
		{
			break;
		}

		size_t nFileSize = (size_t)fin.tellg();
		vtData.resize(nFileSize);

		fin.seekg(0);
		fin.read((char*)vtData.data(), nFileSize);

		fin.close();

		bRet = true;
	} while (false);
	return bRet;
}

std::vector<uint8_t> LoadFile(const std::string& strFilename)
{
	std::vector<uint8_t> vtData;
	LoadFile(strFilename, vtData);
	return vtData;
}

bool StoreFile(const std::string& strFilename, const void* pBuffer, uint32_t nSize)
{
	bool bRet = false;
	do
	{
		if (strFilename.empty() || !pBuffer || !nSize)
		{
			break;
		}

		std::ofstream fout(strFilename, std::ofstream::out | std::ofstream::binary);
		if (!fout.is_open())
		{
			break;
		}

		fout.write((char*)pBuffer, nSize);
		fout.close();

		bRet  = true;
	}while(false);
	return bRet;
}

bool StoreFile(const std::string& strFilename, const std::vector<uint8_t>& vtData)
{
	return StoreFile(strFilename, vtData.data(), vtData.size());
}


#ifdef GPCS4_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN




#else


#endif  //GPCS4_WINDOWS

}
