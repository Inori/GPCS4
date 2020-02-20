#pragma once

#include "SceCommon.h"
#include <vector>

class GLFWwindow;

namespace sce 
{;

/*
* For a real PS4 hardware, libVideoOut abstract the display hardware connected to the console,
* for our emulator, the video out class correspond to a window.
*/


const uint32_t kVideoOutDefaultWidth = 1920;
const uint32_t kVideoOutDefaultHeight = 1080;


struct SceVideoOutSizeInfo
{
	// Runtime size
	uint32_t windowWidth;
	uint32_t windowHeight;

	// Frame Buffer size
	uint32_t frameWidth;
	uint32_t frameHeight;
};


class SceVideoOut
{
public:
	SceVideoOut(uint32_t width, uint32_t height);
	~SceVideoOut();

	GLFWwindow* getWindowHandle();

	void processEvents();

	std::vector<const char*> getExtensions();

	SceVideoOutSizeInfo getSize();

	/**
	 * \brief Get the window surface
	 * 
	 * This will be used to create swapchain.
	 * This must be called after VkInstance created.
	 * \returns The window surface
	 */
	VkSurfaceKHR getWindowSurface();

	bool registerDisplayrBuffers(uint32_t startIndex, void* const* addresses, uint32_t bufferNum);

	const void* retrieveDisplayBuffer(uint32_t index);

	void setFlipRate(uint32_t rate);

	uint32_t getFlipRate() const;

private:
	static void windowResizeCallback(GLFWwindow* window, int width, int height);
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
	void        destroySurface();

private:

	GLFWwindow* m_window = nullptr;

	uint32_t m_width;
	uint32_t m_height;

	VkSurfaceKHR m_windowSurface = VK_NULL_HANDLE;
	bool m_framebufferResized = false;

	uint32_t m_flipRate = 60;

	std::vector<const void*> m_displayBuffers;
};

} // sce


