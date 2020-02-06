#pragma once

#include "GPCS4Common.h"

#include <string>
#include <vector>
#include <memory>

namespace UtilFile
{;

bool LoadFile(const std::string& strFilename, std::vector<uint8_t>& vtData);

std::vector<uint8_t> LoadFile(const std::string& strFilename);

bool StoreFile(const std::string& strFilename, const std::vector<uint8_t>& vtData);

bool StoreFile(const std::string& strFilename, const void* pBuffer, uint32_t nSize);

struct FileCloser
{
	void operator()(FILE *fp) const noexcept
	{
		if (fp != nullptr)
			fclose(fp);
	}
};

typedef std::unique_ptr<FILE, FileCloser> file_uptr;

}
