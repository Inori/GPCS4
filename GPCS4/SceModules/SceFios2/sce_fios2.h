/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceFios2
 *        library: libSceFios2
 *    
 */

#pragma once

#include "sce_module_common.h"
#include "sce_fios2_types.h"

extern const SCE_EXPORT_MODULE g_ExpModuleSceFios2;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceFios2
//////////////////////////////////////////////////////////////////////////

int PS4API sceFiosDateToComponents(void);


int PS4API sceFiosDeleteSync(void);


int PS4API sceFiosDirectoryCreateSync(void);


bool PS4API sceFiosDirectoryExistsSync(const SceFiosOpAttr *pAttr, const char *pPath);


int PS4API sceFiosFHCloseSync(void);


int PS4API sceFiosFHOpenSync(void);


int PS4API sceFiosFHReadSync(void);


int PS4API sceFiosFHSeek(void);


int PS4API sceFiosFHStatSync(void);


int PS4API sceFiosFHWriteSync(void);


bool PS4API sceFiosFileExistsSync(const SceFiosOpAttr *pAttr, const char *pPath);


int PS4API sceFiosFileGetSizeSync(void);


int PS4API sceFiosFileReadSync(void);


int PS4API sceFiosFileWriteSync(void);


int PS4API sceFiosStatSync(void);


int PS4API sceFiosDeallocatePassthruFH(void);


int PS4API sceFiosFilenoToFH(void);


int PS4API sceFiosFHToFileno(void);


int PS4API sceFiosIsValidHandle(void);


int PS4API sceFiosFHOpenWithModeSync(void);


int PS4API sceFiosRenameSync(void);