#pragma once

#include <string>
#include <vector>

namespace str
{;

std::vector<std::string> split(const std::string& strToSplit, char cDelimeter);

std::string concat(const std::vector<std::string>& ss, const std::string d);

std::string format(const char* pFormat, ...);

std::string replaceAll(const std::string& str, const std::string& from, const std::string& to);
}
