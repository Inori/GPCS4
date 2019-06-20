#pragma once

#include <string>
#include <vector>

namespace UtilString
{;

bool Split(const std::string& strToSplit, char cDelimeter, std::vector<std::string>& vtStrValue);

std::string Format(const char* pFormat, ...);

}
