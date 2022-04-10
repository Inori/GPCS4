#pragma once

#include "GcnCommon.h"


namespace sce::gcn
{
	struct GcnShaderInstruction;

	struct GcnAnalysisInfo
	{
		uint32_t placeHoder;
	};

	class GcnAnalyzer
	{
	public:
		GcnAnalyzer(
			GcnAnalysisInfo& analysis);
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
