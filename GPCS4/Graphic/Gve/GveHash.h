#pragma once

namespace gve
{;

struct GveEqual
{
	template<typename T>
	size_t operator () (const T& a, const T& b) const 
	{
		return a == b;
	}
};

struct GveHash
{
	template<typename T>
	size_t operator () (const T& object) const 
	{
		return object.hash();
	}
};


class GveHashState 
{

public:

	void add(size_t hash) 
	{
		m_value ^= hash + 0x9e3779b9
			+ (m_value << 6)
			+ (m_value >> 2);
	}

	operator size_t () const 
	{
		return m_value;
	}

private:

	size_t m_value = 0;
};

}  // namespace gve