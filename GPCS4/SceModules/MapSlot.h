#pragma once

#include "GPCS4Common.h"
#include <vector>
#include <mutex>
#include <algorithm>


template <typename T> 
bool MapSlotDefaultEmpty(const T& item)
{
	return (item == 0);
}

template <typename T>
bool MapSlotDefaultEqual(const T& lhs, const T& rhs)
{
	return (lhs == rhs);
}

template <typename T, 
	bool (*EmptylFunc)(const T&) = MapSlotDefaultEmpty<T>,
	bool (*EqualFunc)(const T&, const T&) = MapSlotDefaultEqual>
class MapSlot
{
public:

	MapSlot()
	{
	}

	MapSlot(const uint32_t nSize):
		m_vtArray(nSize)
	{
	}

	~MapSlot()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_vtArray.clear();
	}

	uint32_t Size()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_vtArray.size();
	}

	void Resize(const uint32_t nSize)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_vtArray.resize(nSize);
	}


	// we start at 1 for convenience,
	// because 0 means SCE_OK or invalid value for some sce functions
	uint32_t GetEmptySlotIndex()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		uint32_t nIdx = 0;
		for (uint32_t i = 1; i != m_vtArray.size(); ++i)
		{
			if (!EmptylFunc(m_vtArray[i]))
			{
				continue;
			}
			nIdx = i;
			break;
		}
		return nIdx;
	}

	uint32_t GetItemIndex(const T& item)
	{
		auto pred = [&item](T& ele) { return EqualFunc(item, ele); };
		std::lock_guard<std::mutex> lock(m_mutex);
		auto it = std::find_if(m_vtArray.begin(), m_vtArray.end(), pred);
		return it != m_vtArray.end() ? std::distance(m_vtArray.begin(), it) : 0;
	}

	T& GetItemAt(const uint32_t i)
	{
		return (*this)[i];
	}

	void SetItemAt(const uint32_t i, T& item)
	{
		(*this)[i] = item;
	}

	void SetItemAt(const uint32_t i, T&& item)
	{
		(*this)[i] = std::move(item);
	}

	T& operator[](const uint32_t i)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		LOG_ASSERT((i < m_vtArray.size()), "index exceed max item size %d/%d", i, m_vtArray.size());
		return m_vtArray[i];
	}

private:
	std::mutex m_mutex;
	std::vector<T> m_vtArray;
};

