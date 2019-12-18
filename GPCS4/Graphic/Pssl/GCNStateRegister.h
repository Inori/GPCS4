#pragma once

#include "PsslCommon.h"
#include "GCNSpirvTypes.h"

#include <string>

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
	SpirvRegisterU64(GCNCompiler* compiler, const std::string& name = nullptr);
	~SpirvRegisterU64();

	// lower 32bit uint32
	SpirvRegisterPointer low();

	// higher 32bit uint32
	SpirvRegisterPointer high();

	// whole 64bit uint64
	SpirvRegisterPointer value();

private:
	SpirvRegisterPointer createU64Value(const std::string& name);
	SpirvRegisterPointer castToVec2(SpirvRegisterPointer u64Val);
	SpirvRegisterPointer mapAccessPtr(RegType type);

private:
	GCNCompiler* m_compiler;
	SpirvModule* m_module;
	std::string m_name;

	SpirvRegisterPointer m_value;
	SpirvRegisterPointer m_vec2Ptr;

	// We cast a uint64 value to a vec2 (two uint32),
	// so that we can access both low and high part.
	SpirvRegisterPointer m_low;
	SpirvRegisterPointer m_high;
};


struct GcnStateRegister
{
	GcnStateRegister(GCNCompiler* compiler):
		scc(compiler, "scc"),
		vcc(compiler, "vcc"),
		exec(compiler, "exec")
	{}

	SpirvRegisterU64 scc;
	SpirvRegisterU64 vcc;
	SpirvRegisterU64 exec;
};


}  // namespace pssl

