#pragma once


typedef enum {
	SCE_AUDIO_OUT_STATE_OUTPUT_UNKNOWN = 0,
	SCE_AUDIO_OUT_STATE_OUTPUT_CONNECTED_PRIMARY = (1 << 0), // primary output
	SCE_AUDIO_OUT_STATE_OUTPUT_CONNECTED_SECONDARY = (1 << 1), // secondary output
	SCE_AUDIO_OUT_STATE_OUTPUT_CONNECTED_TERTIARY = (1 << 2), // controller speaker
	SCE_AUDIO_OUT_STATE_OUTPUT_CONNECTED_HEADPHONE = (1 << 6), // USB/bluetooth headphone
	SCE_AUDIO_OUT_STATE_OUTPUT_CONNECTED_EXTERNAL = (1 << 7), // recording, remoteplay, spectating, etc.
} SceAudioOutStateOutput;


typedef enum {
	SCE_AUDIO_OUT_STATE_CHANNEL_UNKNOWN = 0, // error
	SCE_AUDIO_OUT_STATE_CHANNEL_DISCONNECTED = 0, // disconnected
	SCE_AUDIO_OUT_STATE_CHANNEL_1 = 1, // 1.0ch
	SCE_AUDIO_OUT_STATE_CHANNEL_2 = 2, // 2.0ch
	SCE_AUDIO_OUT_STATE_CHANNEL_6 = 6, // 5.1ch
	SCE_AUDIO_OUT_STATE_CHANNEL_8 = 8, // 7.1ch
} SceAudioOutStateChannel;


typedef struct {
	uint16_t output;		// SceAudioOutStateOutput (bitwise OR)
	uint8_t  channel;		// SceAudioOutStateChannel
	uint8_t  reserved8_1[1];// reserved
	int16_t  volume;
	uint16_t rerouteCounter;
	uint64_t flag;			// SceAudioOutStateFlag (bitwise OR)
	uint64_t reserved64[2];	// reserved
} SceAudioOutPortState;


struct SceAudioOutOutputParam 
{
	int32_t handle;
	const void *ptr;
};
