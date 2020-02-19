#include "SceVideoOut.h"
#include "SceGnmDriver.h"

#include "../GraphicShared.h"
#include "../Gve/GveInstance.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace sce
{;

SceVideoOut::SceVideoOut(uint32_t width, uint32_t height):
	m_width(width),
	m_height(height)
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	m_window = glfwCreateWindow(m_width, m_height, GPCS4_APP_NAME, nullptr, nullptr);
	glfwSetWindowUserPointer(m_window, this);

	glfwSetWindowSizeCallback(m_window, windowResizeCallback);
	glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);

	//glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GLFW_TRUE);
}

SceVideoOut::~SceVideoOut()
{
	destroySurface();
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

GLFWwindow* SceVideoOut::getWindowHandle()
{
	return m_window;
}

VkSurfaceKHR SceVideoOut::getWindowSurface()
{
	if (m_windowSurface == VK_NULL_HANDLE)
	{
		auto       gnmDriver = getGnmDriver(SCE_VIDEO_HANDLE_MAIN);
		VkInstance instance  = *gnmDriver->m_instance;
		glfwCreateWindowSurface(instance, m_window, nullptr, &m_windowSurface);
	}
	return m_windowSurface;
}

void SceVideoOut::destroySurface()
{
	if (m_windowSurface)
	{
		auto gnmDriver = getGnmDriver(SCE_VIDEO_HANDLE_MAIN);
		VkInstance instance  = *gnmDriver->m_instance;
		vkDestroySurfaceKHR(instance, m_windowSurface, nullptr);
		m_windowSurface = VK_NULL_HANDLE;
	}
}

std::vector<const char*> SceVideoOut::getExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
	return extensions;
}

SceVideoOutSizeInfo SceVideoOut::getSize()
{
	SceVideoOutSizeInfo result = {};

	glfwGetWindowSize(m_window, (int*)&result.windowWidth, (int*)&result.windowHeight);
	glfwGetFramebufferSize(m_window, (int*)&result.frameWidth, (int*)&result.frameHeight);

	return result;
}

bool SceVideoOut::registerDisplayrBuffers(uint32_t startIndex, void* const* addresses, uint32_t bufferNum)
{
	bool bRet = false;
	do
	{
		if (!addresses || !bufferNum)
		{
			break;
		}

		m_displayBuffers.assign(addresses, addresses + bufferNum);

		bRet = true;
	} while (false);
	return bRet;
}

const void* SceVideoOut::retrieveDisplayBuffer(uint32_t index)
{
	return m_displayBuffers[index];
}

void SceVideoOut::setFlipRate(uint32_t rate)
{
	m_flipRate = rate;
}

uint32_t SceVideoOut::getFlipRate() const
{
	return m_flipRate;
}

void SceVideoOut::processEvents()
{
	glfwPollEvents();
}

void SceVideoOut::windowResizeCallback(GLFWwindow* window, int width, int height)
{
	auto videoOut = reinterpret_cast<SceVideoOut*>(glfwGetWindowUserPointer(window));
	
	videoOut->m_width = width;
	videoOut->m_height = height;
}

void SceVideoOut::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto videoOut = reinterpret_cast<SceVideoOut*>(glfwGetWindowUserPointer(window));
	videoOut->m_framebufferResized = true;
}

} // sce