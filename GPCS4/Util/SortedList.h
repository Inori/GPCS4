#pragma once

#include <functional>
#include <list>
#include <map>

template <typename T>
using CompareFunc = std::function<bool(const T& lhs, const T& rhs) const>;

template <typename T>
class SortedList
{
	using iterator        = std::list<T>::iterator;
	using const_iterator  = std::list<T>::const_iterator;
	using size_type       = std::list<T>::size_type;
	using value_type      = std::list<T>::value_type;
	using reference       = value_type&;
	using const_reference = const value_type&;

public:
	SortedList() :
		m_compare(std::less<T>()),
		m_list()
	{
	}

	SortedList(const CompareFunc& compare) :
		m_compare(compare),
		m_list()
	{
	}

	~SortedList()
	{

	}

	iterator begin() noexcept
	{
		return m_list.begin();
	}

	const_iterator begin() const noexcept
	{
		return m_list.begin();
	}

	iterator end() noexcept
	{
		return m_list.end();
	}

	const_iterator end() const noexcept
	{
		return m_list.end();
	}

	reference front()
	{
		return m_list.front();
	}

	const_reference front() const
	{
		return m_list.front();
	}

	reference back()
	{
		return m_list.back();
	}

	const_reference back() const
	{
		return m_list.back();
	}

	template<typename ValTy, typename PrTy>
	iterator find(const ValTy& val, PrTy match)
	{

	}

	template <typename ValTy, typename PrTy>
	const_iterator find(const ValTy& val, PrTy match) const
	{
	}


	void push(T&& val)
	{

	}

	void push(const T& val)
	{

	}

	void remove(const T& val)
	{
		m_list.remove(val);
	}

	iterator erase(const_iterator where)
	{
		return m_list.erase(where);
	}

	void clear() noexcept
	{
		m_list.clear();
	}

	size_type size() const noexcept
	{
		return m_list.size();
	}

	bool empty() const noexcept
	{
		return m_list.empty();
	}


private:
	CompareFunc<T> m_compare;
	std::list<T>   m_list;
};