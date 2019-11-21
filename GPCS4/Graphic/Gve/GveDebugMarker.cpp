#include "GveDebugMarker.h"
#include "GveDevice.h"

namespace gve
{;



PFN_vkDebugMarkerSetObjectTagEXT GveDebugMarker::pfnDebugMarkerSetObjectTag = VK_NULL_HANDLE;
PFN_vkDebugMarkerSetObjectNameEXT GveDebugMarker::pfnDebugMarkerSetObjectName = VK_NULL_HANDLE;
PFN_vkCmdDebugMarkerBeginEXT GveDebugMarker::pfnCmdDebugMarkerBegin = VK_NULL_HANDLE;
PFN_vkCmdDebugMarkerEndEXT GveDebugMarker::pfnCmdDebugMarkerEnd = VK_NULL_HANDLE;
PFN_vkCmdDebugMarkerInsertEXT GveDebugMarker::pfnCmdDebugMarkerInsert = VK_NULL_HANDLE;

GveDebugMarker::GveDebugMarker(GveDevice* device):
	m_device(device)
{
#ifdef GVE_DEBUG_MARKER
	setupDebugFunctions();
#endif  // GVE_DEBUG_MARKER
}


GveDebugMarker::~GveDebugMarker()
{

}

void GveDebugMarker::setupDebugFunctions()
{
	pfnDebugMarkerSetObjectTag = reinterpret_cast<PFN_vkDebugMarkerSetObjectTagEXT>(vkGetDeviceProcAddr(*m_device, "vkDebugMarkerSetObjectTagEXT"));
	pfnDebugMarkerSetObjectName = reinterpret_cast<PFN_vkDebugMarkerSetObjectNameEXT>(vkGetDeviceProcAddr(*m_device, "vkDebugMarkerSetObjectNameEXT"));
	pfnCmdDebugMarkerBegin = reinterpret_cast<PFN_vkCmdDebugMarkerBeginEXT>(vkGetDeviceProcAddr(*m_device, "vkCmdDebugMarkerBeginEXT"));
	pfnCmdDebugMarkerEnd = reinterpret_cast<PFN_vkCmdDebugMarkerEndEXT>(vkGetDeviceProcAddr(*m_device, "vkCmdDebugMarkerEndEXT"));
	pfnCmdDebugMarkerInsert = reinterpret_cast<PFN_vkCmdDebugMarkerInsertEXT>(vkGetDeviceProcAddr(*m_device, "vkCmdDebugMarkerInsertEXT"));
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
	pfnDebugMarkerSetObjectName(*m_device, &nameInfo);
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
	pfnDebugMarkerSetObjectTag(*m_device, &tagInfo);
#endif  // GVE_DEBUG_MARKER
}

void GveDebugMarker::cmdMarkerBegin(VkCommandBuffer command, const char* marker)
{
#ifdef GVE_DEBUG_MARKER
	VkDebugMarkerMarkerInfoEXT markerInfo = {};
	markerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
	markerInfo.pNext = nullptr;
	markerInfo.pMarkerName = marker;
	pfnCmdDebugMarkerBegin(command, &markerInfo);
#endif  // GVE_DEBUG_MARKER
}

void GveDebugMarker::cmdMarkerEnd(VkCommandBuffer command)
{
#ifdef GVE_DEBUG_MARKER
	pfnCmdDebugMarkerEnd(command);
#endif  // GVE_DEBUG_MARKER
}

void GveDebugMarker::cmdMarkerInsert(VkCommandBuffer command, const char* marker)
{
#ifdef GVE_DEBUG_MARKER
	VkDebugMarkerMarkerInfoEXT markerInfo = {};
	markerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
	markerInfo.pNext = nullptr;
	markerInfo.pMarkerName = marker;
	pfnCmdDebugMarkerInsert(command, &markerInfo);
#endif  // GVE_DEBUG_MARKER
}



}  // namespace gve