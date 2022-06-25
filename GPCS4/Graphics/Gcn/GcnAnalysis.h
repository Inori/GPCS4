#pragma once

#include "GcnCommon.h"
#include "GcnCompilerDefs.h"

#include <array>
#include <unordered_set>
#include <unordered_map>

namespace sce::gcn
{
	class GcnProgramInfo;
	struct GcnShaderInstruction;

	struct GcnExportInfo
	{
		uint32_t                                  paramCount = 0;
		std::array<GcnRegMask, GcnMaxExportParam> params;
		uint32_t                                  mrtCount = 0;
		std::array<GcnRegMask, GcnMaxExportParam> mrts;
	};

	struct GcnAnalysisInfo
	{
		GcnExportInfo exportInfo;

		bool hasComputeLane = false;
	};

	/**
	 * \brief GCN shader analyzer
	 * 
	 * Pre-collect global information of a shader
	 * which is not possible to get when stepping a instruction.
     * The information will later be used by the actual compiler.
	 */
	class GcnAnalyzer
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
		void processInstruction(
			const GcnShaderInstruction& ins);

	private:
		void analyzeInstruction(
			const GcnShaderInstruction& ins);

		void analyzeExp(
			const GcnShaderInstruction& ins);

		void analyzeLane(
			const GcnShaderInstruction& ins);

	private:
		GcnAnalysisInfo*      m_analysis;
		const GcnProgramInfo& m_programInfo;
	};


}  // namespace sce::gcn
