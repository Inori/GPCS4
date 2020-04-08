#pragma once

#include "GPCS4Common.h"
#include "GCNInstruction.h"
#include "GCNParser/GCNParser.h"


namespace pssl
{;


/**
 * \brief Component swizzle
 *
 * Maps vector components to
 * other vector components.
 */
class GcnRegSwizzle 
{

public:

	GcnRegSwizzle() { }
	GcnRegSwizzle(uint32_t x, uint32_t y, uint32_t z, uint32_t w)
		: m_mask((x << 0) | (y << 2) | (z << 4) | (w << 6)) { }

	uint32_t operator [] (uint32_t id) const 
	{
		return (m_mask >> (id + id)) & 0x3;
	}

	bool operator == (const GcnRegSwizzle& other) const { return m_mask == other.m_mask; }
	bool operator != (const GcnRegSwizzle& other) const { return m_mask != other.m_mask; }

private:

	uint8_t m_mask = 0;

};


/**
 * \brief Component mask
 *
 * Enables access to certain
 * subset of vector components.
 */
class GcnRegMask 
{

public:

	GcnRegMask() { }
	GcnRegMask(uint32_t mask) : m_mask(mask) { }
	GcnRegMask(bool x, bool y, bool z, bool w)
		: m_mask((x ? 0x1 : 0) | (y ? 0x2 : 0)
			| (z ? 0x4 : 0) | (w ? 0x8 : 0)) { }

	bool operator [] (uint32_t id) const 
	{
		return (m_mask >> id) & 1;
	}

	uint32_t popCount() const 
	{
		const uint8_t n[16] = { 0, 1, 1, 2, 1, 2, 2, 3,
								1, 2, 2, 3, 2, 3, 3, 4 };
		return n[m_mask & 0xF];
	}

	uint32_t firstComponent() const
	{
		const uint8_t n[16] = { 4, 0, 1, 0, 2, 0, 1, 0,
								3, 0, 1, 0, 2, 0, 1, 0 };
		return n[m_mask & 0xF];
	}

	uint32_t minComponents() const 
	{
		const uint8_t n[16] = { 0, 1, 2, 2, 3, 3, 3, 3,
								4, 4, 4, 4, 4, 4, 4, 4 };
		return n[m_mask & 0xF];
	}

	bool operator == (const GcnRegMask& other) const { return m_mask == other.m_mask; }
	bool operator != (const GcnRegMask& other) const { return m_mask != other.m_mask; }

	GcnRegMask& operator |= (const GcnRegMask& other) 
	{
		m_mask |= other.m_mask;
		return *this;
	}

	static GcnRegMask firstN(uint32_t n) 
	{
		return GcnRegMask(n >= 1, n >= 2, n >= 3, n >= 4);
	}

	static GcnRegMask select(uint32_t n) 
	{
		return GcnRegMask(n == 0, n == 1, n == 2, n == 3);
	}

	std::string maskString() const {
		std::string out = "";
		out += (m_mask & 0x1) ? "x" : "";
		out += (m_mask & 0x2) ? "y" : "";
		out += (m_mask & 0x4) ? "z" : "";
		out += (m_mask & 0x8) ? "w" : "";
		return out;
	}

private:

	uint8_t m_mask = 0;

};

class GCNCodeSlice 
{

public:

	GCNCodeSlice(
		const uint32_t* ptr,
		const uint32_t* end)
		: m_ptr(ptr), m_end(end) { }

	const void* ptrAt(uint32_t id) const;

	uint32_t at(uint32_t id) const;

	uint32_t frontDword() const;
	uint64_t frontQword() const;

	uint32_t readDword();
	uint64_t readQword();

	GCNCodeSlice take(uint32_t n) const;
	GCNCodeSlice skip(uint32_t n) const;

	bool atEnd() const 
	{
		return m_ptr == m_end;
	}

private:

	const uint32_t* m_ptr = nullptr;
	const uint32_t* m_end = nullptr;

};

//////////////////////////////////////////////////////////////////////////


class GCNDecodeContext
{
public:
	GCNDecodeContext();
	~GCNDecodeContext();

	void decodeInstruction(GCNCodeSlice& code);

	GCNInstruction& getInstruction();

private:
	void freeInstruction();

private:
	const GDT_HW_GENERATION m_ps4HWGeneration = GDT_HW_GENERATION_SEAISLAND;
	GCNParser m_parser;
	GCNInstruction m_instruction;
};



} // namespace pssl