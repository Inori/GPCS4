#pragma once

#include <list>
#include <unordered_map>

namespace util
{
	/**
	 * \brief Insert Ordered Set
	 * 
	 * like std::set, except that begin() -> end() iterates in the
	 * order that elements were added to the set (not in the order
	 * of operator<(T, T))
	 * 
	 */
	
	template <typename T>
	class insert_ordered_set
	{
	public:
		typedef typename std::list<T>::iterator iterator;

		insert_ordered_set() = default;
		insert_ordered_set(const insert_ordered_set& other)
		{
			*this = other;
		}
		insert_ordered_set& operator=(const insert_ordered_set& other)
		{
			clear();
			for (auto i : other.m_list)
			{
				insert(i);  // inserting manually creates proper iterators
			}
			return *this;
		}

		iterator begin()
		{
			return m_list.begin();
		}
		iterator end()
		{
			return m_list.end();
		}

		void erase(const T& val)
		{
			auto it = m_map.find(val);
			if (it != m_map.end())
			{
				m_list.erase(it->second);
				m_map.erase(it);
			}
		}

		void erase(iterator position)
		{
			m_map.erase(*position);
			m_list.erase(position);
		}

		// cheating a bit, not returning the iterator
		void insert(const T& val)
		{
			auto it = m_map.find(val);
			if (it == m_map.end())
			{
				m_list.push_back(val);
				m_map.insert(std::make_pair(val, --m_list.end()));
			}
		}

		size_t size() const
		{
			return m_map.size();
		}
		bool empty() const
		{
			return m_map.empty();
		}

		void clear()
		{
			m_map.clear();
			m_list.clear();
		}

		size_t count(const T& val) const
		{
			return m_map.count(val);
		}

	private:
		std::unordered_map<T, typename std::list<T>::iterator> m_map;
		std::list<T>                                           m_list;
	};


	/**
	 * \brief Insert Ordered Map
	 *
	 * like std::map, except that begin() -> end() iterates in the
	 * order that elements were added to the map (not in the order
	 * of operator<(Key, Key))
	 *
	 */
	template <typename Key, typename T>
	class insert_ordered_map
	{
	public:
		typedef typename std::list<std::pair<const Key, T>>::iterator       iterator;
		typedef typename std::list<std::pair<const Key, T>>::const_iterator const_iterator;

		insert_ordered_map() = default;
		insert_ordered_map(const insert_ordered_map& other)
		{
			for (auto kv : other)
			{
				insert(kv);
			}
		}

		insert_ordered_map& operator=(const insert_ordered_map& other)
		{
			if (this != &other)
			{
				this->~insert_ordered_map();
				new (this) insert_ordered_map<Key, T>(other);
			}
			return *this;
		}

		bool operator==(const insert_ordered_map& other)
		{
			return m_map == other.m_map && m_list == other.m_list;
		}
		bool operator!=(const insert_ordered_map& other)
		{
			return !(*this == other);
		}

		iterator begin()
		{
			return m_list.begin();
		}
		iterator end()
		{
			return m_list.end();
		}

		const_iterator begin() const
		{
			return m_list.begin();
		}
		const_iterator end() const
		{
			return m_list.end();
		}

		std::pair<iterator, bool> insert(const std::pair<const Key, T>& kv)
		{
			// Try inserting with a placeholder list iterator.
			auto inserted = m_map.insert({ kv.first, m_list.end() });
			if (inserted.second)
			{
				// This is a new item; insert it in the list and update the iterator.
				m_list.push_back(kv);
				inserted.first->second = std::prev(m_list.end());
			}
			return { inserted.first->second, inserted.second };
		}

		T& operator[](const Key& k)
		{
			std::pair<const Key, T> kv = { k, {} };
			return insert(kv).first->second;
		}

		T& at(const Key& k)
		{
			return m_map.at(k)->second;
		}

		iterator find(const Key& k)
		{
			auto it = m_map.find(k);
			if (it == m_map.end())
			{
				return end();
			}
			return it->second;
		}

		void erase(const Key& k)
		{
			auto it = m_map.find(k);
			if (it != m_map.end())
			{
				m_list.erase(it->second);
				m_map.erase(it);
			}
		}

		void erase(iterator position)
		{
			erase(position->first);
		}

		void clear()
		{
			m_map.clear();
			m_list.clear();
		}

		void swap(insert_ordered_map<Key, T>& other)
		{
			m_map.swap(other.m_map);
			m_list.swap(other.m_list);
		}

		size_t size() const
		{
			return m_map.size();
		}
		bool empty() const
		{
			return m_map.empty();
		}
		size_t count(const Key& k) const
		{
			return m_map.count(k);
		}

	private:
		std::unordered_map<
			Key, 
			typename std::list<std::pair<const Key, T>>::iterator>
										   m_map;
		std::list<std::pair<const Key, T>> m_list;
	};
}  // namespace util
