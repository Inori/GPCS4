#include "sce_appcontentutil.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs

LOG_CHANNEL(SceModules.SceAppContentUtil);


//////////////////////////////////////////////////////////////////////////
// library: libSceAppContent
//////////////////////////////////////////////////////////////////////////


int PS4API sceAppContentAddcontEnqueueDownload(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceAppContentAddcontMount(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceAppContentAddcontUnmount(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceAppContentGetAddcontInfoList(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceAppContentInitialize(const SceAppContentInitParam *initParam, SceAppContentBootParam *bootParam)
{
	LOG_SCE_TRACE("initParam %p bootParam %p", initParam, bootParam);
	bootParam->attr = 0;
	return SCE_OK;
}


int PS4API sceAppContentAppParamGetInt(SceAppContentAppParamId paramId, int32_t *value)
{
	LOG_SCE_TRACE("paramId %d value %p", paramId, value);
	int ret = SCE_OK;
	do 
	{
		if (paramId == SCE_APP_CONTENT_APPPARAM_ID_SKU_FLAG) 
		{
			*value = SCE_APP_CONTENT_APPPARAM_SKU_FLAG_FULL;
			break;
		}


		LOG_WARN("USER_DEFINED_PARAM dummy implemented");
		if (paramId == SCE_APP_CONTENT_APPPARAM_ID_USER_DEFINED_PARAM_1) 
		{
			*value = 1;
			break;
		}

		if (paramId == SCE_APP_CONTENT_APPPARAM_ID_USER_DEFINED_PARAM_2) 
		{
			*value = 0;
			break;
		}

		if (paramId == SCE_APP_CONTENT_APPPARAM_ID_USER_DEFINED_PARAM_3) 
		{
			*value = 0;
			break;
		}

		if (paramId == SCE_APP_CONTENT_APPPARAM_ID_USER_DEFINED_PARAM_4) 
		{
			*value = 56;
			break;
		}


		*value = 0;
		ret = SCE_APP_CONTENT_ERROR_PARAMETER;
	}
	while(false);
	return ret;
}

