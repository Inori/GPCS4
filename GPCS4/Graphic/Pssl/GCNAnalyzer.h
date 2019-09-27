#pragma once

#include "GCNInstruction.h"

namespace pssl
{;

struct GcnAnalysisInfo
{
	bool useM0Debug = false;
};

// Used for collecting global information
// of a shader module which is not convenient
// for per instruction process in compiler.

class GCNAnalyzer
{
public:
	GCNAnalyzer(GcnAnalysisInfo& analysis);
	~GCNAnalyzer();

	void processInstruction(GCNInstruction& ins);

private:
	GcnAnalysisInfo* m_analysis = nullptr;
};


}  // namespace pssl
