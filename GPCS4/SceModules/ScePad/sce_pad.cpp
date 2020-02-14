#include "sce_pad.h"
#include "MapSlot.h"
#include "ScePad.h"

// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs

LOG_CHANNEL(SceModules.ScePad);

//////////////////////////////////////////////////////////////////////////
// library: libScePad
//////////////////////////////////////////////////////////////////////////

#define SCE_PAD_COUNT_MAX (16 * 4 + 1) //16 users, 4 ports per user + 1 system user
MapSlot<ScePad*> g_padSlot(SCE_PAD_COUNT_MAX);

//////////////////////////////////////////////////////////////////////////

int PS4API scePadGetControllerInformation(int32_t handle, ScePadControllerInformation *pInfo)
{
	LOG_SCE_TRACE("handle %d info %p", handle, pInfo);
	return g_padSlot[handle]->getInformation(pInfo);
}


int PS4API scePadGetHandle(SceUserServiceUserId userId, int32_t type, int32_t index)
{
	LOG_SCE_TRACE("uid %d type %d index %d", userId, type, index);
	int port = SCE_PAD_ERROR_NO_HANDLE;
	do
	{
		uint32_t padCount = g_padSlot.Size();
		for (uint32_t i = 0; i != padCount; ++i) 
		{
			auto& pad = g_padSlot[i];
			if (!pad)
			{
				continue;
			}
			if (pad->type() == type && pad->userId() == userId)
			{
				port = i;
				break;
			}
		}
	} while (false);
	return port;
}


int PS4API scePadInit(void)
{
	LOG_SCE_TRACE("nothing to do.");
	return SCE_OK;
}


int PS4API scePadOpen(SceUserServiceUserId userId, int32_t type, int32_t index, const ScePadOpenParam* pParam)
{
	LOG_SCE_TRACE("uid %d type %d index %d", userId, type, index);
	int port = SCE_PAD_ERROR_FATAL;
	do 
	{
		uint32_t padCount = g_padSlot.Size();
		bool opended = false;
		for (uint32_t i = 0; i != padCount; ++i)
		{
			auto& pad = g_padSlot[i];
			if (!pad)
			{
				continue;
			}
			if (pad->type() == type && pad->userId() == userId)
			{
				opended = true;
				break;
			}
		}

		if (opended)
		{
			port = SCE_PAD_ERROR_ALREADY_OPENED;
			break;
		}

		uint32_t handle = g_padSlot.GetEmptySlotIndex();
		ScePad* pad = new ScePad(userId, type, index);
		g_padSlot[handle] = pad;

		port = handle;
	} while (false);
	return port;
}


int PS4API scePadClose(int32_t handle)
{
	LOG_SCE_TRACE("handle %d", handle);
	int err = SCE_PAD_ERROR_INVALID_HANDLE;
	do 
	{
		auto& pad = g_padSlot[handle];
		if (!pad)
		{
			break;
		}

		delete pad;
		g_padSlot[handle] = nullptr;

		err = SCE_OK;
	} while (false);
	return err;
}


int PS4API scePadRead(int32_t handle, ScePadData *pData, int32_t num)
{
	LOG_SCE_TRACE("handle %d", handle);

	return g_padSlot[handle]->read(pData, num);
}

int PS4API scePadSetTiltCorrectionState(int32_t handle, bool bEnable)
{
	LOG_SCE_DUMMY_IMPL();
	return SCE_OK;
}


int PS4API scePadReadState(int32_t handle, ScePadData *pData)
{
	LOG_SCE_TRACE("handle %d", handle);

	return g_padSlot[handle]->readState(pData);
}


int PS4API scePadSetLightBar(int32_t handle, const ScePadLightBarParam *pParam)
{
	LOG_SCE_TRACE("handle %d", handle);
	return g_padSlot[handle]->setLightBar(handle, pParam);
}


int PS4API scePadResetLightBar(int32_t handle)
{
	LOG_SCE_TRACE("handle %d", handle);
	return g_padSlot[handle]->resetLightBar(handle);
}

int PS4API scePadSetMotionSensorState(void) 
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePadSetVibration(int32_t handle, const ScePadVibrationParam *pParam)
{
	LOG_SCE_TRACE("handle %d", handle);
	return g_padSlot[handle]->setVibration(handle, pParam);
}




