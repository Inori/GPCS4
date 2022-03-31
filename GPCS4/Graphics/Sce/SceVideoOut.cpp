#include "SceVideoOut.h"

#include "SceGnmDriver.h"
#include "ScePresenter.h"
#include "SceVideoOut/sce_videoout_types.h"
#include "VirtualGPU.h"

#include "Emulator/Emulator.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace sce
{

	VirtualDisplay::VirtualDisplay()
	{
		glfwInit();

		// for vulkan
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		// a hardware display shouldn't be resizable of course.
		// well, a virtual one should be:) ,but currently we just disable resize.
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		m_window = glfwCreateWindow(VirtualDisplayWidth, VirtualDisplayHeight, GPCS4_APP_NAME, nullptr, nullptr);
		glfwSetWindowUserPointer(m_window, this);

		glfwSetWindowSizeCallback(m_window, windowResizeCallback);
		glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
	}

	VirtualDisplay::~VirtualDisplay()
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	void VirtualDisplay::processEvents()
	{
		glfwPollEvents();
	}

	DisplaySize VirtualDisplay::getSize() const
	{
		DisplaySize result = {};
		glfwGetFramebufferSize(m_window, (int*)&result.width, (int*)&result.height);
		return result;
	}

	VkSurfaceKHR VirtualDisplay::getWindowSurface(VkInstance instance)
	{
		// this is the only window surface during the process lifetime
		// let system destroy the surface when the window is destroied.
		if (m_windowSurface == VK_NULL_HANDLE)
		{
			glfwCreateWindowSurface(instance, m_window, nullptr, &m_windowSurface);
		}
		return m_windowSurface;
	}

	void VirtualDisplay::windowResizeCallback(GLFWwindow* window, int width, int height)
	{
	}

	void VirtualDisplay::framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
	}

	//////////////////////////////////////////////////////////////////////////

	SceVideoOut::SceVideoOut(int32_t busType, const void* param) :
		m_busType(busType)
	{
	}

	SceVideoOut::~SceVideoOut()
	{
	}

	int32_t SceVideoOut::busType()
	{
		return m_busType;
	}

	DisplaySize SceVideoOut::getSize() const
	{
		return m_display.getSize();
	}

	bool SceVideoOut::registerDisplayrBuffers(
		uint32_t                          startIndex,
		void* const*                      addresses,
		uint32_t                          bufferNum,
		const SceVideoOutBufferAttribute* attribute)
	{
		bool bRet = false;
		do
		{
			if (!addresses || !bufferNum)
			{
				break;
			}

			SceDisplayBuffer buffer = {};
			buffer.format           = attribute->pixelFormat;
			buffer.tile             = attribute->tilingMode;
			buffer.width            = attribute->width;
			buffer.height           = attribute->height;
			buffer.size             = calculateBufferSize(attribute);

			for (uint32_t i = 0; i != bufferNum; ++i)
			{
				buffer.address = addresses[i];
				m_displayBuffers.emplace_back(buffer);
			}

			// It's time to create vulkan swapchain.
			createPresenter(bufferNum, attribute);

			bRet = true;
		} while (false);
		return bRet;
	}
	
	void SceVideoOut::createPresenter(
		uint32_t bufferNum,
		const SceVideoOutBufferAttribute* attribute)
	{
		PresenterDesc desc = {};
		desc.imageExtent   = { attribute->width, attribute->height };
		desc.imageCount    = bufferNum;

		// TODO:
		// use attribute->pixelFormat and convert SceVideoOutPixelFormat to VkSurfaceFormatKHR
		desc.numFormats            = 1;
		desc.formats[0].format     = VK_FORMAT_B8G8R8A8_SRGB;
		desc.formats[0].colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

		desc.numPresentModes = 1;
		desc.presentModes[0] = VK_PRESENT_MODE_MAILBOX_KHR;

		auto& gnmDriver = GPU().gnmDriver();
		gnmDriver.createPresenter(this, desc);
	}

	uint32_t SceVideoOut::numDisplayBuffer()
	{
		return m_displayBuffers.size();
	}

	sce::SceDisplayBuffer SceVideoOut::getDisplayBuffer(uint32_t index)
	{
		return m_displayBuffers[index];
	}

	VkSurfaceKHR SceVideoOut::getSurface(VkInstance instance)
	{
		return m_display.getWindowSurface(instance);
	}

	void SceVideoOut::setFlipRate(uint32_t rate)
	{
		m_flipRate = rate;
	}

	uint32_t SceVideoOut::getFlipRate() const
	{
		return m_flipRate;
	}

	uint32_t SceVideoOut::calculateBufferSize(const SceVideoOutBufferAttribute* attribute)
	{
		// TODO:
		// Implement size

		// Note:
		// The returned size must be equal with GnmRenderTarget::getColorSizeAlign
		return 0;
	}


}  // namespace sce