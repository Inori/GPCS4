#pragma once

#include <string>
#include <vector>

namespace UtilString
{;

bool Split(const std::string& strToSplit, char cDelimeter, std::vector<std::string>& vtStrValue);

std::string Format(const char* pFormat, ...);

std::string ReplaceAll(const std::string& str, const std::string& from, const std::string& to);
}
