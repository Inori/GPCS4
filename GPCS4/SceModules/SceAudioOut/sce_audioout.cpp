#include "sce_audioout.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceAudioOut
//////////////////////////////////////////////////////////////////////////

static bool init = false;
static int volume;

#define SCE_AUDIO_OUT_ERROR_ALREADY_INIT 0x8026000e
#define SCE_AUDIO_OUT_ERROR_NOT_INIT 0x8026000f
#define SCE_AUDIO_OUT_ERROR_INVALID_SIZE 0x80260006
#define SCE_AUDIO_OUT_ERROR_INVALID_VOLUME 0x80260009
#define SCE_AUDIO_OUT_ERROR_INVALID_FLAG 0x80260013
#define SCE_AUDIO_OUT_ERROR_INVALID_POINTER 0x80260004


int PS4API sceAudioOutInit(void)
{
	LOG_FIXME("Not implemented");

	if (init)
	{
		return SCE_AUDIO_OUT_ERROR_ALREADY_INIT;
	}

	init = true;
	return SCE_OK;
}


int PS4API sceAudioOutOpen(SceUserServiceUserId userId, int32_t type, int32_t index, uint32_t len, uint32_t freq, uint32_t param)
{
	if (!init)
	{
		return SCE_AUDIO_OUT_ERROR_NOT_INIT;
	}

	if (len < 0)
	{
		return SCE_AUDIO_OUT_ERROR_INVALID_SIZE;
	}

	LOG_SCE_DUMMY_IMPL();
	return 0x789;
}


int PS4API sceAudioOutClose(int32_t handle)
{
	LOG_FIXME("Not implemented");

	if (!init)
	{
		return SCE_AUDIO_OUT_ERROR_NOT_INIT;
	}

	return SCE_OK;
}

int PS4API sceAudioOutGetPortState(int32_t handle, SceAudioOutPortState *state)
{
	if (!init)
	{
		return SCE_AUDIO_OUT_ERROR_NOT_INIT;
	}

	LOG_SCE_DUMMY_IMPL();
	state->output = SCE_AUDIO_OUT_STATE_OUTPUT_CONNECTED_PRIMARY;
	state->channel = SCE_AUDIO_OUT_STATE_CHANNEL_2;
	state->volume = 50;
	state->rerouteCounter = 0;
	return SCE_OK;
}


int PS4API sceAudioOutOutput(void)
{
	LOG_FIXME("Not implemented");

	if (!init)
	{
		return SCE_AUDIO_OUT_ERROR_NOT_INIT;
	}

	return SCE_OK;
}


int PS4API sceAudioOutOutputs(void)
{
	LOG_FIXME("Not implemented");

	if (!init)
	{
		return SCE_AUDIO_OUT_ERROR_NOT_INIT;
	}

	return SCE_OK;
}


int PS4API sceAudioOutSetVolume(int32_t handle, int32_t flag, int32_t *vol)
{
	LOG_FIXME("Not implemented");
	
	// if initialization has not happened
	if (!init)
	{
		return SCE_AUDIO_OUT_ERROR_NOT_INIT;
	}

	// check for invalid flag
	if (flag < 0 || flag >= (1 << 8))
	{
		return SCE_AUDIO_OUT_ERROR_INVALID_FLAG;
	}

	// if pointer is invalid
	if (vol == nullptr)
	{
		return SCE_AUDIO_OUT_ERROR_INVALID_FLAG;
	}

	// check a maximum of 8 bits in flag
	for (int i = 0; i < 8; i++)
	{
		// if this bit is 1
		if (flag & (1 << i))
		{
			// if the volume is out of range
			if (vol[i] < 0 || vol[i] > 32768)
			{
				return SCE_AUDIO_OUT_ERROR_INVALID_VOLUME;
			}
		}
	}

	return SCE_OK;
}




