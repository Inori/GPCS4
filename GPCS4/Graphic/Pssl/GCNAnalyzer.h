#pragma once

#include "GCNInstruction.h"

namespace pssl
{;

struct GcnExportInfo
{
	uint32_t target = 0;  // will be cast to EXPInstruction::TGT
	std::vector<uint32_t> regIndices;
};

struct GcnAnalysisInfo
{
	std::vector<GcnExportInfo> expParams;
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
	void getExportInfo(GCNInstruction& ins);

private:
	GcnAnalysisInfo* m_analysis = nullptr;
};


}  // namespace pssl
