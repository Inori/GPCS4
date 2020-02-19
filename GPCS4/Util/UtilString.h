#pragma once

#include <string>
#include <vector>

namespace UtilString
{;

std::vector<std::string> Split(const std::string& strToSplit, char cDelimeter);

std::string Concat(const std::vector<std::string>& ss, const std::string d);

std::string Format(const char* pFormat, ...);

std::string ReplaceAll(const std::string& str, const std::string& from, const std::string& to);
}
