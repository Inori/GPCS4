#include "GveDebugMarker.h"
#include "GveDevice.h"

namespace gve
{;


GveDebugMarker::GveDebugMarker():
	m_device(nullptr)
{

}

GveDebugMarker::GveDebugMarker(GveDevice* device):
	m_device(device)
{

}


GveDebugMarker::~GveDebugMarker()
{

}

void GveDebugMarker::setObjectName(uint64_t object, VkDebugReportObjectTypeEXT objType, const char* objName)
{
#ifdef GVE_DEBUG_MARKER
	VkDebugMarkerObjectNameInfoEXT nameInfo = {};
	nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT;
	nameInfo.pNext = nullptr;
	nameInfo.object = object;
	nameInfo.objectType = objType;
	nameInfo.pObjectName = objName;
	vkDebugMarkerSetObjectNameEXT(*m_device, &nameInfo);
#endif  // GVE_DEBUG_MARKER
}

void GveDebugMarker::setObjectTag(uint64_t object, VkDebugReportObjectTypeEXT objType, 
	void* tagData, size_t tagSize, uint64_t tagName /*= 0*/)
{
#ifdef GVE_DEBUG_MARKER
	VkDebugMarkerObjectTagInfoEXT tagInfo = {};
	tagInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_TAG_INFO_EXT;
	tagInfo.pNext = nullptr;
	tagInfo.object = object;
	tagInfo.objectType = objType;
	tagInfo.tagName = tagName;
	tagInfo.pTag = tagData;
	tagInfo.tagSize = tagSize;
	vkDebugMarkerSetObjectTagEXT(*m_device, &tagInfo);
#endif  // GVE_DEBUG_MARKER
}

void GveDebugMarker::cmdMarkerBegin(VkCommandBuffer command, const char* marker)
{
#ifdef GVE_DEBUG_MARKER
	VkDebugMarkerMarkerInfoEXT markerInfo = {};
	markerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
	markerInfo.pNext = nullptr;
	markerInfo.pMarkerName = marker;
	vkCmdDebugMarkerBeginEXT(command, &markerInfo);
#endif  // GVE_DEBUG_MARKER
}

void GveDebugMarker::cmdMarkerEnd(VkCommandBuffer command)
{
#ifdef GVE_DEBUG_MARKER
	vkCmdDebugMarkerEndEXT(command);
#endif  // GVE_DEBUG_MARKER
}

void GveDebugMarker::cmdMarkerInsert(VkCommandBuffer command, const char* marker)
{
#ifdef GVE_DEBUG_MARKER
	VkDebugMarkerMarkerInfoEXT markerInfo = {};
	markerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
	markerInfo.pNext = nullptr;
	markerInfo.pMarkerName = marker;
	vkCmdDebugMarkerInsertEXT(command, &markerInfo);
#endif  // GVE_DEBUG_MARKER
}

}  // namespace gve