#pragma once

#include "GPCS4Common.h"
#include "GCNInstruction.h"
#include "GCNParser/GCNParser.h"


namespace pssl
{;

class GCNCodeSlice {

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

	const GCNInstruction&  getInstruction();

	void freeInstruction();

private:
	

private:
	GCNParser m_parser;
	GCNInstruction m_instruction;
};



} // namespace pssl