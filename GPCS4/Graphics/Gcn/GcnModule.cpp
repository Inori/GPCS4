#include "GcnModule.h"

using namespace sce::vlt;

namespace sce::gcn
{
	GcnModule::GcnModule(
		const std::vector<uint8_t>& code)
	{
	}

	GcnModule::~GcnModule()
	{
	}

	Rc<VltShader> GcnModule::compile(
		const std::string& fileName) const
	{
	}

	void GcnModule::runAnalyzer(
		GcnAnalyzer& analyzer,
		GcnCodeSlice slice) const
	{
	}

	void GcnModule::runCompiler(
		GcnCompiler& compiler,
		GcnCodeSlice slice) const
	{
	}

}  // namespace sce::gcn