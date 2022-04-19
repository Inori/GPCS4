#pragma once

#include "GcnCommon.h"
#include "GcnDecoder.h"
#include "GcnProgramInfo.h"
#include "GcnHeader.h"
#include "GcnFetchShader.h"
#include "Violet/VltRc.h"

#include <vector>

namespace sce::vlt
{
	class VltShader;
}  // namespace sce::vlt

namespace sce::gcn
{
	class GcnInstructionIterator;
	union GcnShaderMeta;

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
         * \brief Compiles GCN shader to SPIR-V module
         * 
         * \returns The compiled shader object
         */
		vlt::Rc<vlt::VltShader> compile(
			const GcnShaderMeta& meta) const;

	private:

		void runInstructionIterator(
			GcnInstructionIterator* insIterator,
			GcnCodeSlice            slice) const;

	private:
		GcnProgramInfo           m_programInfo;
		GcnHeader                m_header;
		const uint8_t*           m_code;
		
		GcnShaderResourceTable m_resourceTable;
	};

}  // namespace sce::gcn