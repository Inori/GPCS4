#pragma once

#include "GcnCommon.h"


namespace sce::gcn
{
	class GcnProgramInfo;
	struct GcnShaderInstruction;

	struct GcnAnalysisInfo
	{
		uint32_t placeHoder;
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
		~GcnAnalyzer();

		/**
         * \brief Processes a single instruction
         * \param [in] ins The instruction
         */
		void processInstruction(
			const GcnShaderInstruction& ins);

	private:
		GcnAnalysisInfo* m_analysis = nullptr;
	};


}  // namespace sce::gcn
