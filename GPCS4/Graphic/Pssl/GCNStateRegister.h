#pragma once

#include "PsslCommon.h"
#include "GCNSpirvTypes.h"

#include <string>

namespace pssl
{;

class SpirvModule;

struct GcnStateRegister
{
	SpirvRegisterPointer lo;
	SpirvRegisterPointer hi;

	// Load the state register as Uint64
	SpirvRegisterValue load(
		SpirvModule& module) const;

	// Store a Uint64 value to the state register
	void store(
		SpirvModule&              module,
		const SpirvRegisterValue& value) const;
};


// For all status registers related to thread,
// we always assume we are running on 0's thread, thread_id = 1.
// ie. exec will be initialized to 1.
struct GcnStateRegisters
{
	GcnStateRegister vcc;

	GcnStateRegister exec;

	SpirvRegisterPointer m0;

	// Spirv condition register, 
	// since I didn't find any instructions 
	// which will load/store scc directly,
	// I treat scc as bool type.
	SpirvRegisterValue scc;
};


}  // namespace pssl

