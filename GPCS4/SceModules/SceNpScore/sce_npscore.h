/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceNpScore
 *        library: libSceNpScore
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceNpScore;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceNpScore
//////////////////////////////////////////////////////////////////////////

int PS4API sceNpScoreCreateNpTitleCtx(void);


int PS4API sceNpScoreCreateRequest(void);


int PS4API sceNpScoreDeleteNpTitleCtx(void);


int PS4API sceNpScoreDeleteRequest(void);


int PS4API sceNpScoreGetFriendsRankingAsync(void);


int PS4API sceNpScoreGetRankingByNpIdAsync(void);


int PS4API sceNpScoreGetRankingByRangeAsync(void);


int PS4API sceNpScorePollAsync(void);


int PS4API sceNpScoreRecordScoreAsync(void);


int PS4API sceNpScoreGetFriendsRanking(void);


int PS4API sceNpScoreGetRankingByNpId(void);


int PS4API sceNpScoreGetRankingByRange(void);




