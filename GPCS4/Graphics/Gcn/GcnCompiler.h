#pragma once

#include "GcnCommon.h"
#include "GcnProgramInfo.h"
#include "Violet/VltRc.h"

#include <vector>

namespace sce::vlt
{
	class VltShader;
}  // namespace sce::vlt

namespace sce::gcn
{

	class GcnHeader;
	struct GcnShaderInstruction;
	struct GcnAnalysisInfo;

	/**
	 * \brief Shader recompiler
	 * 
	 * Recompile GCN instructions into Spir-V byte code.
	 * Produce VltShader for Violet.
	 */
	class GcnCompiler
	{
	public:
		GcnCompiler(
			const std::string&     fileName,
			const GcnProgramInfo&  programInfo,
			const GcnHeader&       header,
			const GcnAnalysisInfo& analysis);
		~GcnCompiler();

		/**
         * \brief Processes a single instruction
         * \param [in] ins The instruction
         */
		void processInstruction(
			const GcnShaderInstruction& ins);

		/**
         * \brief Finalizes the shader
         * \returns The final shader object
         */
		vlt::Rc<vlt::VltShader> finalize();

	private:
		GcnProgramInfo         m_programInfo;
		const GcnHeader*       m_header;
		const GcnAnalysisInfo* m_analysis;
	};

}  // namespace sce::gcn