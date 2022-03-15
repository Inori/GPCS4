#include "x86_memory_monitor.h"
#include "x86_helpers.h"
#include "xbyak.h"
#include <intrin.h>

#ifdef max
#undef max
#endif


X86MemoryMonitor::X86MemoryMonitor(MonitorFlags flags, MemoryCallback* callback) :
	MemoryMonitor(flags, callback)
{
	code_generator = std::make_unique<Xbyak::CodeGenerator>(code_buffer.size(), code_buffer.data());
	DetectAvxInformation();
}

X86MemoryMonitor::~X86MemoryMonitor()
{
}

X86MemoryMonitor::InstructionType X86MemoryMonitor::GetInstructionType(const Instruction& inst)
{
	InstructionType type = InstructionType::None;

	const auto&     zinst = inst.zinst.instruction;
	const auto      category = zinst.meta.category;
	const auto      mem_op   = GetExplicitMemoryOperand(inst.zinst.operands, zinst.operand_count_visible);

	// note:
	// the condition order is important
	// although it's ugly...
	if (category == ZYDIS_CATEGORY_AVX2GATHER ||
		category == ZYDIS_CATEGORY_GATHER ||
		category == ZYDIS_CATEGORY_SCATTER)
	{
		type = InstructionType::GatherScatter;
	}
	else if (zinst.attributes & ZYDIS_ATTRIB_HAS_MODRM)
	{
		type = InstructionType::ModRm;
	}
	else if (mem_op != nullptr &&
			 mem_op->mem.base == ZYDIS_REGISTER_NONE &&
			 mem_op->mem.index == ZYDIS_REGISTER_NONE)
	{
		type = InstructionType::AbsAddr;
	}
	else if (zinst.mnemonic == ZYDIS_MNEMONIC_XLAT)
	{
		type = InstructionType::Xlat;	
	}
	else if (category == ZYDIS_CATEGORY_STRINGOP)
	{
		type = InstructionType::StringOp;
	} 

	return type;
}

void X86MemoryMonitor::DetectAvxInformation()
{
	bool support_avx    = false;
	bool support_avx512 = false;
	DetectAvxSupport(support_avx, support_avx512);

	// assume all cpu support sse
	avx_info.feature = AvxFeature::SSE;
	if (support_avx)
	{
		avx_info.feature = AvxFeature::AVX;
	}
	if (support_avx512)
	{
		avx_info.feature = AvxFeature::AVX512;
	}

	// TODO:
	assert(avx_info.feature == AvxFeature::AVX || avx_info.feature == AvxFeature::AVX512);

	int32_t info[4] = { 0 };
	__cpuidex(info, 1, 0);
	avx_info.support_xsave = ((info[2] & ((int)1 << 26)) != 0) &&	// XSAVE
							 ((info[2] & ((int)1 << 27)) != 0);		// OSXSAVE
	// TODO:
	// Support CPUs which don't support xsave/xrstor
	assert(avx_info.support_xsave == true);

	__cpuidex(info, 0x0D, 0);
	// ebx: size to keep currently enabled components of the frame
	// ecx: maximal frame size of all components
	// here we take ecx value
	avx_info.xsave_frame_size = info[2];
}

void X86MemoryMonitor::EmitGetMemoryAddress(
	const Instruction&         inst,
	Xbyak::CodeGenerator&      a,
	const ZydisDecodedOperand* mem_operand,
	ZydisRegister              dst)
{
	using namespace Xbyak::util;

	const auto& zinst = inst.zinst;

	if (mem_operand->type != ZYDIS_OPERAND_TYPE_MEMORY)
	{
		FATAL("Error operand type.");
	}

	if (mem_operand->mem.base != ZYDIS_REGISTER_RIP)
	{
		if (mem_operand->mem.type != ZYDIS_MEMOP_TYPE_VSIB)
		{
			EmitGetMemoryAddressNormal(inst, a, mem_operand, dst);
		}
		else
		{
			// handled in EmitGatherScatter
			FATAL("unexpected mem type VSIB detected, should be handled elsewhere.");
		}
	}
	else
	{
		// rip-relative address

		// TODO:
		// currently, inst.address is not the runtime address of the instruction
		// as we decode on the backup memory, which is not needed when we are already
		// running in the memory space of the target process.
		// we should remove the old TinyInst's backup strategy and use real runtime address.
		FATAL("TODO: runtime address is not implemented.");

		ZyanU64 abs_address = 0;
		if (ZYAN_FAILED(ZydisCalcAbsoluteAddress(
				&zinst.instruction,
				mem_operand,
				inst.address,
				&abs_address)))
		{
			FATAL("Calculate absolute address failed.");
		}
		a.mov(ZydisRegToXbyakReg(dst), abs_address);
	}
}

void X86MemoryMonitor::EmitGetMemoryAddressNormal(
	const Instruction& inst, Xbyak::CodeGenerator& a, const ZydisDecodedOperand* mem_operand, ZydisRegister dst)
{
	using namespace Xbyak::util;

	const auto&         zinst   = inst.zinst;
	bool                is_xlat = false;
	ZydisDecodedOperand mem_op  = *mem_operand;
	if (zinst.instruction.mnemonic == ZYDIS_MNEMONIC_XLAT)
	{
		is_xlat = true;

		if (dst != ZYDIS_REGISTER_RAX)
		{
			a.push(rax);
		}

		a.movzx(rax, al);
		mem_op.mem.index = ZYDIS_REGISTER_RAX;
	}

	// lea rdx, [mem]
	uint8_t encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	size_t  encoded_length = sizeof(encoded_instruction);
	encoded_length         = LeaReg(zinst.instruction.machine_mode, dst,
                            mem_op, zinst.instruction.address_width,
                            encoded_instruction, encoded_length);

	if (is_xlat && dst != ZYDIS_REGISTER_RAX)
	{
		a.pop(rax);
	}
	a.db(encoded_instruction, encoded_length);
}

InstructionResult X86MemoryMonitor::EmitExplicitMemoryAccess(
	const Instruction& inst, Xbyak::CodeGenerator& a)
{
	InstructionResult action = INST_NOTHANDLED;

	const auto& zinst   = inst.zinst;
	const auto  operand = GetExplicitMemoryOperand(
        zinst.operands, zinst.instruction.operand_count_visible);

	if (operand->actions & ZYDIS_OPERAND_ACTION_MASK_READ)
	{
		EmitMemoryCallback(inst, a, false, operand, ZYDIS_REGISTER_NONE);

		// return INST_NOTHANDLED which causes
		// the original instruction to be appended
		action = INST_NOTHANDLED;
	}

	if (operand->actions & ZYDIS_OPERAND_ACTION_MASK_WRITE)
	{
		auto addr_reg = EmitPreWrite(inst, a);

		EmitMemoryCallback(inst, a, true, operand, addr_reg);
	
		EmitPostWrite(inst, a, addr_reg);

		// we've already emit the original instruction in EmitPreWrite
		// so let tinydbr pass the original instruction.
		action = INST_HANDLED;
	}

	return action;
}

ZydisRegister X86MemoryMonitor::EmitPreWrite(
	const Instruction& inst, Xbyak::CodeGenerator& a)
{
	using namespace Xbyak::util;

	const auto& zinst = inst.zinst;
	auto addr_reg = GetFreeRegister(zinst.instruction, zinst.operands);
	
	a.push(ZydisRegToXbyakReg(addr_reg));

	auto mem_op = GetExplicitMemoryOperand(
		zinst.operands, zinst.instruction.operand_count_visible);
	EmitGetMemoryAddress(inst, a, mem_op, addr_reg);

	// insert original instruction
	a.db(reinterpret_cast<const uint8_t*>(inst.address), inst.length);

	return addr_reg;
}

void X86MemoryMonitor::EmitPostWrite(
	const Instruction& inst, Xbyak::CodeGenerator& a, ZydisRegister addr_register)
{
	using namespace Xbyak::util;

	a.pop(ZydisRegToXbyakReg(addr_register));
}

InstructionResult X86MemoryMonitor::EmitXlat(
	const Instruction& inst, Xbyak::CodeGenerator& a)
{
	using namespace Xbyak::util;

	EmitSaveContext(a);
	
	EmitGetMemoryAddress(inst, a, 
		&inst.zinst.operands[0], ZYDIS_REGISTER_RDX);

	EmitProlog(a);

	// mov rcx, this
	a.mov(rcx, reinterpret_cast<uint64_t>(this));
	// xlat:
	// Set AL to memory byte [RBX + unsigned AL].
	// so the size is always 1 byte
	a.mov(r8d, 1);

	auto func = decltype(&X86MemoryMonitor::OnMemoryRead)(&X86MemoryMonitor::OnMemoryRead);
	uint64_t callback  = reinterpret_cast<uint64_t>(reinterpret_cast<void*&>(func));
	// now the parameters are all ready
	// call the memory access callback
	a.mov(rax, callback);
	a.call(rax);

	EmitEpilog(a);
	EmitRestoreContext(a);
	return INST_NOTHANDLED;
}

// see DynamicRIO: drx_expand_scatter_gather
InstructionResult X86MemoryMonitor::EmitGatherScatter(const Instruction& inst, Xbyak::CodeGenerator& a)
{
	using namespace Xbyak::util;

	InstructionResult action = INST_NOTHANDLED;
	if (m_flags & SaveExtendedState)
	{
		action = EmitGatherScatterEs(inst, a);
	}
	else
	{
		action = EmitGatherScatterNoEs(inst, a);
	}

	return action;
}

InstructionResult X86MemoryMonitor::EmitGatherScatterEs(
	const Instruction& inst, Xbyak::CodeGenerator& a)
{
	using namespace Xbyak::util;

	InstructionResult action = INST_NOTHANDLED;
	const auto&       zinst  = inst.zinst;

	GatherScatterInfo info = {};
	GetGatherScatterInfo(inst, &info);

	if (!info.is_load)
	{
		// If any pair of the index, mask, or destination registers are the same, this instruction results a #UD fault.
		// So we don't need to save the index register before write.
		a.db(reinterpret_cast<uint8_t*>(inst.address), inst.length);
		action = INST_HANDLED;
	}

	EmitSaveContextEs(a);

	// set max stack align
	uint64_t stack_space = ((ShadowSpaceSize + ZmmRegWidth) / ZmmRegWidth + 1) * ZmmRegWidth;
	// the stack is already aligned in EmitSaveContext
	// so we keep the alignment
	a.sub(rsp, stack_space);

	auto temp_xmm = GetFreeRegisterSSE(zinst.instruction, zinst.operands);

	// generate expansion calls
	EmitGatherScatterElementAccess(inst, a, info, temp_xmm);


	a.add(rsp, stack_space);
	EmitRestoreContextEs(a);

	return action;
}

InstructionResult X86MemoryMonitor::EmitGatherScatterNoEs(
	const Instruction& inst, Xbyak::CodeGenerator& a)
{
	using namespace Xbyak::util;

	InstructionResult action = INST_NOTHANDLED;
	const auto&       zinst  = inst.zinst;

	GatherScatterInfo info = {};
	GetGatherScatterInfo(inst, &info);

	if (!info.is_load)
	{
		// If any pair of the index, mask, or destination registers are the same, this instruction results a #UD fault.
		// So we don't need to save the index register before write.
		a.db(reinterpret_cast<uint8_t*>(inst.address), inst.length);
		action = INST_HANDLED;
	}

	EmitSaveContextNoEs(a);

	// reserve stack and align
	a.mov(rbp, rsp);
	// set max stack align
	a.sub(rsp, ShadowSpaceSize + ZmmRegWidth);
	a.and_(rsp, static_cast<uint32_t>(~(ZmmRegWidth - 1)));


	auto temp_xmm = GetFreeRegisterSSE(zinst.instruction, zinst.operands);
	// We save the largest reg corresponding to temp_xmm that is supported by the system.
	// This is required because mov-ing a part of a
	// ymm/zmm reg zeroes the remaining automatically. So we need to save the complete
	// ymm/zmm reg and not just the lower xmm bits.
	auto temp_mm = GetFullSizeVectorRegister(temp_xmm);

	uint8_t encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	size_t  encoded_length = sizeof(encoded_instruction);
	encoded_length         = MovStackAVX(zinst.instruction.machine_mode, 0,
                                 temp_mm, true, encoded_instruction, encoded_length);
	a.db(encoded_instruction, encoded_length);

	// generate expansion calls
	EmitGatherScatterElementAccess(inst, a, info, temp_xmm);


	// restore mm
	encoded_length = MovStackAVX(zinst.instruction.machine_mode, 0,
								 temp_mm, false, encoded_instruction, encoded_length);
	a.db(encoded_instruction, encoded_length);

	a.mov(rsp, rbp);
	EmitRestoreContextNoEs(a);

	return action;
}

void X86MemoryMonitor::EmitGatherScatterElementAccess(
	const Instruction&       inst,
	Xbyak::CodeGenerator&    a,
	const GatherScatterInfo& info,
	ZydisRegister            temp_xmm)
{
	using namespace Xbyak::util;

	const auto& zinst  = inst.zinst;

	uint32_t element_count = info.vector_size / std::max(info.index_size, info.value_size);

	auto     read_func  = decltype(&X86MemoryMonitor::OnMemoryRead)(&X86MemoryMonitor::OnMemoryRead);
	uint64_t on_read    = reinterpret_cast<uint64_t>(reinterpret_cast<void*&>(read_func));
	auto     write_func = decltype(&X86MemoryMonitor::OnMemoryWrite)(&X86MemoryMonitor::OnMemoryWrite);
	uint64_t on_write   = reinterpret_cast<uint64_t>(reinterpret_cast<void*&>(write_func));

	// reserve a xmm register and a gpr register
	auto free_reg_pair   = GetFreeRegister2(zinst.instruction, zinst.operands);
	auto index_reg       = free_reg_pair.first;
	auto base_backup_reg = free_reg_pair.second;

	auto xbk_index_reg       = ZydisRegToXbyakReg(index_reg);
	auto xbk_base_backup_reg = ZydisRegToXbyakReg(base_backup_reg);

	// TODO:
	// When specify SaveExtendedState flag, rax, rdx and rdi will be modified
	// in EmitSaveContext, thus if the instruction is scatter and address base
	// register is one of the above, the address will be broken, need to fix.
	// e.g. recover again from the pushaq stack before this mov.
	a.mov(xbk_base_backup_reg, ZydisRegToXbyakReg(info.base_reg));

	for (uint32_t i = 0; i != element_count; ++i)
	{
		// extract scalar index value for vsib
		EmitExtractScalarFromVector(a, i, info.index_size, info.index_reg, temp_xmm, index_reg, info.is_evex);

		// sign extend the extracted value.
		if (info.index_size == 4)
		{
			a.movsxd(ZydisRegToXbyakReg<Xbyak::Reg64>(index_reg), ZydisRegToXbyakReg(GetNBitRegister(index_reg, 32)));
		}

		// calculate the memory address with normal ModRm/SIB form.
		const uint8_t shift_table[] = { 0, 0, 1, 0, 2, 0, 0, 0, 3 };
		if (info.scale != 1)
		{
			uint8_t shift_bits = shift_table[info.scale];
			a.shl(xbk_index_reg, shift_bits);
		}

		a.add(xbk_index_reg, xbk_base_backup_reg);

		if (info.disp != 0)
		{
			a.adc(xbk_index_reg, info.disp);
		}

		// everything is done
		// call the callback function
		a.mov(rcx, reinterpret_cast<uint64_t>(this));
		a.mov(rdx, xbk_index_reg);
		a.mov(r8, info.value_size);
		if (info.is_load)
		{
			a.mov(rax, on_read);
		}
		else
		{
			a.mov(rax, on_write);
		}
		a.call(rax);
	}
}

void X86MemoryMonitor::EmitExtractScalarFromVector(
	Xbyak::CodeGenerator& a,
	size_t                element_idx,
	size_t                element_size,
	ZydisRegister         vec_reg,
	ZydisRegister         tmp_xmm,
	ZydisRegister         dst_reg,
	bool                  is_avx512)
{
	using namespace Xbyak::util;

	ZydisRegister vec_reg_zmm  = GetNBitVectorRegister(vec_reg, ZmmRegWidth * 8);
	ZydisRegister vec_reg_ymm  = GetNBitVectorRegister(vec_reg, YmmRegWidth * 8);
	int           scalarxmmimm = element_idx * element_size / XmmRegWidth;
	
	auto xmm = ZydisRegToXbyakReg(tmp_xmm);

	if (is_avx512)
	{
		//PREXL8(bb, sg_instr,
		//	   INSTR_XL8(INSTR_CREATE_vextracti32x4_mask(
		//					 drcontext, opnd_create_reg(scratch_xmm),
		//					 opnd_create_reg(DR_REG_K0),
		//					 opnd_create_immed_int(scalarxmmimm, OPSZ_1),
		//					 opnd_create_reg(from_simd_reg_zmm)),
		//				 orig_app_pc));


		// force cast Xbyak::Reg to Xbyak::Zmm
		// k0 is always 0xFFFFFFFFFFFFFFFF
		a.vextracti32x4(xmm | k0, ZydisRegToXbyakReg<Xbyak::Zmm>(vec_reg_zmm), scalarxmmimm);
	}
	else
	{
		//PREXL8(bb, sg_instr,
		//	   INSTR_XL8(
		//		   INSTR_CREATE_vextracti128(drcontext, opnd_create_reg(scratch_xmm),
		//									 opnd_create_reg(from_simd_reg_ymm),
		//									 opnd_create_immed_int(scalarxmmimm, OPSZ_1)),
		//		   orig_app_pc));
		 
		
		// force cast Xbyak::Reg to Xbyak::Ymm
		a.vextracti128(xmm, ZydisRegToXbyakReg<Xbyak::Ymm>(vec_reg_ymm), scalarxmmimm);
	}

	if (element_size == 4)
	{
		//PREXL8(bb, sg_instr,
		//	   INSTR_XL8(INSTR_CREATE_vpextrd(
		//					 drcontext,
		//					 opnd_create_reg(
		//						 IF_X64_ELSE(reg_64_to_32(scratch_reg), scratch_reg)),
		//					 opnd_create_reg(scratch_xmm),
		//					 opnd_create_immed_int((el * scalar_bytes) % XMM_REG_SIZE /
		//											   opnd_size_in_bytes(OPSZ_4),
		//										   OPSZ_1)),
		//				 orig_app_pc));

		uint8_t imm = (element_idx * element_size) % XmmRegWidth / 4;
		auto    dst = GetNBitRegister(dst_reg, 32);
		a.vpextrd(ZydisRegToXbyakReg(dst), ZydisRegToXbyakReg<Xbyak::Xmm>(tmp_xmm), imm);
	}
	else if (element_size == 8)
	{
		if (!Is64BitGPRRegister(dst_reg))
		{
			FATAL("The qword index versions are unsupported in 32-bit mode.");
		}
		
		//PREXL8(bb, sg_instr,
		//	   INSTR_XL8(INSTR_CREATE_vpextrq(
		//					 drcontext, opnd_create_reg(scratch_reg),
		//					 opnd_create_reg(scratch_xmm),
		//					 opnd_create_immed_int((el * scalar_bytes) % XMM_REG_SIZE /
		//											   opnd_size_in_bytes(OPSZ_8),
		//										   OPSZ_1)),
		//				 orig_app_pc));
		uint8_t imm = (element_idx * element_size) % XmmRegWidth / 8;
		a.vpextrq(ZydisRegToXbyakReg(dst_reg), ZydisRegToXbyakReg<Xbyak::Xmm>(tmp_xmm), imm);
	}
	else
	{
		FATAL("Unexpected scalar size.");
	}
}

void X86MemoryMonitor::EmitStringRead(
	const Instruction& inst, Xbyak::CodeGenerator& a, 
	const std::vector<ZydisRegister>& src_reg_list)
{
	using namespace Xbyak::util;
	const auto& zinst = inst.zinst;
	Xbyak::Label label;

	EmitSaveContext(a);
	EmitProlog(a);

	// size in bytes
	uint8_t operand_size = zinst.instruction.operand_width / 8;

	if (zinst.instruction.attributes & ZYDIS_ATTRIB_HAS_REP ||
		zinst.instruction.attributes & ZYDIS_ATTRIB_HAS_REPE ||
		zinst.instruction.attributes & ZYDIS_ATTRIB_HAS_REPNE)
	{
		a.mov(r15, rcx);

		if (operand_size != 1)
		{
			const uint8_t shift_table[] = { 0, 0, 1, 0, 2, 0, 0, 0, 3 };
			const uint8_t shift_bits    = shift_table[operand_size];
			a.shl(r15, shift_bits);
		}
	}
	else
	{
		a.mov(r15, operand_size);
	}
	
	auto     func     = decltype(&X86MemoryMonitor::OnMemoryRead)(&X86MemoryMonitor::OnMemoryRead);
	uint64_t callback = reinterpret_cast<uint64_t>(reinterpret_cast<void*&>(func));

	a.pushfq();
	a.pop(rax);
	a.bt(rax, 0x0A);
	a.jnc(label);
	for (const auto& src_reg : src_reg_list)
	{
		a.sub(ZydisRegToXbyakReg(src_reg), r15);
	}
a.L(label);
	for (const auto& src_reg : src_reg_list)
	{
		a.mov(rcx, reinterpret_cast<uint64_t>(this));
		a.mov(rdx, ZydisRegToXbyakReg(src_reg));
		a.mov(r8, r15);  // r15 is nonvolatile register
		a.mov(rax, callback);
		a.call(rax);
	}

	EmitEpilog(a);
	EmitRestoreContext(a);
}

void X86MemoryMonitor::EmitStringWrite(
	const Instruction& inst, Xbyak::CodeGenerator& a)
{
	using namespace Xbyak::util;
	const auto&  zinst = inst.zinst;
	Xbyak::Label label;

	EmitSaveContext(a);
	EmitProlog(a);

	// size in bytes
	uint8_t operand_size = zinst.instruction.operand_width / 8;

	if (zinst.instruction.attributes & ZYDIS_ATTRIB_HAS_REP ||
		zinst.instruction.attributes & ZYDIS_ATTRIB_HAS_REPE ||
		zinst.instruction.attributes & ZYDIS_ATTRIB_HAS_REPNE)
	{
		a.sub(r15, rcx);

		if (operand_size != 1)
		{
			const uint8_t shift_table[] = { 0, 0, 1, 0, 2, 0, 0, 0, 3 };
			const uint8_t shift_bits    = shift_table[operand_size];
			a.shl(r15, shift_bits);
		}
	}
	else
	{
		a.mov(r15, operand_size);
	}

	auto     func     = decltype(&X86MemoryMonitor::OnMemoryWrite)(&X86MemoryMonitor::OnMemoryWrite);
	uint64_t callback = reinterpret_cast<uint64_t>(reinterpret_cast<void*&>(func));

	a.pushfq();
	a.pop(rax);
	a.bt(rax, 0x0A);
	a.jc(label);
	a.sub(rdi, r15);  // for string write, destination register must be rdi
a.L(label);
	a.mov(rcx, reinterpret_cast<uint64_t>(this));
	a.mov(rdx, rdi);
	a.mov(r8, r15);
	a.mov(rax, callback);
	a.call(rax);

	EmitEpilog(a);
	EmitRestoreContext(a);
}

void X86MemoryMonitor::GetGatherScatterInfo(const Instruction& inst, GatherScatterInfo* info)
{
	auto opcode = inst.zinst.instruction.mnemonic;
	switch (opcode)
	{
	case ZYDIS_MNEMONIC_VGATHERDPD:
		info->index_size = 4;
		info->value_size = 8;
		info->is_load    = true;
		break;
	case ZYDIS_MNEMONIC_VGATHERQPD:
		info->index_size = 8;
		info->value_size = 8;
		info->is_load    = true;
		break;
	case ZYDIS_MNEMONIC_VGATHERDPS:
		info->index_size = 4;
		info->value_size = 4;
		info->is_load    = true;
		break;
	case ZYDIS_MNEMONIC_VGATHERQPS:
		info->index_size = 8;
		info->value_size = 4;
		info->is_load    = true;
		break;
	case ZYDIS_MNEMONIC_VPGATHERDD:
		info->index_size = 4;
		info->value_size = 4;
		info->is_load    = true;
		break;
	case ZYDIS_MNEMONIC_VPGATHERQD:
		info->index_size = 8;
		info->value_size = 4;
		info->is_load    = true;
		break;
	case ZYDIS_MNEMONIC_VPGATHERDQ:
		info->index_size = 4;
		info->value_size = 8;
		info->is_load    = true;
		break;
	case ZYDIS_MNEMONIC_VPGATHERQQ:
		info->index_size = 8;
		info->value_size = 8;
		info->is_load    = true;
		break;
	case ZYDIS_MNEMONIC_VSCATTERDPD:
		info->index_size = 4;
		info->value_size = 8;
		info->is_load    = false;
		break;
	case ZYDIS_MNEMONIC_VSCATTERQPD:
		info->index_size = 8;
		info->value_size = 8;
		info->is_load    = false;
		break;
	case ZYDIS_MNEMONIC_VSCATTERDPS:
		info->index_size = 4;
		info->value_size = 4;
		info->is_load    = false;
		break;
	case ZYDIS_MNEMONIC_VSCATTERQPS:
		info->index_size = 8;
		info->value_size = 4;
		info->is_load    = false;
		break;
	case ZYDIS_MNEMONIC_VPSCATTERDD:
		info->index_size = 4;
		info->value_size = 4;
		info->is_load    = false;
		break;
	case ZYDIS_MNEMONIC_VPSCATTERQD:
		info->index_size = 8;
		info->value_size = 4;
		info->is_load    = false;
		break;
	case ZYDIS_MNEMONIC_VPSCATTERDQ:
		info->index_size = 4;
		info->value_size = 8;
		info->is_load    = false;
		break;
	case ZYDIS_MNEMONIC_VPSCATTERQQ:
		info->index_size = 8;
		info->value_size = 8;
		info->is_load    = false;
		break;
	default:
		FATAL("Unsupported opcode.");
		break;
	}

	info->is_evex = inst.zinst.instruction.attributes & ZYDIS_ATTRIB_HAS_EVEX;

	const ZydisDecodedOperand* operand = nullptr;
	if (info->is_load)
	{
		operand = &inst.zinst.operands[0];
	}
	else
	{
		operand = &inst.zinst.operands[2];
	}
	info->vector_size = operand->size / 8;

	auto mem_op = GetExplicitMemoryOperand(
		inst.zinst.operands, inst.zinst.instruction.operand_count_visible);
	info->base_reg  = mem_op->mem.base;
	info->index_reg = mem_op->mem.index;
	info->disp      = mem_op->mem.disp.value;
	info->scale     = mem_op->mem.scale;
}

InstructionResult X86MemoryMonitor::EmitStringOp(const Instruction& inst, Xbyak::CodeGenerator& a)
{
	using namespace Xbyak::util;

	InstructionResult action = INST_NOTHANDLED;

	const auto& zinst = inst.zinst.instruction;
	// In 64-bit mode, if 67H is used to override address size attribute,
	// the count register is ECX and any implicit source/destination operand will
	// use the corresponding 32-bit index register.
	if (zinst.raw.prefix_count == 2 && zinst.raw.prefixes[1].value == 0x67)
	{
		// unlikely case
		FATAL("Not support 32-bit string instruction yet.");
	}

	bool                       has_write = false;
	std::vector<ZydisRegister> src_regs;
	for (size_t i = 0; i != zinst.operand_count; ++i)
	{
		auto& operand = inst.zinst.operands[i];
		if ((operand.type == ZYDIS_OPERAND_TYPE_MEMORY) &&
			(operand.actions & ZYDIS_OPERAND_ACTION_MASK_READ))
		{
			src_regs.push_back(operand.mem.base);
		}

		if (operand.actions & ZYDIS_OPERAND_ACTION_MASK_WRITE)
		{
			has_write = true;
		}
	}

	if (!src_regs.empty())
	{
		EmitStringRead(inst, a, src_regs);
	}

	if (has_write)
	{
		a.push(r15);
		a.mov(r15, rcx);
		a.db(reinterpret_cast<const uint8_t*>(inst.address), inst.length);

		EmitStringWrite(inst, a);

		a.pop(r15);

		action = INST_HANDLED;
	}

	return action;
}

InstructionResult X86MemoryMonitor::EmitMemoryAccess(
	const Instruction& inst, Xbyak::CodeGenerator& a)
{
	InstructionResult action = INST_NOTHANDLED;

	auto type = GetInstructionType(inst);
	switch (type)
	{
	case InstructionType::None:
	case InstructionType::ModRm:
	case InstructionType::AbsAddr:
	{
		action = EmitExplicitMemoryAccess(inst, a);
	}
	break;
	case InstructionType::Xlat:
	{
		action = EmitXlat(inst, a);
	}
	break;
	case InstructionType::StringOp:
	{
		action = EmitStringOp(inst, a);
	}
	break;
	case InstructionType::GatherScatter:
	{
		action = EmitGatherScatter(inst, a);
	}
	break;
	default:
		break;
	}
	
	return action;
}

void X86MemoryMonitor::EmitMemoryCallback(
	const Instruction&         inst,
	Xbyak::CodeGenerator&      a,
	bool                       is_write,
	const ZydisDecodedOperand* mem_operand,
	ZydisRegister              addr_register)
{
	using namespace Xbyak::util;

	EmitSaveContext(a);

	uint64_t callback = 0;
	if (!is_write)
	{
		EmitGetMemoryAddress(inst, a, mem_operand, ZYDIS_REGISTER_RDX);

		auto func = decltype(&X86MemoryMonitor::OnMemoryRead)(&X86MemoryMonitor::OnMemoryRead);
		callback  = reinterpret_cast<uint64_t>(reinterpret_cast<void*&>(func));
	}
	else
	{
		a.mov(rdx, ZydisRegToXbyakReg(addr_register));

		auto func = decltype(&X86MemoryMonitor::OnMemoryWrite)(&X86MemoryMonitor::OnMemoryWrite);
		callback  = reinterpret_cast<uint64_t>(reinterpret_cast<void*&>(func));
	}

	EmitProlog(a);

	// mov rcx, this
	a.mov(rcx, reinterpret_cast<uint64_t>(this));
	// mov r8d, size
	uint32_t mem_size = mem_operand->size / 8;
	a.mov(r8d, mem_size);

	// now the parameters are all ready
	// call the memory access callback
	a.mov(rax, callback);
	a.call(rax);

	EmitEpilog(a);
	EmitRestoreContext(a);
}

// TODO:
// we may need to save/restore xmm registers also.
// 
// pushfq
// pushaq
void X86MemoryMonitor::EmitSaveContext(Xbyak::CodeGenerator& a)
{
	if (m_flags & SaveExtendedState)
	{
		EmitSaveContextEs(a);
	}
	else
	{
		EmitSaveContextNoEs(a);
	}
}

// popaq
// popfq
void X86MemoryMonitor::EmitRestoreContext(Xbyak::CodeGenerator& a)
{
	if (m_flags & SaveExtendedState)
	{
		EmitRestoreContextEs(a);
	}
	else
	{
		EmitRestoreContextNoEs(a);
	}
}


void X86MemoryMonitor::EmitSaveContextEs(Xbyak::CodeGenerator& a)
{
	using namespace Xbyak::util;

	a.pushfq();
	uint8_t encoded_instruction[32] = { 0 };
	size_t  encoded_length          = Pushaq(
        ZYDIS_MACHINE_MODE_LONG_64, encoded_instruction, sizeof(encoded_instruction));
	a.db(encoded_instruction, encoded_length);

	// backup rsp
	a.mov(rbp, rsp);

	a.sub(rsp, avx_info.xsave_frame_size);
	a.and_(rsp, static_cast<uint32_t>(~(64 - 1)));

	// zero the xsave area
	assert(avx_info.xsave_frame_size % sizeof(uint64_t) == 0);
	uint64_t loop_count = avx_info.xsave_frame_size / sizeof(uint64_t);

	a.mov(rcx, loop_count);
	a.mov(rdi, rsp);
	a.xor_(rax, rax);
	a.rep();
	a.stosq();

	// set xsave mask
	a.or_(eax, 0xFFFFFFFF);
	a.or_(edx, 0xFFFFFFFF);

	// emm... xbyak doesn't support xsave, so we use zydis encoder
	ZydisEncoderRequest req;
	memset(&req, 0, sizeof(req));
	req.mnemonic             = ZYDIS_MNEMONIC_XSAVE64;
	req.operand_count        = 1;
	req.operands[0].type     = ZYDIS_OPERAND_TYPE_MEMORY;
	req.operands[0].mem.base = ZYDIS_REGISTER_RSP;

	encoded_length = sizeof(encoded_instruction);
	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		FATAL("Failed to encode instruction");
	}
	a.db(encoded_instruction, encoded_length);
}

void X86MemoryMonitor::EmitRestoreContextEs(Xbyak::CodeGenerator& a)
{
	using namespace Xbyak::util;

	uint8_t encoded_instruction[32] = { 0 };
	size_t  encoded_length          = sizeof(encoded_instruction);

	// set xsave mask
	a.or_(eax, 0xFFFFFFFF);
	a.or_(edx, 0xFFFFFFFF);

	// emm... xbyak doesn't support xrstor, so we use zydis encoder
	ZydisEncoderRequest req;
	memset(&req, 0, sizeof(req));
	req.mnemonic             = ZYDIS_MNEMONIC_XRSTOR64;
	req.operand_count        = 1;
	req.operands[0].type     = ZYDIS_OPERAND_TYPE_MEMORY;
	req.operands[0].mem.base = ZYDIS_REGISTER_RSP;

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		FATAL("Failed to encode instruction");
	}
	a.db(encoded_instruction, encoded_length);

	// restore rsp
	a.mov(rsp, rbp);

	encoded_length = Popaq(
		ZYDIS_MACHINE_MODE_LONG_64, encoded_instruction, sizeof(encoded_instruction));
	a.db(encoded_instruction, encoded_length);
	a.popfq();
}

void X86MemoryMonitor::EmitSaveContextNoEs(Xbyak::CodeGenerator& a)
{
	a.pushfq();

	uint8_t encoded_instruction[32] = { 0 };
	size_t  encoded_length          = Pushaq(
        ZYDIS_MACHINE_MODE_LONG_64, encoded_instruction, sizeof(encoded_instruction));
	a.db(encoded_instruction, encoded_length);
}

void X86MemoryMonitor::EmitRestoreContextNoEs(Xbyak::CodeGenerator& a)
{
	uint8_t encoded_instruction[32] = { 0 };
	size_t  encoded_length          = Popaq(
        ZYDIS_MACHINE_MODE_LONG_64, encoded_instruction, sizeof(encoded_instruction));
	a.db(encoded_instruction, encoded_length);

	a.popfq();
}

void X86MemoryMonitor::EmitProlog(Xbyak::CodeGenerator& a)
{
	if (m_flags & SaveExtendedState)
	{
		EmitPrologEs(a);
	}
	else
	{
		EmitPrologNoEs(a);
	}
}

void X86MemoryMonitor::EmitEpilog(Xbyak::CodeGenerator& a)
{
	if (m_flags & SaveExtendedState)
	{
		EmitEpilogEs(a);
	}
	else
	{
		EmitEpilogNoEs(a);
	}
}

void X86MemoryMonitor::EmitPrologEs(Xbyak::CodeGenerator& a)
{
	using namespace Xbyak::util;

	// shadow space
	a.sub(rsp, ShadowSpaceSize);
}

void X86MemoryMonitor::EmitEpilogEs(Xbyak::CodeGenerator& a)
{
	using namespace Xbyak::util;

	a.add(rsp, ShadowSpaceSize);
}

void X86MemoryMonitor::EmitPrologNoEs(Xbyak::CodeGenerator& a)
{
	using namespace Xbyak::util;

	// backup rsp
	a.mov(rbp, rsp);
	// shadow space
	a.sub(rsp, ShadowSpaceSize);
	// 16 bytes align
	a.and_(rsp, static_cast<uint32_t>(~(16 - 1)));
}

void X86MemoryMonitor::EmitEpilogNoEs(Xbyak::CodeGenerator& a)
{
	using namespace Xbyak::util;

	// restore rsp
	a.mov(rsp, rbp);
}

InstructionResult X86MemoryMonitor::InstrumentInstruction(
	ModuleInfo*  module,
	Instruction& inst,
	size_t       bb_address,
	size_t       instruction_address)
{
	InstructionResult action = INST_NOTHANDLED;
	do 
	{
		if (!NeedToHandle(inst))
		{
			break;
		}

		auto& a = *code_generator;

		// generate call
		action = EmitMemoryAccess(inst, a);

		a.ready();
		if (a.getSize() != 0)
		{
			WriteCode(module, (void*)a.getCode(), a.getSize());
		}
		a.reset();

	} while (false);
	return action;
}

bool X86MemoryMonitor::NeedToHandle(Instruction& inst)
{
	bool need_handle = false;
	do
	{
		const auto& zinst    = inst.zinst;
		const auto  category = zinst.instruction.meta.category;

		const auto operand = GetExplicitMemoryOperand(
			zinst.operands, zinst.instruction.operand_count_visible);

		if (!operand && category != ZYDIS_CATEGORY_STRINGOP)
		{
			break;
		}

		if (m_flags & MonitorFlag::IgnoreCode)
		{
			if (category == ZYDIS_CATEGORY_CALL || category == ZYDIS_CATEGORY_UNCOND_BR)
			{
				break;
			}

			if (zinst.instruction.meta.branch_type != ZYDIS_BRANCH_TYPE_NONE)
			{
				break;
			}
		}

		if (m_flags & MonitorFlag::IgnoreStack && assembler_->IsRspRelative(inst))
		{
			break;
		}

		if (m_flags & MonitorFlag::IgnoreRipRelative && assembler_->IsRipRelative(nullptr, inst, 0))
		{
			break;
		}

		if (operand != nullptr && operand->mem.type == ZYDIS_MEMOP_TYPE_AGEN)
		{
			break;
		}

		if (zinst.instruction.mnemonic == ZYDIS_MNEMONIC_FSETPM287_NOP)
		{
			break;
		}

		// TODO:
		// Support fs and gs segment memory.
		if (operand != nullptr && 
			(operand->mem.segment == ZYDIS_REGISTER_GS || operand->mem.segment == ZYDIS_REGISTER_FS))
		{
			break;
		}

		need_handle  = true;
	}while(false);
	return need_handle;
}

