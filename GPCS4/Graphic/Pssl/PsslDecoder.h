#pragma once

#include "GPCS4Common.h"

namespace pssl
{;

 
struct PsslShaderInstruction
{
	uint32_t placeHolder;
};

//////////////////////////////////////////////////////////////////////////
class PsslCodeSlice {

public:

	PsslCodeSlice(
		const uint32_t* ptr,
		const uint32_t* end)
		: m_ptr(ptr), m_end(end) { }

	const uint32_t* ptrAt(uint32_t id) const;

	uint32_t at(uint32_t id) const;
	uint32_t read();

	PsslCodeSlice take(uint32_t n) const;
	PsslCodeSlice skip(uint32_t n) const;

	bool atEnd() const 
	{
		return m_ptr == m_end;
	}

private:

	const uint32_t* m_ptr = nullptr;
	const uint32_t* m_end = nullptr;

};

//////////////////////////////////////////////////////////////////////////


class PsslDecoder
{
public:
	PsslDecoder();
	~PsslDecoder();

	void decodeInstruction(PsslCodeSlice slice);

	const PsslShaderInstruction&  getInstruction();

private:

};



} // namespace pssl