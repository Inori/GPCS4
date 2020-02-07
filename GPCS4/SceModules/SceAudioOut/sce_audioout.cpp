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
	auto audioOut = std::make_unique<AudioOut>(userId, type, index, len, freq, param);
	auto slotId = g_AudioSlots.GetEmptySlotIndex();
	g_AudioSlots.SetItemAt(slotId, std::move(audioOut));

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

// TODO: Remove this function when audio part is done.
void audioTest()
{
	const int numAudioChans    = 8;
	const int sampleSize       = 2;
	const uint32_t granularity = 256;
	const uint32_t sampleRate  = 48000;

	FILE* p = fopen("bgm.raw", "rb");

	sceAudioOutInit();
	auto ad = sceAudioOutOpen(0, 0, 0, granularity, sampleRate, SCE_AUDIO_OUT_PARAM_FORMAT_S16_8CH);

	bool endFlag = false;

	while (!endFlag)
	{
		uint8_t buffer[8192];
		auto count = fread(buffer, numAudioChans * sampleSize, granularity, p);
		if (count < 256)
		{
			size_t numBytes  = (256 - count) * numAudioChans * sampleSize;
			size_t startByte = count * numAudioChans * sampleSize;
			memset(buffer + startByte, 0, numBytes);
			endFlag = true;
		}

		sceAudioOutOutput(ad, buffer);
	}

	sceAudioOutOutput(ad, nullptr);
	sceAudioOutClose(ad);
}



