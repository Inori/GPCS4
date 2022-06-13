#pragma once

#include "GcnCommon.h"
#include "GcnDecoder.h"
#include "GcnProgramInfo.h"
#include "GcnHeader.h"
#include "GcnFetchShader.h"
#include "Violet/VltRc.h"

namespace sce::vlt
{
	class VltShader;
}  // namespace sce::vlt

namespace sce::gcn
{
	union GcnShaderMeta;
	class GcnAnalyzer;
	class GcnCompiler;

	class GcnModule
	{
	public:
		GcnModule(
			GcnProgramType type,
			const uint8_t* code);
		~GcnModule();

		/**
         * \brief Shader type
         * \returns Shader type
         */
		const GcnProgramInfo& programInfo() const
		{
			return m_programInfo;
		}

		/**
		 * \brief Get resources bound to the shader
		 */
		const GcnShaderResourceTable& 
			getResourceTable() const
		{
			return m_header.getShaderResourceTable();
		}

		/**
		 * \brief Get shader name
		 * 
		 * The name is generate from original 
		 * GCN shader binary hash and crc.
		 */
		std::string name() const
		{
			return m_programInfo.name() +
				   m_header.key().name();
		}


		/**
         * \brief Compiles GCN shader to SPIR-V module
         * 
         * \returns The compiled shader object
         */
		vlt::Rc<vlt::VltShader> compile(
			const GcnShaderMeta& meta) const;

	private:

		GcnInstructionList decodeShader(
			GcnCodeSlice& slice) const;

		void runAnalyzer(
			GcnAnalyzer&              analyzer,
			const GcnInstructionList& insList) const;

		void runCompiler(
			GcnCompiler&              compiler,
			const GcnInstructionList& insList) const;

		void dumpShader() const;

	private:
		GcnProgramInfo           m_programInfo;
		GcnHeader                m_header;
		const uint8_t*           m_code;
		
		GcnShaderResourceTable m_resourceTable;
	};

}  // namespace sce::gcn