#pragma once

#include "GPCS4Common.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

namespace sce 
{;

/*
* For a real PS4 hardware, libVideoOut abstract the display hardware connected to the console,
* for our emulator, the video out class correspond to a window.
*/


const uint32_t kVideoOutDefaultWidth = 1920;
const uint32_t kVideoOutDefaultHeight = 1080;



class SceVideoOut
{
public:
	SceVideoOut(uint32_t width, uint32_t height);
	~SceVideoOut();

	uint32_t width();

	uint32_t height();

	void getWindowSize(uint32_t& width, uint32_t& height);

	void getFramebufferSize(uint32_t& width, uint32_t& height);

	bool getSurface(VkInstance instance, VkSurfaceKHR& surface);

	std::vector<const char*> getExtensions();

	bool registerBuffers(uint32_t startIndex, uint32_t bufferNum);

	void processEvents();

private:
	static void windowResizeCallback(GLFWwindow* window, int width, int height);
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

private:

	GLFWwindow* m_window;

	uint32_t m_width;
	uint32_t m_height;

	bool m_framebufferResized;
};

} // sce


