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

void SceVideoOut::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto videoOut = reinterpret_cast<SceVideoOut*>(glfwGetWindowUserPointer(window));
	videoOut->m_framebufferResized = true;
}

} // sce