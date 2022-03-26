#pragma once

#include "GPCS4Common.h"
#include <string>

namespace plat
{
	std::string fromws(const wchar_t* ws);

	void tows(const char* mbs, wchar_t* wcs, size_t wcsLen);

	template <size_t N>
	void tows(const char* mbs, wchar_t (&wcs)[N])
	{
		return tows(mbs, wcs, N);
	}

	std::wstring tows(const char* mbs);
}  // namespace plat
