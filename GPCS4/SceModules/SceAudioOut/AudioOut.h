#pragma once

#include <memory>
#include "sce_audioout.h"

struct AudioOutContext;

class AudioOut
{
public:
	AudioOut(SceUserServiceUserId userId, int32_t type, int32_t index, uint32_t len, uint32_t freq, uint32_t param);
	~AudioOut();
	int32_t audioOutput(const void* ptr);
	int32_t audioClose();
	int getLastError();
private:
	std::unique_ptr<AudioOutContext> m_audioOutContext;
};
