#pragma once

namespace vlt
{;

struct VltEqual
{
	template<typename T>
	size_t operator () (const T& a, const T& b) const 
	{
		return a == b;
	}
};

struct VltHash
{
	template<typename T>
	size_t operator () (const T& object) const 
	{
		return object.hash();
	}
};


class VltHashState 
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

}  // namespace vlt