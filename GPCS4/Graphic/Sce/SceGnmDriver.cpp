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

}

SceGnmDriver::~SceGnmDriver()
{
	m_videoOut->destroySurface(*m_instance);
}

bool SceGnmDriver::initGnmDriver()
{
	bool ret = false;
	do
	{
		// Instance
		auto extensions = m_videoOut->getExtensions();
		m_instance      = gveCreateInstance(extensions);
		if (!m_instance)
		{
			LOG_ERR("create vulkan instance failed.");
			break;
		}

		// Physical device
		VkSurfaceKHR windowSurface = m_videoOut->createSurface(*m_instance);
		m_physDevice               = m_instance->pickPhysicalDevice(windowSurface);
		if (!m_physDevice)
		{
			LOG_ERR("pick physical device failed.");
			break;
		}
		
		// Logical device
		m_device = m_physDevice->createLogicalDevice();
		if (!m_device)
		{
			LOG_ERR("create logical device failed.");
			break;
		}

		if (!initGraphics())
		{
			LOG_ERR("initialize graphics environment failed.");
			break;
		}

		if (!initCompute())
		{
			LOG_ERR("initialize compute environment failed.");
			break;
		}

		ret = true;
	} while (false);
	return ret;
}

bool SceGnmDriver::initGraphics()
{
	bool ret = false;
	do
	{
		m_context = m_device->createContext();
		if (!m_context)
		{
			break;
		}

		m_commandParser = std::make_unique<GnmCmdStream>();

		m_graphicsCmdBuffer = std::make_unique<GnmCommandBufferDraw>();
		ret  = true;
	}while(false);
	return ret;
}

bool SceGnmDriver::initCompute()
{
}

bool SceGnmDriver::createSwapchain(uint32_t bufferNum)
{
	// Swap chain
	m_swapchain = m_device->createSwapchain(m_videoOut, bufferNum);

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
	// There's only one hardware graphics queue for most of modern GPUs, including the one on PS4.
	// Thus a PS4 game will call submit function to submit command buffers sequentially, 
	// and normally in one same thread.
	// We just emulate the GPU, parsing and executing one command buffer per call.
	
	// TODO:
	// For future development, we could try to record vulkan command buffer asynchronously,
	// reducing time period of the submit call.

	int err = SCE_GNM_ERROR_UNKNOWN;
	do 
	{
		LOG_ASSERT(count == 1, "Currently only support 1 cmdbuff at one call.");

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

		err = SCE_OK;
	} while (false);
	return err;
}

int SceGnmDriver::sceGnmSubmitDone(void)
{
	m_videoOut->processEvents();
	return SCE_OK;
}

}  //sce