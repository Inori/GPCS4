#include "sce_npscore.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceNpScore_libSceNpScore_FunctionTable[] =
{
	{ 0x2A7340D53120B412, "sceNpScoreCreateNpTitleCtx", (void*)sceNpScoreCreateNpTitleCtx },
	{ 0x1969d640d5d91f93, "sceNpScoreCreateNpTitleCtxA", (void*)sceNpScoreCreateNpTitleCtxA },
	{ 0x816F2ACA362B51B9, "sceNpScoreCreateRequest", (void*)sceNpScoreCreateRequest },
	{ 0x1B4A44F91342C1F9, "sceNpScoreDeleteNpTitleCtx", (void*)sceNpScoreDeleteNpTitleCtx },
	{ 0x74AF3F4A061FEABE, "sceNpScoreDeleteRequest", (void*)sceNpScoreDeleteRequest },
	{ 0xED2B8C52537B43A2, "sceNpScoreGetFriendsRankingAsync", (void*)sceNpScoreGetFriendsRankingAsync },
	{ 0x45DDBB76A505655F, "sceNpScoreGetRankingByNpIdAsync", (void*)sceNpScoreGetRankingByNpIdAsync },
	{ 0xAD2866A971F0A101, "sceNpScoreGetRankingByRangeAsync", (void*)sceNpScoreGetRankingByRangeAsync },
	{ 0x9B50DF351B2D9124, "sceNpScorePollAsync", (void*)sceNpScorePollAsync },
	{ 0x00D26CB0FCF7998D, "sceNpScoreRecordScoreAsync", (void*)sceNpScoreRecordScoreAsync },
	{ 0xF24B88CD4C3ABAD4, "sceNpScoreGetFriendsRanking", (void*)sceNpScoreGetFriendsRanking },
	{ 0xF66644828884ABA6, "sceNpScoreGetRankingByNpId", (void*)sceNpScoreGetRankingByNpId },
	{ 0x2811F10E3CA4FE30, "sceNpScoreGetRankingByRange", (void*)sceNpScoreGetRankingByRange },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceNpScore_LibTable[] =
{
	{ "libSceNpScore", g_pSceNpScore_libSceNpScore_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceNpScore =
{
	"libSceNpScore",
	g_pSceNpScore_LibTable
};


