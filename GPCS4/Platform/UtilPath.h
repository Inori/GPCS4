#pragma once

#include "GPCS4Common.h"
#include <string>
#include <tuple>

namespace UtilPath
{;


std::string GetExePath();

std::string PS4PathToPCPath(const std::string& strPs4Path);

size_t FileCountInDirectory(const std::string& path);

bool splitFileName(std::string const &fileName,
				   std::string *name,
				   std::string *extension);

}
