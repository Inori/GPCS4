#pragma once

#include "GCNInstruction.h"
#include "GCNInstructionIterator.h"
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

	// should be equal to the paired vertex shader's export params count
	uint32_t vinterpAttrCount; 


};

// Used for collecting global information
// of a shader module which is not convenient
// for per instruction process in compiler.

class GCNAnalyzer : public GCNInstructionIterator
{
public:
	GCNAnalyzer(GcnAnalysisInfo& analysis);
	virtual ~GCNAnalyzer();

	virtual void processInstruction(GCNInstruction& ins);

private:
	void getExportInfo(GCNInstruction& ins);
	void getVinterpInfo(GCNInstruction& ins);

private:
	GcnAnalysisInfo* m_analysis = nullptr;

	std::set<uint32_t> m_vinterpAttrSet;
};


}  // namespace pssl
