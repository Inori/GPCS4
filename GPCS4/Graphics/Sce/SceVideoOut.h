#pragma once

#include "SceCommon.h"

#include <vector>

class GLFWwindow;
struct SceVideoOutBufferAttribute;

namespace sce
{

	struct DisplaySize
	{
		// Runtime size in pixels
		uint32_t width;
		uint32_t height;
	};

	/*
     * For a real PS4 hardware, a hardware display is connected to the console,
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

		/**
	     * \brief Get the window size in pixels
	     * 
	     * This can be used to help create
	     * render target/swapchain.
	     * \returns The window size
	     */
		DisplaySize getSize() const;

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
		void*    address;
		int32_t  pixelFormat;  // SceVideoOutPixelFormat
		int32_t  tilingMode;   // SceVideoOutTilingMode
		int32_t  aspectRatio;  // SceVideoOutAspectRatio
		uint32_t pitchInPixel;
		uint32_t option;       // SceVideoOutBufferAttributeOption
		uint32_t width;
		uint32_t height;
		uint32_t size;
	};

	/*
     * For a real PS4 hardware, libVideoOut abstract the display hardware connected to the console,
     * for our emulator, the video out class correspond to a window.
     */

	class SceVideoOut
	{
	public:
		SceVideoOut(int32_t busType, const void* param);
		~SceVideoOut();

		int32_t busType();

		DisplaySize getSize() const;

		bool registerDisplayrBuffers(
			uint32_t                          startIndex,
			void* const*                      addresses,
			uint32_t                          bufferNum,
			const SceVideoOutBufferAttribute* attribute);

		uint32_t numDisplayBuffer();

		SceDisplayBuffer getDisplayBuffer(uint32_t index);

		VkSurfaceKHR getSurface(VkInstance instance);

		void setFlipRate(uint32_t rate);

		uint32_t getFlipRate() const;

	private:
		uint32_t calculateBufferSize(
			const SceVideoOutBufferAttribute* attribute);
		void createPresenter(
			uint32_t                          bufferNum,
			const SceVideoOutBufferAttribute* attribute);

	private:
		VirtualDisplay m_display;

		// SceVideoOutBusType
		int32_t  m_busType  = 0;
		uint32_t m_flipRate = 60;

		std::vector<SceDisplayBuffer> m_displayBuffers;
	};

}  // namespace sce
