#pragma once

#define SCE_GNM_UNUSED(var) (void)var;

#define SCE_GNM_ERROR             LOG_ERR
#define SCE_GNM_ASSERT(expr)      LOG_ASSERT(expr, "Assert failed.")
#define SCE_GNM_ASSERT_MSG        LOG_ASSERT
#define SCE_GNM_ASSERT_MSG_RETURN LOG_ASSERT_RETURN
