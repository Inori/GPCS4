#pragma once

#include "GPCS4Common.h"
#include <string>

namespace pssl
{;

// shader bytecode reader
class PsslSbReader
{
public:
	PsslSbReader(const uint8_t* data, size_t size);
	~PsslSbReader();

	auto readu8() { return this->readNum<uint8_t>(); }
	auto readu16() { return this->readNum<uint16_t>(); }
	auto readu32() { return this->readNum<uint32_t>(); }
	auto readu64() { return this->readNum<uint64_t>(); }

	auto readi8() { return this->readNum<int8_t>(); }
	auto readi16() { return this->readNum<int16_t>(); }
	auto readi32() { return this->readNum<int32_t>(); }
	auto readi64() { return this->readNum<int64_t>(); }

	auto readf32() { return this->readNum<float>(); }
	auto readf64() { return this->readNum<double>(); }

	template<typename T>
	auto readEnum() {
		using Tx = std::underlying_type_t<T>;
		return static_cast<T>(this->readNum<Tx>());
	}

	std::string readString();

	void read(void* dst, size_t n);

	void skip(size_t n);

	PsslSbReader clone(size_t pos) const;

	PsslSbReader resize(size_t size) const;

	bool eof() const {
		return m_pos >= m_size;
	}

	void store(std::ostream&& stream) const;

private:

	PsslSbReader(const uint8_t* data, size_t size, size_t pos)
		: m_data(data), m_size(size), m_pos(pos) { }

	template<typename T>
	T readNum() {
		T result;
		this->read(&result, sizeof(result));
		return result;
	}

private:
	const uint8_t* m_data = nullptr;
	size_t      m_size = 0;
	size_t      m_pos = 0;

};

}  // pssl