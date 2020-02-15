#include "SceGnmDriver.h"
#include "sce_errors.h"

#include "../GraphicShared.h"
#include "../Gnm/GnmCmdStream.h"
#include "../Gnm/GnmCommandBufferDraw.h"
#include "../Gnm/GnmCommandBufferDummy.h"
#include "../Gve/GveInstance.h"
#include "../Gve/GveSwapChain.h"
#include "../Gve/GvePresenter.h"
#include "../Gve/GveCmdList.h"
#include "../Gve/GveImage.h"

LOG_CHANNEL(Graphic.Sce.SceGnmDriver);

namespace sce
{;

using namespace gve;
const int MAX_FRAMES_IN_FLIGHT = 2;

SceGnmDriver::SceGnmDriver(std::shared_ptr<SceVideoOut>& videoOut):
	m_videoOut(videoOut)
{
	// Instance
	auto extensions = m_videoOut->getExtensions();
	m_instance = gveCreateInstance(extensions);

	// Physical device
	VkSurfaceKHR windowSurface = m_videoOut->createSurface(*m_instance);
	m_physDevice = m_instance->pickPhysicalDevice(windowSurface);
	LOG_ASSERT(m_physDevice != nullptr, "pick physical device failed.");

	// Logical device
	m_device = m_physDevice->createLogicalDevice();
	LOG_ASSERT(m_device != nullptr, "create logical device failed.");
}

SceGnmDriver::~SceGnmDriver()
{
	m_commandBuffers.clear();
	m_commandParsers.clear();

	m_videoOut->destroySurface(*m_instance);
}

bool SceGnmDriver::initDriver(uint32_t bufferNum)
{
	// Swap chain
	m_swapchain = m_device->createSwapchain(m_videoOut, bufferNum);

	m_presenter = std::make_unique<gve::GvePresenter>(m_device.ptr(), m_swapchain.ptr());

	createCommandParsers(bufferNum);

	return true;
}

int SceGnmDriver::submitCommandBuffers(uint32_t count, 
	void *dcbGpuAddrs[], uint32_t *dcbSizesInBytes,
	void *ccbGpuAddrs[], uint32_t *ccbSizesInBytes)
{
	return submitAndFlipCommandBuffers(count,
		dcbGpuAddrs, dcbSizesInBytes,
		ccbGpuAddrs, ccbSizesInBytes,
		SCE_VIDEO_HANDLE_MAIN, 0, 0, 0);
}

int SceGnmDriver::submitAndFlipCommandBuffers(uint32_t count, 
	void *dcbGpuAddrs[], uint32_t *dcbSizesInBytes,
	void *ccbGpuAddrs[], uint32_t *ccbSizesInBytes,
	uint32_t videoOutHandle, uint32_t displayBufferIndex, 
	uint32_t flipMode, int64_t flipArg)
{
	int err = SCE_GNM_ERROR_UNKNOWN;
	do 
	{
		LOG_ASSERT(count == 1, "Currently only support only 1 cmdbuff.");

		auto& cmdParser = m_commandParsers[displayBufferIndex];
		if (!cmdParser->processCommandBuffer((uint32_t*)dcbGpuAddrs[0], dcbSizesInBytes[0]))
		{
			break;
		}
	
		auto cmdList = cmdParser->getCommandBuffer()->getCmdList();
		if (!cmdList)
		{
			// cmdList is null when GPCS4_NO_GRAPHICS defined
			err = SCE_OK;
			break;
		}

		m_presenter->present(cmdList);
		// TODO:
		// This should be done asynchronously
		cmdList->reset();

		err = SCE_OK;
	} while (false);
	return err;
}

int SceGnmDriver::sceGnmSubmitDone(void)
{
	m_videoOut->processEvents();
	return SCE_OK;
}

void SceGnmDriver::createCommandParsers(uint32_t count)
{
	// Initialize command buffers and command parsers
	// according to bufferNum
	m_commandBuffers.resize(count);
	for (uint32_t i = 0; i != count; ++i)
	{
		auto rtImgView = m_swapchain->getImageView(i);

#ifndef GPCS4_NO_GRAPHICS
		m_commandBuffers[i] = std::make_shared<GnmCommandBufferDraw>(m_device, rtImgView);
#else
		m_commandBuffers[i] = std::make_shared<GnmCommandBufferDummy>();
#endif // GPCS4_NO_GRAPHICS

	}
	
	m_commandParsers.resize(count);
	for (uint32_t i = 0; i != count; ++i)
	{
		m_commandParsers[i] = std::make_unique<GnmCmdStream>(m_commandBuffers[i]);
	}
}

}  //sce