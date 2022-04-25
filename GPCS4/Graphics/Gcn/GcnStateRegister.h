#pragma once

#include "GcnCommon.h"
#include "GcnCompilerDefs.h"

namespace sce::gcn
{
	class GcnCompiler;

	/**
	 * \brief Abstracts a 64 bits hardware state register
	 */
	class GcnStateRegister
	{
	public:
		GcnStateRegister(
			GcnCompiler* compiler,
			const char*  name);

		~GcnStateRegister();

		/**
		 * \brief Set a value.
		 */
		void init(uint64_t value);

		/**
		 * \brief Load value and return the result
		 * 
		 * The mask can be used to specify dst width,
		 * either one gpr or two gpr.
		 * e.g:
		 * mask == 1 -> s0
		 * mask == 3 -> s[0:1]
		 */
		GcnRegisterValuePair emitLoad(
			const GcnRegMask& mask);

		/**
		 * \brief Store value.
		 * 
		 * The mask can be used to specify src width,
		 * either one gpr or two gpr.
		 * e.g:
		 * mask == 1 -> s0
		 * mask == 3 -> s[0:1]
		 */
		void emitStore(
			const GcnRegisterValuePair& value,
			const GcnRegMask&           mask);

	private:
		
		void create();

	private:
		GcnCompiler*       m_compiler;
		std::string        m_name;
		bool               m_created = false;
		GcnRegisterPointer m_low;
		GcnRegisterPointer m_high;
	};


	/**
	 * \brief Hardware state registers
	 * 
	 * Will be updated after specific instruction execution.
	 */
	struct GcnStateRegisters
	{
		GcnStateRegister exec;
		GcnStateRegister vcc;

		GcnRegisterPointer m0;
		GcnRegisterPointer scc;
		GcnRegisterPointer vccz;
		GcnRegisterPointer execz;
	};
}  // namespace sce::gcn