#include "AudioOut.h"
#include "rtaudio/rtaudio_c.h"
#include "BlockingQueue.h"

#include <mutex>
#include <condition_variable>
#include <algorithm>
#include <string>

// #define DUMP_AUDIO
#ifdef DUMP_AUDIO
class AudioDumper
{
public:
	AudioDumper(std::string const &filename)
	{
		m_file = fopen(filename.c_str(), "wb");
	}

	void dumpAudio(const uint8_t *ptr, size_t size)
	{
		fwrite(ptr, size, 1, m_file);
	}
private:
	FILE* m_file;
};
#endif

struct AudioOutContext
{
	struct
	{
		SceUserServiceUserId userId;
		int32_t type;
		int32_t index;
		uint32_t len;
		uint32_t freq;
		uint32_t param;
	} apiParams;

	struct
	{
		uint32_t numChannels;
		uint32_t bytesConsumesPerSample;
	} requestedArgs;

	struct
	{
		uint32_t numChannels;
		uint32_t bytesConsumesPerSample;
	} supportedArgs;

	rtaudio_t audioHandle;
	uint32_t bytesPerSample;

	std::mutex mutex;
	std::condition_variable condDone;
	BlockingQueue<const void*> queue;

	bool doneFlag = false;
	bool streamOpenFlag = false;
	int lastError = 0;
	
	#ifdef DUMP_AUDIO
	AudioDumper audioDumper;
	AudioOutContext() :
		queue{ 1 }, audioDumper{ "audiodump.raw" }
	{
		audioHandle = rtaudio_create(RTAUDIO_API_UNSPECIFIED);
	}
	#else
	AudioOutContext() :
		queue {1}
	{
		audioHandle = rtaudio_create(RTAUDIO_API_UNSPECIFIED);
	}
	#endif

	~AudioOutContext()
	{
		rtaudio_destroy(audioHandle);
	}
};

struct AudioProperties
{
	uint32_t nChannels;
	uint32_t bytesPerSample;
	uint32_t audioFormat;
};


static AudioProperties getAudioProperties(uint32_t param)
{
	uint32_t format       = param & 0x000000ff;
	AudioProperties props = {};

	switch (format)
	{
	case SCE_AUDIO_OUT_PARAM_FORMAT_S16_MONO:
	{
		props.nChannels   = 1;
		props.bytesPerSample  = 2;
		props.audioFormat = RTAUDIO_FORMAT_SINT16;
		break;
	}

	case SCE_AUDIO_OUT_PARAM_FORMAT_S16_STEREO:
	{
		props.nChannels   = 2;
		props.bytesPerSample  = 2;
		props.audioFormat = RTAUDIO_FORMAT_SINT16;
		break;
	}

	case SCE_AUDIO_OUT_PARAM_FORMAT_S16_8CH:
	{
		props.nChannels   = 8;
		props.bytesPerSample  = 2;
		props.audioFormat = RTAUDIO_FORMAT_SINT16;
		break;
	}

	case SCE_AUDIO_OUT_PARAM_FORMAT_S16_8CH_STD:
	{
		props.nChannels   = 8;
		props.bytesPerSample  = 2;
		props.audioFormat = RTAUDIO_FORMAT_SINT16;
		break;
	}

	case SCE_AUDIO_OUT_PARAM_FORMAT_FLOAT_MONO:
	{
		props.nChannels   = 1;
		props.bytesPerSample  = 4;
		props.audioFormat = RTAUDIO_FORMAT_FLOAT32;
	}

	case SCE_AUDIO_OUT_PARAM_FORMAT_FLOAT_STEREO:
	{
		props.nChannels   = 2;
		props.bytesPerSample  = 4;
		props.audioFormat = RTAUDIO_FORMAT_FLOAT32;
		break;
	}

	case SCE_AUDIO_OUT_PARAM_FORMAT_FLOAT_8CH:
	{
		props.nChannels   = 8;
		props.bytesPerSample  = 4;
		props.audioFormat = RTAUDIO_FORMAT_FLOAT32;
		break;
	}

	case SCE_AUDIO_OUT_PARAM_FORMAT_FLOAT_8CH_STD:
	{
		props.nChannels   = 8;
		props.bytesPerSample  = 4;
		props.audioFormat = RTAUDIO_FORMAT_FLOAT32;
		break;
	}
	}

	return props;
}

static int outputCallBack(void* outputBuffer,
						  void* in,
						  unsigned int nFrames,
						  double stream_time,
						  rtaudio_stream_status_t status,
						  void* userdata)
{
	int rc = 0;
	auto ctx = reinterpret_cast<AudioOutContext*>(userdata);
	auto buffer = reinterpret_cast<const uint8_t*>(ctx->queue.pop());
	{
		std::unique_lock<std::mutex> lock {ctx->mutex};
		if (buffer == nullptr)
		{
			rc = 1;
		}
		else
		{
			std::copy(buffer,
					  buffer + ctx->requestedArgs.bytesConsumesPerSample,
					  reinterpret_cast<uint8_t*>(outputBuffer));
			rc = 0;
		}

		ctx->doneFlag = true;
	}

	ctx->condDone.notify_one();

	return rc;
}

AudioOut::AudioOut(SceUserServiceUserId userId,
				   int32_t type,
				   int32_t index,
				   uint32_t len,
				   uint32_t freq,
				   uint32_t param)
{
	m_audioOutContext = std::make_unique<AudioOutContext>();

	// save API parameteres
	m_audioOutContext->apiParams.userId = userId;
	m_audioOutContext->apiParams.type = type;
	m_audioOutContext->apiParams.index = index;
	m_audioOutContext->apiParams.len = len;
	m_audioOutContext->apiParams.freq = freq;
	m_audioOutContext->apiParams.param = param;

	// requested paramters
	auto audioProps = getAudioProperties(param);
	m_audioOutContext->bytesPerSample            = audioProps.bytesPerSample;
	m_audioOutContext->requestedArgs.numChannels = audioProps.nChannels;
	m_audioOutContext->requestedArgs.bytesConsumesPerSample =
		m_audioOutContext->requestedArgs.numChannels
		* audioProps.bytesPerSample 
		* m_audioOutContext->apiParams.len;

	auto devId = rtaudio_get_default_output_device(m_audioOutContext->audioHandle);
	auto devInfo = rtaudio_get_device_info(m_audioOutContext->audioHandle, devId);

	// supported parameters
	m_audioOutContext->supportedArgs.numChannels = devInfo.output_channels;

	m_audioOutContext->supportedArgs.bytesConsumesPerSample =
		m_audioOutContext->supportedArgs.numChannels 
		* audioProps.bytesPerSample 
		* m_audioOutContext->apiParams.len;

	// open audio stream
	rtaudio_stream_parameters_t streamParam;
	streamParam.device_id = devId;
	streamParam.first_channel = 0;
	streamParam.num_channels  = m_audioOutContext->requestedArgs.numChannels;

	unsigned int bufferFrameSize = static_cast<unsigned int>(m_audioOutContext->apiParams.len);
	m_audioOutContext->lastError =
		rtaudio_open_stream(m_audioOutContext->audioHandle,
							&streamParam,
							nullptr,
							audioProps.audioFormat,
							m_audioOutContext->apiParams.freq,
							&bufferFrameSize,
							outputCallBack,
							m_audioOutContext.get(),
							nullptr,
							nullptr);
}

AudioOut::~AudioOut() = default;

int32_t AudioOut::audioOutput(const void* ptr)
{
	int rc = 0;
	do
	{
		if (m_audioOutContext->streamOpenFlag == false)
		{
			rtaudio_start_stream(m_audioOutContext->audioHandle);
			if (rtaudio_error(m_audioOutContext->audioHandle) != nullptr)
			{
				m_audioOutContext->lastError = -1;
				rc                           = -1;

				break;	
			}
			m_audioOutContext->streamOpenFlag = true;
		}

		if (ptr == nullptr)
		{
			std::unique_lock<std::mutex> lock { m_audioOutContext->mutex };
			m_audioOutContext->condDone.wait(lock, [=] { return m_audioOutContext->doneFlag; });
			break;
		}

		auto dataPtr = reinterpret_cast<const uint8_t*>(ptr);

#ifdef DUMP_AUDIO
			m_audioOutContext
				->audioDumper
				.dumpAudio(dataPtr, m_audioOutContext->requestedArgs.bytesConsumesPerSample);
#endif

		{
			std::unique_lock<std::mutex> lock{ m_audioOutContext->mutex };
			m_audioOutContext->doneFlag = false;
			m_audioOutContext->queue.push(dataPtr);
			m_audioOutContext->condDone.wait(lock, [=] { return m_audioOutContext->doneFlag; });
		}

	} while (false);

	return rc;
}

int32_t AudioOut::audioClose()
{
	m_audioOutContext->queue.push(nullptr);
	rtaudio_close_stream(m_audioOutContext->audioHandle);
	m_audioOutContext->streamOpenFlag = false;

	return 0;
}

int AudioOut::getLastError()
{
	return m_audioOutContext->lastError;
}
