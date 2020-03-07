#pragma once

#include "SceCommon.h"
#include <vector>

class GLFWwindow;
struct SceVideoOutBufferAttribute;

namespace sce 
{;


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

/**
 * \brief Gnm Display Buffer Descriptor
 *
 *  Display buffer information
 */
struct SceDisplayBuffer
{
	const void* address;
	int32_t     tile;
	int32_t     format;
	uint32_t    width;
	uint32_t    height;
	uint32_t    size;
};


/*
* For a real PS4 hardware, libVideoOut abstract the display hardware connected to the console,
* for our emulator, the video out class correspond to a window.
*/

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
	 * \returns The window surface
	 */
	VkSurfaceKHR getWindowSurface(VkInstance instance);

	bool registerDisplayrBuffers(
		uint32_t                          startIndex,
		void* const*                      addresses,
		uint32_t                          bufferNum,
		const SceVideoOutBufferAttribute* attribute);

	uint32_t numDisplayBuffer();

	SceDisplayBuffer getDisplayBuffer(uint32_t index);

	void setFlipRate(uint32_t rate);

	uint32_t getFlipRate() const;

private:
	static void windowResizeCallback(
		GLFWwindow* window,
		int         width,
		int         height);

	static void framebufferResizeCallback(
		GLFWwindow* window,
		int         width,
		int         height);

	uint32_t calculateBufferSize(const SceVideoOutBufferAttribute* attribute);

	void        destroySurface();

private:

	GLFWwindow* m_window = nullptr;

	uint32_t m_width;
	uint32_t m_height;

	VkSurfaceKHR m_windowSurface      = VK_NULL_HANDLE;
	VkInstance   m_instance           = VK_NULL_HANDLE;
	bool         m_framebufferResized = false;

	uint32_t m_flipRate = 60;

	std::vector<SceDisplayBuffer> m_displayBuffers;
};

} // sce


