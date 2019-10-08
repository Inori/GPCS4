#pragma once

#include "GCNInstruction.h"
#include <set>

namespace pssl
{;

struct GcnExportInfo
{
	uint32_t target = 0;  // will be cast to EXPInstruction::TGT
	std::vector<uint32_t> regIndices;
	bool isCompressed;
};

struct GcnAnalysisInfo
{
	std::vector<GcnExportInfo> expParams;
	uint32_t vinterpAttrCount;  // should be equal to the paired vertex shader's export params count
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
	void getVinterpInfo(GCNInstruction& ins);

private:
	GcnAnalysisInfo* m_analysis = nullptr;

	std::set<uint32_t> m_vinterpAttrSet;
};


}  // namespace pssl
