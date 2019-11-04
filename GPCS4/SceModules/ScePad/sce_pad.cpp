#include "sce_pad.h"
#include "MapSlot.h"

// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



struct PadHandleItem
{
	SceUserServiceUserId userId;  
	int32_t type;
	int32_t index;
	bool portState;
};

bool isPadPortClose(PadHandleItem& item)
{
	return (item.portState == false);
}

bool isEqualPad(const PadHandleItem& lpad, const PadHandleItem& rpad)
{
	return lpad.userId == rpad.userId && lpad.type == rpad.type && lpad.index == rpad.index;
}

MapSlot<PadHandleItem, decltype(isPadPortClose)> g_padHandleSlots(SCE_PAD_HANDLE_MAX, isPadPortClose);

//////////////////////////////////////////////////////////////////////////
// library: libScePad
//////////////////////////////////////////////////////////////////////////

int PS4API scePadClose(int32_t handle)
{
	LOG_SCE_TRACE("handle %h", handle);
	int ret = 0;
	do
	{
		PadHandleItem& item = g_padHandleSlots[handle];
		if (item.portState == false)
		{
			ret = SCE_PAD_ERROR_NO_HANDLE;
			break;
		}

		g_padHandleSlots[handle] = { 0, 0, 0, false };
		ret = SCE_OK;
	} while (false);

	return ret;
}


int PS4API scePadGetControllerInformation(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePadGetHandle(SceUserServiceUserId userId, int32_t type, int32_t index)
{
	LOG_SCE_TRACE("userId %u type %t index %i", userId, type, index);
	int ret = 0;
	PadHandleItem item  { userId, type, index, true };

	ret = g_padHandleSlots.GetItemIndex(item, isEqualPad);
	if (ret == 0) 
	{
		ret = SCE_PAD_ERROR_NO_HANDLE;
	}

	return ret;
}


int PS4API scePadInit(void)
{
	LOG_SCE_TRACE("");
	return SCE_OK;
}


int PS4API scePadOpen(SceUserServiceUserId userId, int32_t type, int32_t index, const ScePadOpenParam* pParam)
{
	LOG_SCE_TRACE("userId %u type %t index %i", userId, type, index);
	int ret = 0;
    PadHandleItem item  { userId, type, index, true };
	do 
	{
		ret = g_padHandleSlots.GetItemIndex(item, isEqualPad);
		if (ret > 0) 
		{
			ret = SCE_PAD_ERROR_ALREADY_OPENED;
			break;
		}
		ret = g_padHandleSlots.GetEmptySlotIndex();

		g_padHandleSlots.SetItemAt(ret, item);

	} while (false);

	return ret;
}


int PS4API scePadReadState(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePadResetLightBar(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePadSetLightBar(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePadSetVibration(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePadRead(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}




