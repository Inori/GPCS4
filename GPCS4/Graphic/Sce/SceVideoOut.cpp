#include "SceVideoOut.h"

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
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

uint32_t SceVideoOut::width()
{
	return m_width;
}

uint32_t SceVideoOut::height()
{
	return m_height;
}

GLFWwindow* SceVideoOut::getWindowHandle()
{
	return m_window;
}

void SceVideoOut::getWindowSize(uint32_t& width, uint32_t& height)
{
	glfwGetWindowSize(m_window, (int*)&width, (int*)&height);
}

void SceVideoOut::getFramebufferSize(uint32_t& width, uint32_t& height)
{
	glfwGetFramebufferSize(m_window, (int*)&width, (int*)&height);
}

VkSurfaceKHR SceVideoOut::createSurface(VkInstance instance)
{
	if (m_windowSurface == VK_NULL_HANDLE)
	{
		glfwCreateWindowSurface(instance, m_window, nullptr, &m_windowSurface);
	}
	return m_windowSurface;
}

void SceVideoOut::destroySurface(VkInstance instance)
{
	if (m_windowSurface)
	{
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

bool SceVideoOut::registerBuffers(uint32_t startIndex, uint32_t bufferNum)
{
	bool bRet = false;
	do
	{
		
		bRet  = true;
	}while(false);
	return bRet;
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