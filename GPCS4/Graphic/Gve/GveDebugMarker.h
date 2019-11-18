#pragma once
#include "GveCommon.h"


namespace gve
{;

class GveDevice;

class GveDebugMarker
{
public:
	GveDebugMarker();
	GveDebugMarker(GveDevice* device);
	~GveDebugMarker();

	void setObjectName(uint64_t object, VkDebugReportObjectTypeEXT objType, const char* objName);

	void setObjectTag(uint64_t object, VkDebugReportObjectTypeEXT objType,
		void* tagData, size_t tagSize, uint64_t tagName = 0);

	void cmdMarkerBegin(VkCommandBuffer command, const char* marker);

	void cmdMarkerEnd(VkCommandBuffer command);

	void cmdMarkerInsert(VkCommandBuffer command, const char* marker);

private:
	GveDevice* m_device;
};

}  // namespace gve
