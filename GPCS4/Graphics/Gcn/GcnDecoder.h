#pragma once

#include "GcnInstruction.h"

namespace sce::gcn
{

	class GcnCodeSlice
	{

	public:
		GcnCodeSlice(
			const uint32_t* ptr,
			const uint32_t* end) :
			m_ptr(ptr),
			m_end(end)
		{
		}
		
		GcnCodeSlice(
			const GcnCodeSlice& other) = default;

		~GcnCodeSlice() = default;

		uint32_t at(uint32_t id) const;

		uint32_t readu32();
		uint64_t readu64();

		bool atEnd() const
		{
			return m_ptr == m_end;
		}

	private:
		const uint32_t* m_ptr = nullptr;
		const uint32_t* m_end = nullptr;
	};

	class GcnDecodeContext
	{
		enum GcnOperandFieldRange
		{
			ScalarGPRMin         = 0,
			ScalarGPRMax         = 103,
			SignedConstIntPosMin = 129,
			SignedConstIntPosMax = 192,
			SignedConstIntNegMin = 193,
			SignedConstIntNegMax = 208,
			VectorGPRMin         = 256,
			VectorGPRMax         = 511
		};

	public:
		GcnDecodeContext();
		~GcnDecodeContext();

		const GcnShaderInstruction& getInstruction() const
		{
			return m_instruction;
		}

		void decodeInstruction(GcnCodeSlice& code);

	private:
		GcnInstEncoding getInstructionEncoding(uint32_t token);
		uint32_t        getEncodingLength(GcnInstEncoding encoding);
		uint32_t        getOpMapOffset(GcnInstEncoding encoding);
		uint32_t        mapEncodingOp(GcnInstEncoding encoding, GcnOpcode opcode);
		bool            hasAdditionalLiteral(GcnInstEncoding encoding, uint32_t opcode);
		void            updateInstructionMeta(GcnInstEncoding encoding);

		GcnOperandField getOperandField(uint32_t code);

		void decodeInstruction32(GcnInstEncoding encoding, GcnCodeSlice& code);
		void decodeInstruction64(GcnInstEncoding encoding, GcnCodeSlice& code);
		void decodeLiteralConstant(GcnInstEncoding encoding, GcnCodeSlice& code);

		// 32 bits encodings
		void decodeInstructionSOP1(uint32_t hexInstruction);
		void decodeInstructionSOPP(uint32_t hexInstruction);
		void decodeInstructionSOPC(uint32_t hexInstruction);
		void decodeInstructionSOPK(uint32_t hexInstruction);
		void decodeInstructionSOP2(uint32_t hexInstruction);
		void decodeInstructionVOP1(uint32_t hexInstruction);
		void decodeInstructionVOPC(uint32_t hexInstruction);
		void decodeInstructionVOP2(uint32_t hexInstruction);
		void decodeInstructionSMRD(uint32_t hexInstruction);
		void decodeInstructionVINTRP(uint32_t hexInstruction);
		// 64 bits encodings
		void decodeInstructionVOP3(uint64_t hexInstruction);
		void decodeInstructionMUBUF(uint64_t hexInstruction);
		void decodeInstructionMTBUF(uint64_t hexInstruction);
		void decodeInstructionMIMG(uint64_t hexInstruction);
		void decodeInstructionDS(uint64_t hexInstruction);
		void decodeInstructionEXP(uint64_t hexInstruction);

	private:
		GcnShaderInstruction m_instruction;
	};


	/**
	 * \brief Convenient function to convert general instruction into specific encodings
	 */

	using GcnInstructionVariant =
		std::variant<
			GcnShaderInstSOP1,
			GcnShaderInstSOP2,
			GcnShaderInstSOPK,
			GcnShaderInstSOPC,
			GcnShaderInstSOPP,
			GcnShaderInstVOP1,
			GcnShaderInstVOP2,
			GcnShaderInstVOP3,
			GcnShaderInstVOPC,
			GcnShaderInstSMRD,
			GcnShaderInstMUBUF,
			GcnShaderInstMTBUF,
			GcnShaderInstMIMG,
			GcnShaderInstVINTRP,
			GcnShaderInstDS,
			GcnShaderInstEXP>;

	// Do not use this directly, use gcnInstructionAs
	GcnInstructionVariant gcnInstructionConvert(const GcnShaderInstruction& ins);

	template <typename InsType>
	static inline const InsType gcnInstructionAs(
		const GcnShaderInstruction& ins)
	{
		return std::get<InsType>(gcnInstructionConvert(inst));
	}

}  // namespace sce::gcn
