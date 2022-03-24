#pragma once

#include "../../memory_monitor_impl.h"
#include <memory>
#include <array>

namespace Xbyak
{
class CodeGenerator;
}


class X86MemoryMonitorImpl : public MemoryMonitorImpl
{
private:
	enum class InstructionType
	{
		None,
		ModRm,
		AbsAddr,
		Xlat,
		StringOp,
		GatherScatter
	};

	enum class AvxFeature
	{
		SSE,
		AVX,
		AVX512
	};

	struct AvxInformation
	{
		AvxFeature feature;
		bool       support_xsave;
		uint32_t   xsave_frame_size;
	};

	struct GatherScatterInfo
	{
		size_t        index_size  = 0;
		size_t        value_size  = 0;
		size_t        vector_size = 0;
		bool          is_load     = false;
		bool          is_evex     = false;
		ZydisRegister index_reg   = ZYDIS_REGISTER_NONE;
		ZydisRegister base_reg    = ZYDIS_REGISTER_NONE;
		uint32_t      disp        = 0;
		uint8_t       scale       = 0;
	};

	// temp code size to generate instruction to call callbacks
	constexpr static size_t TempCodeSize    = 0x300;
	constexpr static size_t ShadowSpaceSize = 0x20;
	constexpr static size_t XmmRegWidth     = 16;
	constexpr static size_t YmmRegWidth     = 32;
	constexpr static size_t ZmmRegWidth     = 64;

public:
	X86MemoryMonitorImpl(MonitorFlags flags, MemoryCallback* callback);
	virtual ~X86MemoryMonitorImpl();

protected:
	InstructionResult InstrumentInstruction(
		ModuleInfo*  module,
		Instruction& inst,
		size_t       bb_address,
		size_t       instruction_address) override;

private:
	bool NeedToHandle(Instruction& inst);

	InstructionType GetInstructionType(
		const Instruction& inst);

	void DetectAvxInformation();

	void EmitSaveContext(Xbyak::CodeGenerator& a);
	void EmitRestoreContext(Xbyak::CodeGenerator& a);

	void EmitSaveContextEs(Xbyak::CodeGenerator& a);
	void EmitRestoreContextEs(Xbyak::CodeGenerator& a);
	void EmitSaveContextNoEs(Xbyak::CodeGenerator& a);
	void EmitRestoreContextNoEs(Xbyak::CodeGenerator& a);


	void EmitProlog(Xbyak::CodeGenerator& a);
	void EmitEpilog(Xbyak::CodeGenerator& a);

	void EmitPrologEs(Xbyak::CodeGenerator& a);
	void EmitEpilogEs(Xbyak::CodeGenerator& a);
	void EmitPrologNoEs(Xbyak::CodeGenerator& a);
	void EmitEpilogNoEs(Xbyak::CodeGenerator& a);

	InstructionResult EmitMemoryAccess(
		const Instruction& inst,
		Xbyak::CodeGenerator& a);

	void EmitMemoryCallback(
		const Instruction&         inst,
		Xbyak::CodeGenerator&      a,
		bool                       is_write,
		const ZydisDecodedOperand* mem_operand,
		ZydisRegister              addr_register);

	void EmitGetMemoryAddress(
		const Instruction&         inst,
		Xbyak::CodeGenerator&      a,
		const ZydisDecodedOperand* mem_operand,
		ZydisRegister              dst);

	void EmitGetMemoryAddressNormal(
		const Instruction&         inst,
		Xbyak::CodeGenerator&      a,
		const ZydisDecodedOperand* mem_operand,
		ZydisRegister              dst);


	InstructionResult EmitExplicitMemoryAccess(
		const Instruction& inst,
		Xbyak::CodeGenerator& a);

	InstructionResult EmitStringOp(
		const Instruction& inst,
		Xbyak::CodeGenerator& a);

	InstructionResult EmitXlat(
		const Instruction& inst,
		Xbyak::CodeGenerator& a);

	InstructionResult EmitGatherScatter(
		const Instruction&    inst,
		Xbyak::CodeGenerator& a);
	InstructionResult EmitGatherScatterEs(
		const Instruction&    inst,
		Xbyak::CodeGenerator& a);
	InstructionResult EmitGatherScatterNoEs(
		const Instruction&    inst,
		Xbyak::CodeGenerator& a);
	void EmitGatherScatterElementAccess(
		const Instruction&       inst,
		Xbyak::CodeGenerator&    a,
		const GatherScatterInfo& info,
		ZydisRegister            temp_xmm);

	void EmitExtractScalarFromVector(
		Xbyak::CodeGenerator& a,
		size_t                element_idx,
		size_t                element_size,
		ZydisRegister         vec_reg,
		ZydisRegister         tmp_xmm,
		ZydisRegister         dst_reg,
		bool                  is_avx512);

	ZydisRegister EmitPreWrite(
		const Instruction&    inst,
		Xbyak::CodeGenerator& a);
	void EmitPostWrite(
		const Instruction&    inst,
		Xbyak::CodeGenerator& a,
		ZydisRegister         addr_register);

	void EmitStringRead(
		const Instruction&                inst,
		Xbyak::CodeGenerator&             a,
		const std::vector<ZydisRegister>& src_reg_list);

	void EmitStringWrite(
		const Instruction&    inst,
		Xbyak::CodeGenerator& a);

	void GetGatherScatterInfo(
		const Instruction& inst,
		GatherScatterInfo* info);

private:
	AvxInformation                        avx_info = {};
	std::array<uint8_t, TempCodeSize>     code_buffer;
	std::unique_ptr<Xbyak::CodeGenerator> code_generator;
};


// When SaveExtendedState is not set,
// simd registers (xmm, ymm etc.) will not be saved during user callbacks,
// this will improve the performance distinctly.
// But at the same time this will corrupt simd registers and crash the target
// if such registers are used in user callbacks.
// And it is hard to disable such register usage for a function.
// 
// This is a helpful macro to force a function not to use simd instructions,
// but it is not guaranteed to work as expected, 
// because it can only force the top level function, not for sub function calls,
// especially for some functions which will call into the kernel implicitly (like many STL functions).
// So be careful with SaveExtendedState flag.

#define DISABLE_SIMD_INSTRUCTION __attribute__((__target__("arch=x86-64,no-fxsr,no-mmx,no-fxsr,no-sse,no-sse2")))