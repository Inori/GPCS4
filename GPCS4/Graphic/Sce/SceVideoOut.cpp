#include "SceVideoOut.h"

namespace sce
{;

SceVideoOut::SceVideoOut(uint32_t width, uint32_t height):
	m_width(width),
	m_height(height),
	m_framebufferResized(false)
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	m_window = glfwCreateWindow(m_width, m_height, GPCS4_APP_NAME, nullptr, nullptr);
	glfwSetWindowUserPointer(m_window, this);

	glfwSetWindowSizeCallback(m_window, windowResizeCallback);
	glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
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

void SceVideoOut::getWindowSize(uint32_t& width, uint32_t& height)
{
	glfwGetWindowSize(m_window, (int*)&width, (int*)&height);
}

void SceVideoOut::getFramebufferSize(uint32_t& width, uint32_t& height)
{
	glfwGetFramebufferSize(m_window, (int*)&width, (int*)&height);
}

bool SceVideoOut::getSurface(VkInstance instance, VkSurfaceKHR& surface)
{
	bool bRet = false;
	do
	{
		if (glfwCreateWindowSurface(instance, m_window, nullptr, &surface) != VK_SUCCESS)
		{
			break;
		}
		bRet  = true;
	}while(false);
	return bRet;
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