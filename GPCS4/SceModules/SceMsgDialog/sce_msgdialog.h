/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceMsgDialog
 *        library: libSceMsgDialog
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceMsgDialog;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceMsgDialog
//////////////////////////////////////////////////////////////////////////

struct SceMsgDialogResult {
	int32_t mode;
	int32_t result;
	int32_t buttonId;
	char reserved[32];
};

// not sure if all of these data types are right
struct SceMsgDialogParam {
	int baseParam;
	size_t size;
	int mode;
	int : 32;
	void *userMsgParam;
	void *progBarParam;
	void *sysMsgParam;
	int userId;
	char reserved[40];
	int : 32;
};

int PS4API sceMsgDialogGetResult(SceMsgDialogResult* result);


int PS4API sceMsgDialogGetStatus(void);


int PS4API sceMsgDialogInitialize(void);


int PS4API sceMsgDialogOpen(const SceMsgDialogParam *param);


int PS4API sceMsgDialogTerminate(void);


int PS4API sceMsgDialogUpdateStatus(void);




