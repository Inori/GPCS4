#pragma once

#include "GPCS4Common.h"
#include <string>
#include <vector>

namespace UtilFile
{;

bool LoadFile(const std::string& strFilename, std::vector<uint8_t>& vtData);

bool StoreFile(const std::string& strFilename, const std::vector<uint8_t>& vtData);

bool StoreFile(const std::string& strFilename, const uint8_t* pBuffer, uint32_t nSize);

}
