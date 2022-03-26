#pragma once

#include <string>
#include <sstream>
#include <vector>
#include "PlatString.h"

namespace util::str
{
	std::vector<std::string> split(const std::string& strToSplit, char cDelimeter);

	std::string concat(const std::vector<std::string>& ss, const std::string d);

	std::string replaceAll(const std::string& str, const std::string& from, const std::string& to);

	std::string format(const char* pFormat, ...);

	inline void formatex1(std::stringstream&) { }

	template <typename... Tx>
	void formatex1(std::stringstream& str, const wchar_t* arg, const Tx&... args)
	{
		str << fromws(arg);
		formatex1(str, args...);
	}

	template <typename T, typename... Tx>
	void formatex1(std::stringstream& str, const T& arg, const Tx&... args)
	{
		str << arg;
		formatex1(str, args...);
	}

	template <typename... Args>
	std::string formatex(const Args&... args)
	{
		std::stringstream stream;
		formatex1(stream, args...);
		return stream.str();
	}
}  // namespace util::str
