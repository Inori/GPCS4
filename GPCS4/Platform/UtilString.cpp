#include "UtilString.h"
#include <cstdarg>
#include <sstream>

namespace UtilString
{;


std::vector<std::string> Split(const std::string& strToSplit, char cDelimeter)
{
	std::vector<std::string> result;
	do 
	{
		if (strToSplit.empty())
		{
			break;
		}

		std::stringstream ss(strToSplit);
		std::string item;
		while (std::getline(ss, item, cDelimeter))
		{
			result.push_back(item);
		}

		if (item.empty())
		{
			result.push_back(item);
		}

	} while (false);
	return result;
}


std::string Concat(const std::vector<std::string>& ss, const std::string d)
{
	std::string ret;
	for (size_t i = 0; i < ss.size(); ++i)
	{
		if (i > 0)
		{
			ret += d;
		}
		ret += ss[i];
	}
	return ret;
}

std::string Format(const char* pFormat, ...)
{
	std::string strDst;
	static char szTemp[0x1000] = { 0 };
	do
	{
		if (!pFormat)
		{
			break;
		}

		va_list pVarList;
		va_start(pVarList, pFormat);

		if (vsprintf(szTemp, pFormat, pVarList) < 0)
		{
			break;
		}

		strDst.assign(szTemp);

		va_end(pVarList);

	} while (false);

	return strDst;
}

std::string ReplaceAll(const std::string& cstr, const std::string& from, const std::string& to) 
{
	std::string str = cstr;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}

}