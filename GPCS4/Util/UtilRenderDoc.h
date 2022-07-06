#pragma once

#include "UtilSingleton.h"

struct RENDERDOC_API_1_5_0;

namespace util
{
	class RdcController : public Singleton<RdcController>
	{
		friend class Singleton<RdcController>;
	public:
		RdcController();
		virtual ~RdcController();

		/**
		 * \brief Trigger Capture
		 * 
		 * This function will trigger a capture as if the user had pressed one of the capture hotkeys.
		 * The capture will be taken from the next frame presented to whichever window is considered current.
		 */
		void triggerCapture();

		/**
		 * \brief Start Frame Capture
		 * 
		 * This function will immediately begin a capture for the specified device/window combination.
		 */
		void startFrameCapture();

		/**
		 * \brief End Frame Capture
		 * 
		 * This function will immediately end an active capture for the specified device/window combination.
		 */
		void endFrameCapture();

		/**
		 * \brief Discard Frame Capture
		 * 
		 * This function is similar to EndFrameCapture() but the capture contents will be discarded
		 * immediately, and not processed and written to disk. This will be more efficient than
		 * EndFrameCapture() if the frame capture is not needed.
		 */
		void discardFrameCapture();

		/**
		 * \brief Is Frame Capturing
		 * 
		 * This function returns a value to indicate whether the current frame is capturing.
		 */
		bool isFrameCapturing();
		
		/**
		 * \brief 
		 * 
		 * This function returns a value to indicate whether the RenderDoc UI
		 * is currently connected to the current process.
		 */
		bool isTargetControlConnected();

	private:
		void initRdcApi();

	private:
		RENDERDOC_API_1_5_0* m_api = nullptr;
	};


}  // namespace util