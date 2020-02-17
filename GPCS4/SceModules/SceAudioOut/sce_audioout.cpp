#include "sce_audioout.h"
#include "AudioOut.h"
#include "MapSlot.h"

#include <memory>


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs

LOG_CHANNEL(SceModules.SceAudioOut);

constexpr int MAX_AUDIO_SLOTS = 20;
static MapSlot<std::unique_ptr<AudioOut>> g_AudioSlots {MAX_AUDIO_SLOTS};
//////////////////////////////////////////////////////////////////////////
// library: libSceAudioOut
//////////////////////////////////////////////////////////////////////////

int PS4API sceAudioOutInit(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int32_t PS4API sceAudioOutOpen(SceUserServiceUserId userId, int32_t type, int32_t index, uint32_t len, uint32_t freq, uint32_t param)
{
	LOG_DEBUG("sceAudioOutOpen() userId: %d type: %d index: %d len: %d freq: %d param: %d",
			  userId,
			  type,
			  index,
			  len,
			  freq,
			  param);

	int32_t slotId = 0;

	do
	{
		auto audioOut = std::make_unique<AudioOut>(userId, type, index, len, freq, param);

		auto err = audioOut->getLastError();
		if (err != 0)
		{
			slotId = SCE_AUDIO_OUT_ERROR_TRANS_EVENT;
			break;
		}

		slotId = g_AudioSlots.GetEmptySlotIndex();
		g_AudioSlots.SetItemAt(slotId, std::move(audioOut));

	} while (false);

	return slotId;
}


int32_t PS4API sceAudioOutClose(int32_t handle)
{
	auto& audioOut = g_AudioSlots.GetItemAt(handle);
	audioOut->audioClose();
	audioOut.reset(nullptr);

	return SCE_OK;
}


int PS4API sceAudioOutGetPortState(int32_t handle, SceAudioOutPortState *state)
{
	LOG_SCE_DUMMY_IMPL();
	state->output = SCE_AUDIO_OUT_STATE_OUTPUT_CONNECTED_PRIMARY;
	state->channel = SCE_AUDIO_OUT_STATE_CHANNEL_2;
	state->volume = 50;
	state->rerouteCounter = 0;
	return SCE_OK;
}


int32_t PS4API sceAudioOutOutput(int32_t handle, const void *ptr)
{
	int rc         = SCE_OK;

	auto& audioOut = g_AudioSlots.GetItemAt(handle);
	rc = audioOut->audioOutput(ptr);
	if (rc != 0)
	{
		rc = SCE_AUDIO_OUT_ERROR_TRANS_EVENT;
	}

	return rc;
}


int PS4API sceAudioOutOutputs(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceAudioOutSetVolume(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceAudioOutInitIpmiGetSession(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


