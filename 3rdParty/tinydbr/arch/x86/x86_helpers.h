/*
Copyright 2020 Google LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

https ://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef ARCH_X86_X86_HELPERS_H
#define ARCH_X86_X86_HELPERS_H

#include <Zydis/Zydis.h>
#include <xbyak.h>



ZyanU32       GetRegisterWidth(ZydisRegister reg);
ZydisRegister GetFullSizeRegister(
    ZydisRegister reg, int child_ptr_size = sizeof(void*));
ZydisRegister GetReducedVectorRegister(ZydisRegister reg);
ZydisRegister GetFullSizeVectorRegister(ZydisRegister reg, bool hw_support = true);
bool          GetFreeRegister(
			 const ZydisDecodedInstruction& inst,
			 const ZydisDecodedOperand*     operands,
			 size_t                         count,
			 std::vector<ZydisRegister>&    reg_list);
ZydisRegister GetFreeRegister(
    const ZydisDecodedInstruction& inst, 
    const ZydisDecodedOperand* operands);
std::pair<ZydisRegister, ZydisRegister> GetFreeRegister2(
	const ZydisDecodedInstruction& inst,
	const ZydisDecodedOperand*     operands);
ZydisRegister GetFreeRegisterSSE(
	const ZydisDecodedInstruction& inst,
	const ZydisDecodedOperand*     operands);
ZydisRegister GetLow8BitRegister(ZydisRegister reg);
ZydisRegister GetNBitRegister(ZydisRegister reg, size_t nbits);
ZydisRegister GetNBitVectorRegister(ZydisRegister reg, size_t nbits);
size_t        GetMMRegisterSize(ZydisRegister reg);
bool          IsGeneralPurposeRegister(ZydisRegister reg);
bool          IsVectorRegister(ZydisRegister reg);
bool          Is64BitGPRRegister(ZydisRegister reg);
bool          IsHigh8BitRegister(ZydisRegister reg);
bool          IsSetCCInstruction(ZydisMnemonic mnemonic);
void          DetectAvxSupport(bool& support_avx, bool& support_avx512);

template <typename T>
const T& ZydisRegToXbyakReg(ZydisRegister reg)
{
	using namespace Xbyak;
	using namespace Xbyak::util;

	static const Xbyak::Reg table[] = {
		Reg(), al, cl, dl, bl, ah, ch, dh, bh, spl, bpl, sil, dil, r8b, r9b, r10b, r11b, r12b, r13b, r14b, r15b,
		ax, cx, dx, bx, sp, bp, si, di, r8w, r9w, r10w, r11w, r12w, r13w, r14w, r15w,
		eax, ecx, edx, ebx, esp, ebp, esi, edi, r8d, r9d, r10d, r11d, r12d, r13d, r14d, r15d,
		rax, rcx, rdx, rbx, rsp, rbp, rsi, rdi, r8, r9, r10, r11, r12, r13, r14, r15,
		st0, st1, st2, st3, st4, st5, st6, st7, Reg(), Reg(), Reg(),
		mm0, mm1, mm2, mm3, mm4, mm5, mm6, mm7,
		xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8, xmm9, xmm10, xmm11, xmm12, xmm13, xmm14, xmm15, xmm16, xmm17, xmm18, xmm19, xmm20, xmm21, xmm22, xmm23, xmm24, xmm25, xmm26, xmm27, xmm28, xmm29, xmm30, xmm31,
		ymm0, ymm1, ymm2, ymm3, ymm4, ymm5, ymm6, ymm7, ymm8, ymm9, ymm10, ymm11, ymm12, ymm13, ymm14, ymm15, ymm16, ymm17, ymm18, ymm19, ymm20, ymm21, ymm22, ymm23, ymm24, ymm25, ymm26, ymm27, ymm28, ymm29, ymm30, ymm31,
		zmm0, zmm1, zmm2, zmm3, zmm4, zmm5, zmm6, zmm7, zmm8, zmm9, zmm10, zmm11, zmm12, zmm13, zmm14, zmm15, zmm16, zmm17, zmm18, zmm19, zmm20, zmm21, zmm22, zmm23, zmm24, zmm25, zmm26, zmm27, zmm28, zmm29, zmm30, zmm31,
		tmm0, tmm1, tmm2, tmm3, tmm4, tmm5, tmm6, tmm7,
		Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(),
		Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(),
		Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(),
		Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(), Reg(),
		k0, k1, k2, k3, k4, k5, k6, k7, bnd0, bnd1, bnd2, bnd3,
		Reg(), Reg(), Reg(), Reg(), Reg(), Reg()
	};

	return *(T*)&table[reg];
}

const Xbyak::Operand& ZydisRegToXbyakReg(ZydisRegister reg);


uint32_t Pushaq(ZydisMachineMode mmode, unsigned char* encoded, size_t encoded_size);
uint32_t Popaq(ZydisMachineMode mmode, unsigned char* encoded, size_t encoded_size);
uint32_t MovReg(ZydisMachineMode mmode, ZydisRegister dst, const ZydisDecodedOperand& src, 
    unsigned char* encoded, size_t encoded_size);
uint32_t MovRegAVX(ZydisMachineMode mmode, ZydisRegister dst, const ZydisDecodedOperand& src, 
    unsigned char* encoded, size_t encoded_size);
uint32_t MovStackAVX(
	ZydisMachineMode mmode, size_t stack_offset, 
    ZydisRegister src, bool is_save,
    unsigned char* encoded, size_t encoded_size);
uint32_t LeaReg(ZydisMachineMode mmode, ZydisRegister dst, const ZydisDecodedOperand& src, 
    size_t address_width, unsigned char* encoded, size_t encoded_size);

size_t GetExplicitMemoryOperandCount(
    const ZydisDecodedOperand* operands, size_t count);

const ZydisDecodedOperand* GetExplicitMemoryOperand(
	const ZydisDecodedOperand* operands, size_t count, size_t* index = nullptr);



#if 0
uint32_t Push(xed_state_t* dstate, ZydisRegister r, unsigned char* encoded, size_t encoded_size);
uint32_t Pop(xed_state_t* dstate, ZydisRegister r, unsigned char* encoded, size_t encoded_size);

uint32_t Mov(xed_state_t* dstate, uint32_t operand_width, ZydisRegister base_reg, 
    int32_t displacement, ZydisRegister r2, unsigned char* encoded,
             size_t encoded_size);

uint32_t Lzcnt(xed_state_t* dstate, uint32_t operand_width, ZydisRegister dest_reg, ZydisRegister src_reg, unsigned char* encoded, size_t encoded_size);

uint32_t CmpImm8(xed_state_t* dstate, uint32_t operand_width, ZydisRegister dest_reg, uint64_t imm, unsigned char* encoded, size_t encoded_size);



void CopyOperandFromInstruction(xed_decoded_inst_t *src,
                                xed_encoder_request_t *dest,
                                xed_operand_enum_t src_operand_name,
                                xed_operand_enum_t dest_operand_name,
                                int dest_operand_index,
                                size_t stack_offset);

uint32_t GetInstructionLength(ZydisEncoderRequest* inst);

void FixRipDisplacement(ZydisEncoderRequest* inst,
						size_t               mem_address,
						size_t               fixed_instruction_address);
#endif
#endif  // ARCH_X86_X86_HELPERS_H
