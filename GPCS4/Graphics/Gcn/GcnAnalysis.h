#pragma once

#include "GcnCommon.h"
#include "GcnInstructionIterator.h"
#include "GcnCompilerDefs.h"

#include <array>

namespace sce::gcn
{
	class GcnProgramInfo;
	struct GcnShaderInstruction;

	struct GcnExportInfo
	{
		uint32_t                                  paramCount;
		std::array<GcnRegMask, GcnMaxExportParam> params;
	};

	struct GcnAnalysisInfo
	{
		GcnExportInfo exportInfo;
	};

	/**
	 * \brief GCN shader analyzer
	 * 
	 * Pre-collect global information of a shader
	 * which is not possible to get when stepping a instruction.
     * The information will later be used by the actual compiler.
	 */
	class GcnAnalyzer : public GcnInstructionIterator
	{
	public:
		GcnAnalyzer(
			const GcnProgramInfo& programInfo,
			GcnAnalysisInfo&      analysis);
		virtual ~GcnAnalyzer();

		/**
         * \brief Processes a single instruction
         * \param [in] ins The instruction
         */
		virtual void processInstruction(
			const GcnShaderInstruction& ins) override;

	private:
		void analyzeInstruction(
			const GcnShaderInstruction& ins);

		void analyzeExp(
			const GcnShaderInstruction& ins);

	private:
		GcnAnalysisInfo* m_analysis = nullptr;
	};


}  // namespace sce::gcn
