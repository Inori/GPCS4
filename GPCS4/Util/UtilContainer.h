#pragma once

#include <map>
#include <string>
#include <unordered_map>

namespace util
{
template <typename Container>
bool contains(Container const& c, typename Container::const_reference v)
{
	return std::find(c.begin(), c.end(), v) != c.end();
}

template <typename Key, typename Value, typename Cmp, typename Alloc>
bool contains(std::map<Key, Value, Cmp, Alloc> const& m, Key const& k)
{
	return m.find(k) != m.end();
}

template <typename Value, typename Cmp, typename Alloc>
bool contains(std::map<std::string, Value, Cmp, Alloc> const& m, std::string const& k)
{
	return m.find(k) != m.end();
}

template <typename Key, typename Value, typename Cmp, typename Alloc>
bool contains(std::unordered_map<Key, Value, Cmp, Alloc> const& m, Key const& k)
{
	return m.find(k) != m.end();
}

template <typename Value, typename Cmp, typename Alloc>
bool contains(std::unordered_map<std::string, Value, Cmp, Alloc> const& m, std::string const& k)
{
	return m.find(k) != m.end();
}
}  // namespace util
