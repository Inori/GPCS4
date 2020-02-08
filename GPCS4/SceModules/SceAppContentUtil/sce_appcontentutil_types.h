#pragma once

#include <GPCS4Types.h>


typedef uint32_t SceAppContentAppParamId;

typedef uint32_t SceAppContentMediaType;

typedef uint32_t SceAppContentBootAttribute;



struct SceAppContentInitParam 
{
	char reserved[32];
};


struct SceAppContentBootParam 
{
	char reserved1[4];
	SceAppContentBootAttribute attr;
	char reserved2[32];
};


#define SCE_APP_CONTENT_APPPARAM_ID_SKU_FLAG              0
#define SCE_APP_CONTENT_APPPARAM_ID_USER_DEFINED_PARAM_1  1
#define SCE_APP_CONTENT_APPPARAM_ID_USER_DEFINED_PARAM_2  2
#define SCE_APP_CONTENT_APPPARAM_ID_USER_DEFINED_PARAM_3  3
#define SCE_APP_CONTENT_APPPARAM_ID_USER_DEFINED_PARAM_4  4


#define SCE_APP_CONTENT_APPPARAM_SKU_FLAG_TRIAL 1
#define SCE_APP_CONTENT_APPPARAM_SKU_FLAG_FULL  3
