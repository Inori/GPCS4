#include "sce_audioout.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs

LOG_CHANNEL(SceModules.SceAudioOut);

//////////////////////////////////////////////////////////////////////////
// library: libSceAudioOut
//////////////////////////////////////////////////////////////////////////

int PS4API sceAudioOutInit(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceAudioOutOpen(SceUserServiceUserId userId, int32_t type, int32_t index, uint32_t len, uint32_t freq, uint32_t param)
{
	LOG_SCE_DUMMY_IMPL();
	return 0x789;
}


int PS4API sceAudioOutClose(int32_t handle)
{
	LOG_FIXME("Not implemented");
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


int PS4API sceAudioOutOutput(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
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




