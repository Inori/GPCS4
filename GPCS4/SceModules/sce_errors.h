#pragma once

#define SCE_ERROR_ERROR_FLAG             0x80000000 
#define SCE_ERROR_MAKE_ERROR(fac, sts) (SCE_ERROR_ERROR_FLAG | ((fac) << 16) | (sts))

#define SCE_OK            0
#define SCE_ERROR_UNKNOWN SCE_ERROR_MAKE_ERROR(0xFF, 0xFF)


#include "sce_kernel_error.h"
#include "sce_gnm_error.h"
#include "SceIme/sce_ime_error.h"
#include "SceUserService/sce_userservice_error.h"
#include "SceSystemService/sce_systemservice_error.h"
