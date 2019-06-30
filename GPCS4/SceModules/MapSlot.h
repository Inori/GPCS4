#pragma once

#include "GPCS4Common.h"
#include <vector>
#include <mutex>
#include <algorithm>

template <typename T, typename EmptyFuncT>
class MapSlot
{
public:
	MapSlot(const uint nSize, EmptyFuncT func):
		m_vtArray(nSize),
		m_funcIsItemEmpty(func)
	{

	}

	MapSlot(EmptyFuncT func):
		m_funcIsItemEmpty(func)
	{

	}

	~MapSlot()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_vtArray.clear();
	}

	void Resize(const uint nSize)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_vtArray.resize(nSize);
	}


	// we start at 1 for convenience,
	// because 0 means SCE_OK or invalid value for some sce functions
	uint GetEmptySlotIndex()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		uint nIdx = 0;
		for (uint i = 1; i != m_vtArray.size(); ++i)
		{
			if (!m_funcIsItemEmpty(m_vtArray[i]))
			{
				continue;
			}
			nIdx = i;
			break;
		}
		return nIdx;
	}

	uint GetItemIndex(const T& item)
	{
		auto equal = [&item](T& ele) { return item == ele; };

		std::lock_guard<std::mutex> lock(m_mutex);
		auto it = std::find_if(m_vtArray.begin(), m_vtArray.end(), equal);
		return it != m_vtArray.end() ? std::distance(m_vtArray.begin(), it) : 0;
	}

	template<typename EqualFuncT>
	uint GetItemIndex(const T& item, EqualFuncT equal)
	{
		auto pred = [&item, &equal](T& ele) { return equal(item, ele); };
		std::lock_guard<std::mutex> lock(m_mutex);
		auto it = std::find_if(m_vtArray.begin(), m_vtArray.end(), pred);
		return it != m_vtArray.end() ? std::distance(m_vtArray.begin(), it) : 0;
	}

	T& GetItemAt(const uint i)
	{
		return (*this)[i];
	}

	void SetItemAt(const uint i, T& item)
	{
		(*this)[i] = item;
	}

	T& operator[](const uint i)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

#ifdef GPCS4_DEBUG
		if (i >= m_vtArray.size())
		{
			LOG_ASSERT("index exceed max item size %d/%d", i, m_vtArray.size());
		}
#endif  //GPCS4_DEBUG

		return m_vtArray[i];
	}

private:
	std::mutex m_mutex;
	std::vector<T> m_vtArray;
	std::function<EmptyFuncT> m_funcIsItemEmpty;
};

