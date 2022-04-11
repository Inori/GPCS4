#pragma once

#include "GcnCommon.h"
#include "GcnDecoder.h"
#include "GcnProgramInfo.h"

#include "Violet/VltRc.h"

#include <vector>

namespace sce::vlt
{
	class VltShader;
}  // namespace sce::vlt

namespace sce::gcn
{
	class GcnAnalyzer;
	class GcnCompiler;

	class GcnModule
	{
	public:
		GcnModule(
			const std::vector<uint8_t>& code);
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
         * \brief Compiles GCN shader to SPIR-V module
         * 
         * \param [in] fileName File name, will be added to
         *        the compiled SPIR-V for debugging purposes.
         * \returns The compiled shader object
         */
		vlt::Rc<vlt::VltShader> compile(
			const std::string& fileName) const;
	private:

		void runAnalyzer(
			GcnAnalyzer& analyzer,
			GcnCodeSlice slice) const;

		void runCompiler(
			GcnCompiler& compiler,
			GcnCodeSlice slice) const;

	private:
		GcnProgramInfo m_programInfo;
	};

}  // namespace sce::gcn