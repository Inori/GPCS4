#pragma once

#include "GPCS4Common.h"
#include "../Gnm/GnmCmdStreamDraw.h"
#include <memory>

class SceGnmDriver
{
public:
	SceGnmDriver();
	~SceGnmDriver();




	int submitAndFlipCommandBuffers(uint32_t count,
		void *dcbGpuAddrs[], uint32_t *dcbSizesInBytes,
		void *ccbGpuAddrs[], uint32_t *ccbSizesInBytes,
		uint32_t videoOutHandle, uint32_t displayBufferIndex,
		uint32_t flipMode, int64_t flipArg);

	int sceGnmSubmitDone(void);

private:
	std::shared_ptr<GnmCommandBufferDraw> m_dcb;
	std::shared_ptr <GnmCmdStreamDraw> m_cmdHandlerDraw;
};


