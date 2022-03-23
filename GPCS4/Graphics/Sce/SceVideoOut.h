#pragma once

#include "SceCommon.h"

#include <vector>

class GLFWwindow;
struct SceVideoOutBufferAttribute;

namespace sce
{
;

#define SCE_VIDEO_HANDLE_MAIN 1
#define SCE_VIDEO_HANDLE_MAX  3

struct DisplaySize
{
	// Runtime size
	uint32_t width;
	uint32_t height;
};

/*
* For a real PS4 hardware, a display is connected to the console,
* for our emulator, we use a window to emulate the display.
*/
class VirtualDisplay
{
private:
	// emulated display hardware size
	const uint32_t VirtualDisplayWidth  = 1920;
	const uint32_t VirtualDisplayHeight = 1080;

public:
	VirtualDisplay();
	~VirtualDisplay();

	void processEvents();

	DisplaySize getSize();

	/**
	 * \brief Get the window surface
	 * 
	 * This will be used to create swapchain.
	 * \returns The window surface
	 */
	VkSurfaceKHR getWindowSurface(VkInstance instance);

private:
	static void windowResizeCallback(
		GLFWwindow* window,
		int         width,
		int         height);

	static void framebufferResizeCallback(
		GLFWwindow* window,
		int         width,
		int         height);

private:
	GLFWwindow*  m_window        = nullptr;
	VkSurfaceKHR m_windowSurface = VK_NULL_HANDLE;
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
	SceVideoOut();
	~SceVideoOut();

	VirtualDisplay& display();

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
	uint32_t calculateBufferSize(const SceVideoOutBufferAttribute* attribute);

private:
	VirtualDisplay m_display;

	uint32_t m_flipRate = 60;

	std::vector<SceDisplayBuffer> m_displayBuffers;
};

}  // namespace sce
