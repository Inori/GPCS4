#pragma once

#include "PsslCommon.h"
#include "GCNSpirvTypes.h"

#include <string>
#include <optional>

namespace pssl
{;

class GCNCompiler;
class SpirvModule;

class SpirvRegisterU64
{
	enum class RegType
	{
		REG64_LOW,
		REG64_HIGH,
		REG64_FULL,
	};

public:
	SpirvRegisterU64(
		GCNCompiler* compiler, 
		const std::string& name = nullptr, 
		std::optional<uint32_t> initValue = std::nullopt);
	~SpirvRegisterU64();

	// lower 32bit uint32
	SpirvRegisterPointer low();

	// higher 32bit uint32
	SpirvRegisterPointer high();

	// whole 64bit uint64
	SpirvRegisterPointer value();

private:
	SpirvRegisterPointer createU64Value(const std::string& name, std::optional<uint32_t> initValue);
	SpirvRegisterPointer castToVec2(SpirvRegisterPointer u64Val);
	SpirvRegisterPointer mapAccessPtr(RegType type);

private:
	GCNCompiler* m_compiler;
	SpirvModule* m_module;
	std::string m_name;
	std::optional<uint32_t> m_initValue;

	SpirvRegisterPointer m_value;
	SpirvRegisterPointer m_vec2Ptr;

	// We cast a uint64 value to a vec2 (two uint32),
	// so that we can access both low and high part.
	SpirvRegisterPointer m_low;
	SpirvRegisterPointer m_high;
};


// For all status registers related to thread,
// we always assume we are running on 0's thread, thread_id = 1.
// ie. exec will be initialized to 1.
struct GcnStateRegister
{
	//	GcnStateRegister(GCNCompiler* compiler):
	//		vcc(compiler, "vcc"),
	//		exec(compiler, "exec", 1)
	//	{}
	//
	//	SpirvRegisterU64 vcc;
	//	SpirvRegisterU64 exec;

	// Based on above, a 32 bits uint value is enough to hold state information,
	// thus we treat vcc and vcc_lo as the same register
	// of type Uint32, and vcc_hi as another.
	SpirvRegisterPointer vcc;  // for both vcc and vcc_lo
	SpirvRegisterPointer vcc_hi;

	// Same for exec.
	SpirvRegisterPointer exec;  // for both exec and exec_lo
	SpirvRegisterPointer exec_hi;

	//SpirvRegisterPointer scc;
	SpirvRegisterPointer m0;

	// spirv condition
	SpirvRegisterValue sccz;  // There's no hardware sccz, branch instruction detect if scc is 0 or 1
};


}  // namespace pssl

