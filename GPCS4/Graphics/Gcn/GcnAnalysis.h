#pragma once

#include "GcnCommon.h"
#include "GcnInstructionIterator.h"


namespace sce::gcn
{
	class GcnProgramInfo;
	struct GcnShaderInstruction;

	struct GcnAnalysisInfo
	{
		uint32_t placeHolder;
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
		GcnAnalysisInfo* m_analysis = nullptr;
	};


}  // namespace sce::gcn
