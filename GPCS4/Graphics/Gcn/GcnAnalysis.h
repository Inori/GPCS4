#pragma once

#include "GcnCommon.h"
#include "GcnInstructionIterator.h"
#include "GcnCompilerDefs.h"

#include <array>
#include <unordered_map>

namespace sce::gcn
{
	class GcnProgramInfo;
	class GcnCfgGenerator;
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
		GcnExportInfo       exportInfo;
		GcnControlFlowGraph controlFlow;

		// CFG blocks
		// Key is target address
		// TODO:
		// Remove this, use GcnCfgGenerator and controlFlow
		std::unordered_map<uint32_t, GcnCfgBlock> controlFlowBlocks; 
	};


	/**
	 * \brief GCN control flow graph pass
	 *
	 * Collect branch label information of a shader
	 * for the latter passes.
	 */
	class GcnCfgPass : public GcnInstructionIterator
	{
	public:
		GcnCfgPass(
			GcnCfgGenerator& cfgGenerator,
			GcnAnalysisInfo& analysis);
		virtual ~GcnCfgPass();

		/**
		 * \brief Processes a single instruction
		 * \param [in] ins The instruction
		 */
		virtual void processInstruction(
			const GcnShaderInstruction& ins) override;

	private:

		void analyzeBranch(
			const GcnShaderInstruction& ins);

	private:
		GcnAnalysisInfo* m_analysis = nullptr;
		GcnCfgGenerator* m_cfg      = nullptr;
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
			GcnCfgGenerator&      cfgGenerator,
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
		GcnAnalysisInfo* m_analysis;
		GcnCfgGenerator* m_cfg;
	};


}  // namespace sce::gcn
