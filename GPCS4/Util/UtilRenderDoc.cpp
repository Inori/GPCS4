#include "UtilRenderDoc.h"
#include "PlatModule.h"
#include "renderdoc_app.h"

namespace util
{
	RdcController::RdcController()
	{
		initRdcApi();
	}

	RdcController::~RdcController()
	{
	}

	void RdcController::triggerCapture()
	{
		if (m_api)
		{
			m_api->TriggerCapture();
		}
	}

	void RdcController::startFrameCapture()
	{
		if (m_api)
		{
			m_api->StartFrameCapture(nullptr, nullptr);
		}
	}

	void RdcController::endFrameCapture()
	{
		if (m_api)
		{
			m_api->EndFrameCapture(nullptr, nullptr);
		}
	}

	void RdcController::discardFrameCapture()
	{
		if (m_api)
		{
			m_api->DiscardFrameCapture(nullptr, nullptr);
		}
	}

	bool RdcController::isFrameCapturing()
	{
		return m_api ? m_api->IsFrameCapturing() : false;
	}

	bool RdcController::isTargetControlConnected()
	{
		return m_api ? m_api->IsTargetControlConnected() : false;
	}

	void RdcController::initRdcApi()
	{
		do 
		{
			void* rdcModule = plat::moduleGet("renderdoc.dll");
			if (!rdcModule)
			{
				break;
			}

			pRENDERDOC_GetAPI RENDERDOC_GetAPI =
				(pRENDERDOC_GetAPI)plat::moduleGetSymbol(rdcModule, "RENDERDOC_GetAPI");

			int ret = RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_5_0, (void**)&m_api);
			if (ret != 1)
			{
				break;
			}

			if (!m_api)
			{
				break;
			}

		} while (false);
	}

}  // namespace util