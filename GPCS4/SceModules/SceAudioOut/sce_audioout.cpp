#include "sce_audioout.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceAudioOut
//////////////////////////////////////////////////////////////////////////

static bool g_init = false;

#define SCE_AUDIO_OUT_ERROR_ALREADY_INIT 0x8026000e
#define SCE_AUDIO_OUT_ERROR_NOT_INIT 0x8026000f
#define SCE_AUDIO_OUT_ERROR_INVALID_SIZE 0x80260006
#define SCE_AUDIO_OUT_ERROR_INVALID_VOLUME 0x80260009
#define SCE_AUDIO_OUT_ERROR_INVALID_FLAG 0x80260013
#define SCE_AUDIO_OUT_ERROR_INVALID_POINTER 0x80260004


int PS4API sceAudioOutInit(void)
{
	LOG_SCE_DUMMY_IMPL();

	int returnVal = -1;

	if (g_init)
	{
		returnVal = SCE_AUDIO_OUT_ERROR_ALREADY_INIT;
	}

	else
	{
		g_init = true;
		returnVal = SCE_OK;
	}

	return returnVal;
}


int PS4API sceAudioOutOpen(SceUserServiceUserId userId, int32_t type, int32_t index, uint32_t len, uint32_t freq, uint32_t param)
{
	LOG_SCE_DUMMY_IMPL();

	int returnVal = -1;

	if (!g_init)
	{
		returnVal = SCE_AUDIO_OUT_ERROR_NOT_INIT;
	}

	else if (len < 0)
	{
		returnVal = SCE_AUDIO_OUT_ERROR_INVALID_SIZE;
	}

	else
	{
		returnVal = 0x789;
	}

	return returnVal;
}


int PS4API sceAudioOutClose(int32_t handle)
{
	LOG_FIXME("Not implemented");

	int returnVal = -1;

	if (!g_init)
	{
		returnVal = SCE_AUDIO_OUT_ERROR_NOT_INIT;
	}

	else
	{
		g_init = false;
		returnVal = SCE_OK;
	}
}

int PS4API sceAudioOutGetPortState(int32_t handle, SceAudioOutPortState *state)
{
	LOG_SCE_DUMMY_IMPL();

	int returnVal = -1;

	if (!g_init)
	{
		returnVal = SCE_AUDIO_OUT_ERROR_NOT_INIT;
	}

	else
	{
		state->output = SCE_AUDIO_OUT_STATE_OUTPUT_CONNECTED_PRIMARY;
		state->channel = SCE_AUDIO_OUT_STATE_CHANNEL_2;
		state->volume = 50;
		state->rerouteCounter = 0;
		returnVal = SCE_OK;
	}

	return returnVal;
}


int PS4API sceAudioOutOutput(void)
{
	LOG_FIXME("Not implemented");

	int returnVal = -1;

	if (!g_init)
	{
		returnVal = SCE_AUDIO_OUT_ERROR_NOT_INIT;
	}

	else
	{
		returnVal = SCE_OK;
	}

	return returnVal;
}


int PS4API sceAudioOutOutputs(void)
{
	LOG_FIXME("Not implemented");

	int returnVal = -1;

	if (!g_init)
	{
		returnVal = SCE_AUDIO_OUT_ERROR_NOT_INIT;
	}

	else
	{
		returnVal = SCE_OK;
	}

	return returnVal;
}


int PS4API sceAudioOutSetVolume(int32_t handle, int32_t flag, int32_t *vol)
{
	LOG_FIXME("Not implemented");

	int returnVal = -1;
	
	// if initialization has not happened
	if (!g_init)
	{
		returnVal = SCE_AUDIO_OUT_ERROR_NOT_INIT;
	}

	// check for invalid flag
	else if (flag < 0 || flag >= (1 << 8))
	{
		returnVal = SCE_AUDIO_OUT_ERROR_INVALID_FLAG;
	}

	// if pointer is invalid
	else if (vol == nullptr)
	{
		returnVal = SCE_AUDIO_OUT_ERROR_INVALID_POINTER;
	}

	else
	{
		// index of the vol array
		int volumeIndex = 0;

		// check a maximum of 8 bits in flag
		for (int i = 0; i < 8; i++)
		{
			// if this bit is 1
			if (flag & (1 << i))
			{
				// if the volume is out of range
				if (vol[volumeIndex] < 0 || vol[volumeIndex] > 32768)
				{
					returnVal = SCE_AUDIO_OUT_ERROR_INVALID_VOLUME;
				}

				// increment
				volumeIndex++;
			}
		}

		if (returnVal != SCE_AUDIO_OUT_ERROR_INVALID_VOLUME)
		{
			returnVal = SCE_OK;
		}
	}

	return returnVal;
}




