#pragma once
#include "VltCommon.h"


namespace vlt
{;

class VltDevice;

class VltDebugMarker
{
public:
	
	VltDebugMarker(VltDevice* device);
	~VltDebugMarker();

	void setObjectName(uint64_t object, VkDebugReportObjectTypeEXT objType, const char* objName);

	void setObjectTag(uint64_t object, VkDebugReportObjectTypeEXT objType,
		void* tagData, size_t tagSize, uint64_t tagName = 0);

	void cmdMarkerBegin(VkCommandBuffer command, const char* marker);

	void cmdMarkerEnd(VkCommandBuffer command);

	void cmdMarkerInsert(VkCommandBuffer command, const char* marker);

private:
	void setupDebugFunctions();

private:
	VltDevice* m_device;

	static PFN_vkDebugMarkerSetObjectTagEXT pfnDebugMarkerSetObjectTag;
	static PFN_vkDebugMarkerSetObjectNameEXT pfnDebugMarkerSetObjectName;
	static PFN_vkCmdDebugMarkerBeginEXT pfnCmdDebugMarkerBegin;
	static PFN_vkCmdDebugMarkerEndEXT pfnCmdDebugMarkerEnd;
	static PFN_vkCmdDebugMarkerInsertEXT pfnCmdDebugMarkerInsert;
};

}  // namespace vlt
