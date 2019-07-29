#pragma once

#include "GPCS4Common.h"
#include "PsslShaderInstruction.h"

namespace pssl
{;

class GCNCodeSlice {

public:

	GCNCodeSlice(
		const uint32_t* ptr,
		const uint32_t* end)
		: m_ptr(ptr), m_end(end) { }

	const uint32_t* ptrAt(uint32_t id) const;

	uint32_t at(uint32_t id) const;
	uint32_t read();

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

	void decodeInstruction(GCNCodeSlice slice);

	const PsslShaderInstruction&  getInstruction();

private:

	PsslInstructionEncoding getInstructionEncoding(uint32_t token);

private:
	PsslShaderInstruction m_instruction;
};



} // namespace pssl