#include "VirtualGPU.h"

#include "SceUserService/user_service_defs.h"
#include "sce_errors.h"

#include "Gnm/GnmConstant.h"
#include "Sce/SceGnmDriver.h"
#include "Sce/SceResourceTracker.h"
#include "Sce/SceLabelManager.h"
#include "Sce/SceVideoOut.h"

LOG_CHANNEL(Graphic.VirtualGPU);

namespace sce
{

	VirtualGPU::VirtualGPU()
	{
		m_gnmDriver = std::make_shared<SceGnmDriver>();
	}

	VirtualGPU::~VirtualGPU()
	{
	}

	int VirtualGPU::videoOutOpen(SceUserServiceUserId userId, int32_t type, int32_t index, const void* param)
	{
		int                   result                                       = SCE_ERROR_UNKNOWN;
		const static uint32_t typeIndexTable[SCE_VIDEO_OUT_BUS_TYPE_COUNT] = { 0, 0, 0, 0, 0, 1, 2 };
		do
		{
			if (/*(userId != 0 && userId != SCE_USER_SERVICE_USER_ID_SYSTEM) || */ index != 0)
			{
				// Document states that userId must be SCE_USER_SERVICE_USER_ID_SYSTEM
				// but sample code use 0 and Nier use user id.
				// Fuck that..
				result = SCE_VIDEO_OUT_ERROR_INVALID_VALUE;
				break;
			}

			uint32_t typeIndex = typeIndexTable[type];
			if (m_videoOutSlots[typeIndex] != nullptr)
			{
				result = SCE_VIDEO_OUT_ERROR_RESOURCE_BUSY;
				break;
			}

			m_videoOutSlots[typeIndex] = std::make_shared<SceVideoOut>(type, param);

			result = SceVideoOutPortBase + typeIndex;
		} while (false);
		return result;
	}

	int VirtualGPU::videoOutClose(int32_t handle)
	{
		int result = SCE_VIDEO_OUT_ERROR_INVALID_HANDLE;
		do 
		{
			if (SCE_ERROR_IS_FAILURE(handle))
			{
				// If the handle is an error code,
				// we do nothing.
				break;
			}

			uint32_t index = handle - SceVideoOutPortBase;
			m_videoOutSlots[index].reset();

			result = SCE_OK;
		} while (false);

		return result;
	}

	SceVideoOut& VirtualGPU::videoOutGet(int32_t handle)
	{
		uint32_t index = handle - SceVideoOutPortBase;
		return *m_videoOutSlots[index];
	}

	SceGnmDriver& VirtualGPU::gnmDriver()
	{
		return *m_gnmDriver;
	}

	Gnm::GpuMode VirtualGPU::mode()
	{
		return Gnm::kGpuModeNeo;
	}

}  // namespace sce